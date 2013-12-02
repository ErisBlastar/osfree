/* 
 * (C) Copyright 1992, ..., 2007 the "DOSEMU-Development-Team".
 *
 * for details see file COPYING.DOSEMU in the DOSEMU distribution
 */

/* xms.c for the DOS emulator
 *       Robert Sanders, gt8134b@prism.gatech.edu
 *
 * DANG_BEGIN_MODULE
 *
 * REMARK
 * Currently the XMS 3.0 spec is covered in this file. XMS is fairly simple
 * as it only deals with allocating extended memory and then moving it 
 * around in specific calls. This spec also includes the allocation of UMB's,
 * so they are also included as part of this file. The amount of xms memory
 * returned to DOS programs via the XMS requests, or int15 fnc88 is set in
 * "/etc/dosemu.conf" via the XMS parameter.
 *
 * /REMARK
 * DANG_END_MODULE
 *
 * NOTE: I keep the BYTE size of EMB's in a field called "size" in the EMB
 *    structure.  Most XMS calls specify/expect size in KILOBYTES.
 */


#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "emu.h"
#include "config.h"
#include "memory.h"
#include "xms.h"
#include "hlt.h"
#include "int.h"
#include "hma.h"
#include "machcompat.h"
#include "dos2linux.h"
#include "cpu-emu.h"
#include "smalloc.h"

#undef  DEBUG_XMS

static int umb_find_unused(void);
static void xms_control(void);

/*
static char RCSxms[] = "$Id: xms.c 1769 2007-05-04 05:59:48Z bartoldeman $";
*/

#define	 XMS_GET_VERSION		0x00
#define	 XMS_ALLOCATE_HIGH_MEMORY	0x01
#define	 XMS_FREE_HIGH_MEMORY		0x02
#define	 XMS_GLOBAL_ENABLE_A20		0x03
#define	 XMS_GLOBAL_DISABLE_A20		0x04
#define	 XMS_LOCAL_ENABLE_A20		0x05
#define	 XMS_LOCAL_DISABLE_A20		0x06
#define	 XMS_QUERY_A20			0x07
#define	 XMS_QUERY_FREE_EXTENDED_MEMORY	0x08
#define	 XMS_ALLOCATE_EXTENDED_MEMORY	0x09
#define	 XMS_FREE_EXTENDED_MEMORY	0x0a
#define	 XMS_MOVE_EXTENDED_MEMORY_BLOCK	0x0b
#define	 XMS_LOCK_EXTENDED_MEMORY_BLOCK	0x0c
#define	 XMS_UNLOCK_EXTENDED_MEMORY_BLOCK 0x0d
#define	 XMS_GET_EMB_HANDLE_INFORMATION	0x0e
#define	 XMS_RESIZE_EXTENDED_MEMORY_BLOCK 0x0f
#define	 XMS_ALLOCATE_UMB		0x10
#define	 XMS_DEALLOCATE_UMB		0x11

#define	HIGH_MEMORY_IN_USE		0x92
#define	HIGH_MEMORY_NOT_ALLOCATED	0x93
#define XMS_OUT_OF_SPACE		0xa0
#define XMS_INVALID_HANDLE		0xa2

static int a20_local, a20_global, freeHMA;	/* is HMA free? */

static struct Handle handles[NUM_HANDLES + 1];
static int handle_count = 0;

struct EMM get_emm(unsigned int, unsigned int);
void show_emm(struct EMM);
static unsigned char xms_query_freemem(int), xms_allocate_EMB(int), xms_free_EMB(void),
 xms_move_EMB(void), xms_lock_EMB(int), xms_EMB_info(int), xms_realloc_EMB(int);

static int FindFreeHandle(int);

/* beginning of quote from Mach */

/* technically, UMBS needs to be UMB_SIZE/16 since some fool could allocate
   a million 16 byte entries.  In reality, there's usually about 128K of
   UMB's tops and people tend to allocate it in large chunks.  UMBS is
   currently 512 (as opposed to UMB_SIZE/16 or 16K entries) which is
   probably still insanely high.  Ah linear table searching... -- scottb */
#define UMB_BASE (caddr_t)0xc0000
#define UMB_SIZE 0x40000
#define UMBS 512
#define UMB_PAGE 4096
#define UMB_NULL -1

#if 0
/* EMS page frame 0xd0000-0xdffff.  DOSEMU uses 0xe0000-0xeffff */
#define IN_EMM_SPACE(addr) (config.ems_size && (int)(addr) >= 0xd0000 \
			    && (int)(addr) <= 0xdffff)
#else
/*  EMS page frame now is configurable via /etc/dosemu.conf */
#define SPACE_64K   0x10000
#define IN_EMM_SPACE(addr) ((config.ems_size || config.pm_dos_api) \
			    && (int)(addr) >= EMM_BASE_ADDRESS \
                            && (int)(addr) < (EMM_BASE_ADDRESS+SPACE_64K))

#define IN_HARDWARE_PAGES(addr) ( config.must_spare_hardware_ram\
                                  && ((int)addr >= HARDWARE_RAM_START) \
                                  && ((int)addr < HARDWARE_RAM_STOP) \
                                  && ( config.hardware_pages[((int)addr-HARDWARE_RAM_START) >> 12] ) )
#endif

/* XXX - I have assumed 32k of BIOS... */
#define IN_BIOS_SPACE(addr) ((int)(addr) >= VBIOS_START && \
      (int)(addr) < (VBIOS_START + VBIOS_SIZE) && config.mapped_bios)

#define IN_EMU_SPACE(addr) (((int)(addr) >= (BIOSSEG*16) && (int)(addr) <= \
			    (BIOSSEG*16 + 0xffff)) || IN_BIOS_SPACE(addr))

static struct umb_record {
  vm_address_t addr;
  vm_size_t size;
  boolean_t in_use;
  boolean_t free;
} umbs[UMBS];

#define x_Stub(arg1, s, a...)   x_printf("XMS: "s, ##a)
#define Debug0(args)		x_Stub args
#define Debug1(args)		x_Stub args
#define Debug2(args)		x_Stub args
/* #define dbg_fd stderr */

static int
umb_setup(void)
{
  int i;
  size_t addr_start;
  int size, umb;

  for (i = 0; i < UMBS; i++) {
    umbs[i].in_use = FALSE;
  }

  memcheck_addtype('U', "Upper Memory Block (UMB, XMS 3.0)");

  addr_start = 0x00000;     /* start address */
  while ((size = memcheck_findhole(&addr_start, 1024, 0x100000)) != 0) {
    Debug0((dbg_fd, "findhole - from 0x%5.5zX, %dKb\n", addr_start, size/1024));
    memcheck_reserve('U', addr_start, size);

    umb = umb_find_unused();
    umbs[umb].in_use = TRUE;
    umbs[umb].free = TRUE;
    umbs[umb].addr = (vm_address_t)(uintptr_t) addr_start;
    umbs[umb].size = size;
  }

  for (i = 0; i < UMBS; i++) {
    if (umbs[i].in_use && umbs[i].free) {
      vm_address_t addr = umbs[i].addr;
      vm_size_t size = umbs[i].size;
#if 0
      MACH_CALL((vm_deallocate(mach_task_self(),
			       (vm_address_t) addr,
			       (vm_size_t) size)), "vm_deallocate");
      MACH_CALL((vm_allocate(mach_task_self(), &addr,
			     (vm_size_t) size,
			     FALSE)),
		"vm_allocate of umb block.");
#else
      Debug0((dbg_fd, "umb_setup: addr %p size 0x%04zx\n",
	      (void *) addr, size));
#endif
    }
  }

  return 0;
}

static int
umb_find_unused(void)
{
  int i;

  for (i = 0; i < UMBS; i++) {
    if (!umbs[i].in_use)
      return (i);
  }
  return (UMB_NULL);
}

static int
umb_find(vm_address_t segbase)
{
  int i;
  vm_address_t addr = SEG2LINEAR(segbase);

  for (i = 0; i < UMBS; i++) {
    if (umbs[i].in_use &&
	((addr >= umbs[i].addr) &&
	 (addr < (umbs[i].addr + umbs[i].size)))) {
      return (i);
    }
  }
  return (UMB_NULL);
}

static vm_address_t
umb_allocate(int size)
{
  int i;

  for (i = 0; i < UMBS; i++) {
    if (umbs[i].in_use && umbs[i].free) {
      if (umbs[i].size > size) {
	int new_umb = umb_find_unused();

	if (new_umb != UMB_NULL) {
	 if(size) {
	  umbs[new_umb].in_use = TRUE;
	  umbs[new_umb].free = TRUE;
	  umbs[new_umb].addr =
	    umbs[i].addr + size;
	  umbs[new_umb].size =
	    umbs[i].size - size;
	  umbs[i].size = size;
	  umbs[i].free = FALSE;
	 }
	 return (umbs[i].addr);
	}
      }
      else
	if (umbs[i].size == size) {
	  umbs[i].free = FALSE;
	  return (umbs[i].addr);
	}
    }
  }
  return ((vm_address_t) 0);
}

static void umb_cleanup(int umb)
{
  vm_address_t umb_top = umbs[umb].addr + umbs[umb].size;
  int i, updated;

  do {
    updated = 0;
    for (i = 0; i < UMBS; i++) {
      if (umbs[i].in_use && umbs[i].free) {
	if (umbs[i].addr == umb_top) {
          umbs[umb].size += umbs[i].size;
          umb_top         = umbs[umb].addr + umbs[umb].size;
          umbs[i].in_use  = 0;
	  updated         = 1;
	  continue;
	}
	if (umbs[i].addr + umbs[i].size == umbs[umb].addr) {
	  umbs[umb].addr  = umbs[i].addr;
	  umbs[umb].size += umbs[i].size;
	  umb_top         = umbs[umb].addr + umbs[umb].size;
	  umbs[i].in_use  = 0;
	  updated         = 1;
	}
      }
    }
  } while (updated);
}

static void umb_free_all(void)
{
  int i;
  for (i = 0; i < UMBS; i++) {
    if (umbs[i].in_use && !umbs[i].free) {
      umbs[i].free = TRUE;
      umb_cleanup(i);
    }
  }
}

/* why int? */
static int
umb_free(int segbase)
{
  int umb = umb_find((vm_address_t)(uintptr_t)segbase);

  if (umb != UMB_NULL) {
    umbs[umb].free = TRUE;
    umb_cleanup(umb);
  }
  return (0);
}

static int
umb_query(void)
{
  int i;
  int largest = 0;

  for (i = 0; i < UMBS; i++) {
    if (umbs[i].in_use && umbs[i].free) {
      if (umbs[i].size > largest)
	largest = umbs[i].size;
    }
  }
  Debug0((dbg_fd, "umb_query: largest UMB was %d(%#x) bytes\n",
	largest, largest));
  return (largest);
}

/* end of stuff from Mach */

static smpool mp = SM_EMPTY_POOL;

void
xms_reset(void)
{
  umb_free_all();
  config.xms_size = 0;
}


void xms_helper(void)
{
  int i;

  if (!config.ext_mem)
    return;

  LWORD(eax) = 0;	/* report success */

  if (config.xms_size)
    return;

  config.xms_size = EXTMEM_SIZE >> 10;
  x_printf("XMS: initializing XMS... %d handles\n", NUM_HANDLES);

  freeHMA = 1;
  a20_global = a20_local = 0;

  handle_count = 0;
  for (i = 0; i < NUM_HANDLES + 1; i++) {
    if (handles[i].valid && handles[i].addr)
      smfree(&mp, handles[i].addr);
    handles[i].valid = 0;
  }

  smdestroy(&mp);
  sminit(&mp, ext_mem_base, config.xms_size * 1024);
}

void
xms_init(void)
{
  emu_hlt_t hlt_hdlr;

  umb_setup();

  hlt_hdlr.name = "XMS";
  hlt_hdlr.start_addr = XMSControl_ADD - BIOS_HLT_BLK;
  hlt_hdlr.end_addr = hlt_hdlr.start_addr + 5;
  hlt_hdlr.func = (emu_hlt_func)xms_control;
  hlt_register_handler(hlt_hdlr);
}

static void XMS_RET(int err)
{
  /* careful here: BL should NOT be set to 0 if there is no error,
     in particular for "get handle information" that is wrong,
     but also for locking a handle.
     An exception is xms_query_freemem, which sets BL to zero
     itself, and does not use AX to report an error */
  LWORD(eax) = err ? 0 : 1;
  if (err)
    LO(bx) = err;
}

static void xms_control(void)
{
  int is_umb_fn = 0;

 /* First do the UMB functions */
 switch (HI(ax)) {
  case XMS_ALLOCATE_UMB:
    {
      int size = LWORD(edx) << 4;
      vm_address_t addr = umb_allocate(size);
      is_umb_fn = 1;
      Debug0((dbg_fd, "Allocate UMB memory: %#x\n", size));
      if (addr == (vm_address_t) 0) {
        int avail=umb_query();

	Debug0((dbg_fd, "Allocate UMB Failure\n"));
	XMS_RET(avail ? 0xb0 : 0xb1);
	LWORD(edx) = avail >> 4;
      }
      else {
	Debug0((dbg_fd, "Allocate UMB Success\n"));
	LWORD(eax) = 1;
	LWORD(ebx) = (unsigned long)addr >> 4;
	LWORD(edx) = size >> 4;
      }
      Debug0((dbg_fd, "umb_allocated: %#x0:%#x0\n",
	      (unsigned) LWORD(ebx),
	      (unsigned) LWORD(edx)));
      /* retval = UNCHANGED; */
      break;
    }

  case XMS_DEALLOCATE_UMB:
    {
      is_umb_fn = 1;
      umb_free(LWORD(edx));
      XMS_RET(0);			/* no error */
      Debug0((dbg_fd, "umb_freed: 0x%04x\n",
	      (unsigned) LWORD(edx)));
      /* retval = UNCHANGED; */
      break;
    }

  case 0x12:
    is_umb_fn = 1;
    x_printf("XMS realloc UMB segment 0x%04x size 0x%04x\n",
	     LWORD(edx), LWORD(ebx));
    XMS_RET(0x80);		        /* function unimplemented */
    break;
 }

 if (config.xms_size && !is_umb_fn) {
  switch (HI(ax)) {
  case 0:			/* Get XMS Version Number */
    LWORD(eax) = XMS_VERSION;
    LWORD(ebx) = XMS_DRIVER_VERSION;
    LWORD(edx) = 1;		/* HMA exists */
    x_printf("XMS driver version. XMS Spec: %04x dosemu-XMS: %04x\n",
	     LWORD(eax), LWORD(ebx));
    break;

  case 1:			/* Request High Memory Area */
    x_printf("XMS request HMA size 0x%04x\n", LWORD(edx));
    if (freeHMA) {
      x_printf("XMS: allocating HMA size 0x%04x\n", LWORD(edx));
      freeHMA = 0;
      XMS_RET(0);			/* no error */
    }
    else {
      x_printf("XMS: HMA already allocated\n");
      XMS_RET(0x91);
    }
    break;

  case 2:			/* Release High Memory Area */
    x_printf("XMS release HMA\n");
    if (freeHMA) {
      x_printf("XMS: HMA already free\n");
      XMS_RET(0x93);
    }
    else {
      x_printf("XMS: freeing HMA\n");
      freeHMA = 1;
      XMS_RET(0);			/* no error */
    }
    break;

  case 3:			/* Global Enable A20 */
    x_printf("XMS global enable A20\n");
    if (!a20_global)
      set_a20(1);		/* map in HMA */
    a20_global = 1;
    XMS_RET(0);			/* no error */
    break;

  case 4:			/* Global Disable A20 */
    x_printf("XMS global disable A20\n");
    if (a20_global)
      set_a20(0);
    a20_global = 0;
    XMS_RET(0);			/* no error */
    break;

  case 5:			/* Local Enable A20 */
    x_printf("XMS LOCAL enable A20\n");
    if (!a20_local)
      set_a20(1);
    a20_local++;
    XMS_RET(0);			/* no error */
    break;

  case 6:			/* Local Disable A20 */
    x_printf("XMS LOCAL disable A20\n");
    if (a20_local)
      a20_local--;
    if (!a20_local)
      set_a20(0);
    XMS_RET(0);			/* no error */
    break;

    /* DOS, if loaded as "dos=high,umb" will continually poll
	  * the A20 line.
          */
  case 7:			/* Query A20 */
    LWORD(eax) = a20 ? 1 : 0;
    LO(bx) = 0;			/* no error */
    break;

  case 8:			/* Query Free Extended Memory */
    xms_query_freemem(OLDXMS);
    break;

  case 9:			/* Allocate Extended Memory Block */
    XMS_RET(xms_allocate_EMB(OLDXMS));
    break;

  case 0xa:			/* Free Extended Memory Block */
    XMS_RET(xms_free_EMB());
    break;

  case 0xb:			/* Move Extended Memory Block */
    XMS_RET(xms_move_EMB());
    break;

  case 0xc:			/* Lock Extended Memory Block */
    XMS_RET(xms_lock_EMB(1));
    break;

  case 0xd:			/* Unlock Extended Memory Block */
    XMS_RET(xms_lock_EMB(0));
    break;

  case 0xe:			/* Get EMB Handle Information */
    XMS_RET(xms_EMB_info(OLDXMS));
    break;

  case 0xf:			/* Reallocate Extended Memory Block */
    XMS_RET(xms_realloc_EMB(OLDXMS));
    break;

    /* the functions below are the newer, 32-bit XMS 3.0 interface */

  case 0x88:			/* Query Any Free Extended Memory */
    xms_query_freemem(NEWXMS);
    break;

  case 0x89:			/* Allocate Extended Memory */
    XMS_RET(xms_allocate_EMB(NEWXMS));
    break;

  case 0x8e:			/* Get EMB Handle Information */
    XMS_RET(xms_EMB_info(NEWXMS));
    break;

  case 0x8f:			/* Reallocate EMB */
    XMS_RET(xms_realloc_EMB(NEWXMS));
    break;

  default:
    x_printf("XMS: Unimplemented XMS function AX=0x%04x", LWORD(eax));
    show_regs(__FILE__, __LINE__);		/* if you delete this, put the \n on the line above */
    XMS_RET(0x80);		/* function not implemented */
  }
 }
 /* If this is the UMB request which came via the external himem.sys,
  * dont pass it back. */
 if (!config.xms_size) {
   if (is_umb_fn)
     LWORD(esp) += 4;
   else
     x_printf("XMS: skipping external request, ax=0x%04x, dx=0x%04x\n",
	      LWORD(eax), LWORD(edx));
 }
 fake_retf(0);
}

static int
FindFreeHandle(int start)
{
  int i, h = 0;

  /* first free handle is 1 */
  for (i = start; (i <= NUM_HANDLES) && (h == 0); i++) {
    if (!handles[i].valid) {
      x_printf("XMS: found free handle: %d\n", i);
      h = i;
      break;
    }
    else
      x_printf("XMS: unfree handle %d ", i);
  }
  return h;
}

static int
ValidHandle(unsigned short h)
{
  if ((h <= NUM_HANDLES) && (handles[h].valid))
    return 1;
  else
    return 0;
}

static unsigned char
xms_query_freemem(int api)
{
  unsigned long totalBytes = 0, subtotal, largest;
  int h;

  /* the new XMS api should actually work with the function as it
   * stands...
   */
  if (api == NEWXMS)
    x_printf("XMS: new XMS API query_freemem()!\n");

  /* total XMS mem and largest block can be different because of
   *  fragmentation
   */

  totalBytes = 0;
  for (h = FIRST_HANDLE; h <= NUM_HANDLES; h++) {
    if (ValidHandle(h))
      totalBytes += handles[h].size;
  }

  subtotal = config.xms_size - (totalBytes / 1024);
  if (debug_level('x')) {
    if (smget_free_space(&mp) != subtotal * 1024)
      x_printf("XMS smalloc mismatch!!!\n");
  }
  /* total free is max allowable XMS - the number of K already allocated */

  largest = smget_largest_free_area(&mp) / 1024;

  if (api == OLDXMS) {
    /* old XMS API uses only AX, while new API uses EAX. make
       * sure the old API sees 64 MB if the count >= 64 MB (instead of seeing
       * just the low 16 bits of some number > 65535 (KBytes) which is
       * probably < 64 MB...this makes sure that old API functions at least
       * know of 64MB of the > 64MB available memory.
       */

    if (largest > 65535)
      largest = 65535;
    if (subtotal > 65535)
      subtotal = 65535;
    LWORD(eax) = largest;
    LWORD(edx) = subtotal;
    x_printf("XMS query free memory(old): %dK %dK\n", LWORD(eax),
	     LWORD(edx));
  }
  else {
    REG(eax) = largest;
    REG(edx) = subtotal;
    x_printf("XMS query free memory(new): %ldK %ldK\n",
	     REG(eax), REG(edx));
  }
  /* the following line is NOT superfluous!! (see above) */
  LO(bx) = 0;
  return 0;
}

static unsigned char
xms_allocate_EMB(int api)
{
  unsigned long totalBytes, subtotal;
  unsigned long h;
  unsigned long kbsize;

  if (api == OLDXMS)
    kbsize = LWORD(edx);
  else
    kbsize = REG(edx);
  x_printf("XMS alloc EMB(%s) size %ld KB\n", (api==OLDXMS)?"old":"new",kbsize);

  totalBytes = 0;
  for (h = FIRST_HANDLE; h <= NUM_HANDLES; h++) {
    if (ValidHandle(h))
      totalBytes += handles[h].size;
  }

  subtotal = config.xms_size - (totalBytes / 1024);
  /* total free is max allowable XMS - the number of K already allocated */
  if(kbsize > subtotal)
  {
    x_printf("XMS: out of memory (only %ldK available)\n",subtotal);
    return 0xa0; /* Out of memory */
  }
  
  if (!(h = FindFreeHandle(FIRST_HANDLE))) {
    x_printf("XMS: out of handles\n");
    return 0xa1;
  }
  else {
    handles[h].num = h;
    handles[h].valid = 1;
    handles[h].size = kbsize*1024;

    x_printf("XMS: EMB size %ld bytes\n", handles[h].size);

    /* I could just rely on the behavior of malloc(0) here, but
       * I'd rather not.  I'm going to interpret the XMS 3.0 spec
       * to mean that reserving a handle of size 0 gives it no address
       */
    if (handles[h].size)
      handles[h].addr = smalloc(&mp, handles[h].size);
    else {
      x_printf("XMS WARNING: allocating 0 size EMB\n");
      handles[h].addr = 0;
    }

    handles[h].lockcount = 0;
    handle_count++;

    x_printf("XMS: allocated EMB %lu at %p\n", h, (void *) handles[h].addr);

    if (api == OLDXMS)
      LWORD(edx) = h;		/* handle # */
    else
      REG(edx) = h;
    return 0;
  }
}

static unsigned char
xms_free_EMB(void)
{
  unsigned short int h = LWORD(edx);

  if (!ValidHandle(h)) {
    x_printf("XMS: free EMB bad handle %d\n", h);
    return 0xa2;
  }
  else {

    if (handles[h].addr)
      smfree(&mp, handles[h].addr);
    else
      x_printf("XMS WARNING: freeing handle w/no address, size 0x%08lx\n",
	       handles[h].size);
    handles[h].valid = 0;
    handle_count--;

    x_printf("XMS: free'd EMB %d\n", h);
    return 0;
  }
}

static unsigned char
xms_move_EMB(void)
{
  char *src, *dest;
  struct EMM e = get_emm(REG(ds), LWORD(esi));

  x_printf("XMS move extended memory block\n");
  show_emm(e);

  if (e.SourceHandle == 0) {
    src = (char *) (((e.SourceOffset >> 16) << 4) + \
		    (e.SourceOffset & 0xffff));
  }
  else {
    if (handles[e.SourceHandle].valid == 0) {
      x_printf("XMS: invalid source handle\n");
      return 0xa3;
    }
    src = handles[e.SourceHandle].addr + e.SourceOffset;
    if (src > handles[e.SourceHandle].addr + handles[e.SourceHandle].size)
      return 0xa4;              /* invalid source offset */
    if (src + e.Length > handles[e.SourceHandle].addr + handles[e.SourceHandle].size)
      return 0xa7;              /* invalid Length */
  }

  if (e.DestHandle == 0) {
    dest = (char *) (((e.DestOffset >> 16) << 4) + \
		     (e.DestOffset & 0xffff));
  }
  else {
    if (handles[e.DestHandle].valid == 0) {
      x_printf("XMS: invalid dest handle\n");
      return 0xa5;
    }
    dest = handles[e.DestHandle].addr + e.DestOffset;
    if (dest > handles[e.DestHandle].addr + handles[e.DestHandle].size)
      return 0xa6;              /* invalid dest offset */
    if (dest + e.Length > handles[e.DestHandle].addr + handles[e.DestHandle].size) {
      return 0xa7;		/* invalid Length */
    }
  }

  x_printf("XMS: block move from %p to %p len 0x%lx\n",
	   (void *) src, (void *) dest, e.Length);
  memmove_dos2dos(dest, src, e.Length);
  if ((size_t)dest < 0x110000)
    e_invalidate(dest, e.Length);
  x_printf("XMS: block move done\n");

  return 0;
}

static unsigned char
xms_lock_EMB(int flag)
{
  int h = LWORD(edx);
  unsigned addr;

  if (flag)
    x_printf("XMS lock EMB %d\n", h);
  else
    x_printf("XMS unlock EMB %d\n", h);

  if (ValidHandle(h)) {
    /* flag=1 locks, flag=0 unlocks */

    if (flag)
      handles[h].lockcount++;
    else
      if (handles[h].lockcount)
        handles[h].lockcount--;
      else {
        x_printf("XMS: Unlock handle %d already at 0\n", h);
        return 0xaa;		/* Block is not locked */
      }

    addr = handles[h].addr - (char *)ext_mem_base + LOWMEM_SIZE + HMASIZE;
    LWORD(edx) = addr >> 16;
    LWORD(ebx) = addr & 0xffff;
    return 0;
  }
  else {
    x_printf("XMS: invalid handle %d, can't (un)lock\n", h);
    return 0xa2;		/* invalid handle */
  }
}

static unsigned char
xms_EMB_info(int api)
{
  int h = LWORD(edx);

  if (ValidHandle(h)) {
    if (api == OLDXMS) {
      LO(bx) = NUM_HANDLES - handle_count;
      LWORD(edx) = handles[h].size / 1024;
      x_printf("XMS Get EMB info(old) %d\n", h);
    }
    else {			/* api == NEWXMS */
      LWORD(ecx) = NUM_HANDLES - handle_count;
      REG(edx) = handles[h].size / 1024;
      x_printf("XMS Get EMB info(new) %d\n", h);
    }
    HI(bx) = handles[h].lockcount;
    return 0;
  }
  else {
    return 0xa2;		/* invalid handle */
  }
}

/* untested! if you test it, please tell me! */
static unsigned char
xms_realloc_EMB(int api)
{
  int h;
  unsigned long oldsize;
  unsigned char *oldaddr;

  h = LWORD(edx);

  if (!ValidHandle(h)) {
    x_printf("XMS: invalid handle %d, cannot realloc\n", h);
    return 0xa2;
  }
  else if (handles[h].lockcount) {
    x_printf("XMS: handle %d locked (%d), cannot realloc\n",
	     h, handles[h].lockcount);
    return 0xab;
  }

  oldsize = handles[h].size;
  oldaddr = handles[h].addr;

  /* BX is the 16-bit new size in KBytes for the old API, but the new
   * XMS 3.0 32-bit API uses EBX
   */
  if (api == OLDXMS)
    handles[h].size = LWORD(ebx) * 1024;
  else
    handles[h].size = REG(ebx) * 1024;

  x_printf((api == OLDXMS) ? "XMS realloc EMB(old) %d to size 0x%04lx\n" :
	   "XMS realloc EMB(new) %d to size 0x%08lx\n",
	   h, handles[h].size);

  handles[h].addr = smalloc(&mp, handles[h].size);

  /* memcpy() the old data into the new block, but only
   * min(new,old) bytes.
   */
  memcpy(handles[h].addr, oldaddr,
	 (oldsize <= handles[h].size) ? oldsize : handles[h].size);

  /* free the EMB's old Linux memory */
  smfree(&mp, oldaddr);
  return 0;
}

struct EMM
get_emm(unsigned int seg, unsigned int off)
{
  struct EMM e;
  unsigned char *p;

  p = MK_FP32(seg, off);

  e.Length = *(unsigned int *) p;
  p += 4;
  e.SourceHandle = *(unsigned short *) p;
  p += 2;
  e.SourceOffset = *(unsigned int *) p;
  p += 4;
  e.DestHandle = *(unsigned short *) p;
  p += 2;
  e.DestOffset = *(unsigned int *) p;

  return e;
}

void
show_emm(struct EMM e)
{
  x_printf("XMS show_emm:\n");

  x_printf("L: 0x%08lx SH: 0x%04x  SO: 0x%08lx DH: 0x%04x  DO: 0x%08lx\n",
	   e.Length, e.SourceHandle, e.SourceOffset,
	   e.DestHandle, e.DestOffset);
}

