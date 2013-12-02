/*
 *
 */

#include <shared.h>
#include <loader.h>
#include <bpb.h>

#include "fsd.h"
#include "serial.h"

int pxe_probe(void);

int scrprintf(const char *format, ...);

int serial_init (long port, long speed,
                int word_len, int parity, int stop_bit_len);

int assign_drvletter (char *mode);
int offset (int part_no, int flag);

/* Config.sys preprocessor/editor callback */
struct multiboot_info *
(*callback)(unsigned long addr,
            unsigned long size,
            int drvltr,
            int drvltr2,
            struct term_entry *term);

/* a dummy for minilibc */
void u_msg (char *s)
{
}

#pragma aux stage0base "*"
unsigned long stage0base = REL1_BASE;

extern struct term_entry *t;

extern mu_Open;
extern mu_Read;
extern mu_Close;
extern mu_Terminate;

extern unsigned long  mfs_start;
extern unsigned long  mfs_len;
extern unsigned short ufs_len;

extern unsigned short boot_flags;
extern unsigned long  boot_drive;
extern FileTable      ft;
extern struct term_entry *t;

extern unsigned long  bss_end;
extern stack_bottom;

/* multiboot structure pointer */
extern struct multiboot_info *m;

extern char default_part_types[];
extern char part_types[];

int filemax;
int filepos;
int fileaddr;

unsigned long cdrom_drive;
int drvletter;
int drvletter2;

extern char debug;

#pragma aux u_msg        "*"
#pragma aux m            "*"
#pragma aux filemax      "*"
#pragma aux filepos      "*"
#pragma aux ufs_open     "*"
#pragma aux ufs_read     "*"
#pragma aux ufs_seek     "*"
#pragma aux ufs_close    "*"
#pragma aux ufs_term     "*"
#pragma aux boot_flags   "*"
#pragma aux boot_drive   "*"
#pragma aux ft           "*"
#pragma aux mu_Open      "*"
#pragma aux mu_Read      "*"
#pragma aux mu_Close     "*"
#pragma aux mu_Terminate "*"
#pragma aux stack_bottom "*"
#pragma aux callback     "*"
#pragma aux mfs_start    "*"
#pragma aux mfs_len      "*"
#pragma aux ufs_len      "*"

int toupper (int c);

int kprintf(const char *format, ...);
void comout(unsigned short port, unsigned char c);
void terminit(void);

int ufs_open (char *filename);
int ufs_read (char *buf, int len);
int ufs_seek (int offset);
void ufs_close (void);
void ufs_term (void);

void init (void)
{
}

void
patch_cfgsys(void)
{
  int i;

  if (ufs_open("CONFIG.SYS"))
  {
    kprintf("patch_cfgsys enter\n");

    /* Call config.sys preprocessor/editor
       routine outside microfsd            */
    m = callback(fileaddr, filemax, drvletter, drvletter2, t);
    ufs_close();

    kprintf("patch_cfgsys exit\n");
  }
}

void cmain (void)
{
  int  ldrbase = 0, mfsbase = 0;
  int  ldrlen = 0, mfslen = 0, ufslen = 0;
  char *buf;
  char type[8];
  unsigned long *p, *qq;
  unsigned long q;
  struct geometry geom;
  bios_parameters_block *bpb;
  long port = 0x3f8, t;
  long speed = 9600;
  char mode2[12];
  char mode[12];
  char *pp, *pp0, *r;
  int  i;

  // init terminal
  terminit ();

  if (m->flags & MB_INFO_CMDLINE)
  {
    if (pp = strstr((char *)m->cmdline, "--debug"))
    {
      debug = 1;
    }

    // if "--serial=..." specified on the command line
    if (pp = strstr((char *)m->cmdline, "--port"))
    {
      pp = skip_to(1, pp);
      safe_parse_maxint(&pp, &port);
    }

    if (pp = strstr((char *)m->cmdline, "--speed"))
    {
      pp = skip_to(1, pp);
      safe_parse_maxint(&pp, &speed);
    }

    // copy default partition types
    for (r = default_part_types, i = 0; *r; r++, i++) part_types[i] = *r;
    if (pp = strstr((char *)m->cmdline, "--pt"))
    {
      pp = skip_to(1, pp);
      // copy partition types specified on the command line
      while (i <= 0x20)
      {
        // find part type end
        for (r = pp; *r && *r != ' ' && *r != ','; r++) ;
        if (safe_parse_maxint(&pp, &t)) part_types[i++] = t;
        if (!*r || *r == ' ') break;
        pp = r + 1;
      }
    }
    part_types[i] = '\0';

    memset(mode, 0, sizeof(mode));
    if (pp0 = strstr((char *)m->cmdline, "--drv="))
    {
      pp0 = skip_to(1, pp0);
      // find name end
      for (r = pp0; *r && *r != ' '; r++) ;
      memmove(mode, pp0, r - pp0);
      mode[r - pp0] = '\0';
      // make name uppercase
      r = mode;
      while (*r) *r++ = toupper(*r);
    }

    memset(mode2, 0, sizeof(mode2));
    if (pp = strstr((char *)m->cmdline, "--drv2="))
    {
      pp = skip_to(1, pp);
      // find name end
      for (r = pp; *r && *r != ' '; r++) ;
      memmove(mode2, pp, r - pp);
      mode2[r - pp] = '\0';
      // make name uppercase
      r = mode2;
      while (*r) *r++ = toupper(*r);
    }
  }

  // init serial port
  serial_init(port, speed, UART_8BITS_WORD, UART_NO_PARITY, UART_1_STOP_BIT);

  kprintf("**** Hello MBI microfsd!\n");
  kprintf("comport = 0x%x\n", port);

  kprintf("part_types=");
  for (r = part_types; *r; r++) kprintf("0x%02x,", *r);
  kprintf("\n");

  kprintf("boot_device=%x\n", m->boot_device);

  p = (unsigned long *)(REL1_BASE + 0x20); // an address of mfs_len in the header

  /* set boot flags */
  boot_flags = BOOTFLAG_MICROFSD | BOOTFLAG_MINIFSD;

  if (m->flags & MB_INFO_BOOTDEV)
  {
    boot_drive = m->boot_device >> 24;
  }
  else
  {
    boot_drive = 0xff;
  }

  // set a drive letter according the DLAT info or AUTO algorithm
  kprintf("assign_drvletter() entered\n");

  if (*mode2) // if '--drv2=...' is defined
  {
    if (!strcmp(mode2, "AUTO"))
    {
      if (!strcmp(mode, "AUTO"))
      {
        drvletter2  = assign_drvletter("AUTO");
        drvletter = drvletter2 + 1;
      }
      else
      {
        drvletter2  = assign_drvletter("AUTO");
        drvletter   = mode[0];
        /* shift letters forth */
        if (drvletter <= drvletter2) drvletter2++;
      }
    }
    else
    {
      drvletter   = assign_drvletter(mode);
      drvletter2  = assign_drvletter(mode2);
    }
  }
  else
    drvletter = assign_drvletter(mode);

  kprintf("assign_drvletter() exited\n");

  // correct the command line according the drive letter got
  pp0[0] = (char)drvletter;
  for (i = 1; i < grub_strlen(mode); i++) pp0[i] = ' '; // pad with spaces
  //
  pp[0] = (char)drvletter2;
  for (i = 1; i < grub_strlen(mode2); i++) pp[i] = ' '; // pad with spaces

  /* Patch the config.sys file with boot drive letter */
  patch_cfgsys();

  // load os2ldr
  if (ufs_open("os2ldr"))
  {
    ldrbase = 0x10000;
    buf = (char *)ldrbase;
    ldrlen = ufs_read(buf, -1);
  }
  else
  {
    printf("os2ldr not found!\n");
    __asm {
      cli
      hlt
    }
  }

  // load os2boot (if it exists)
  if (ufs_open("os2boot"))
  {
    mfsbase = 0x7c0;
    buf = (char *)mfsbase;
    mfslen = ufs_read(buf, -1);
  }

  // read a bootsector. there must be "*bootsec*"
  // string in a command line
  if (ufs_open("*bootsec*"))
  {
    buf = (char *)(REL1_BASE - 0x200);
    ufs_read(buf, 512);
  }

  if (get_diskinfo (boot_drive, &geom)
    || ! (geom.flags & BIOSDISK_FLAG_CDROM))
    cdrom_drive = GRUB_INVALID_DRIVE;
  else
    cdrom_drive = boot_drive;

  /* set filetable */
  ft.ft_ldrseg = ldrbase >> 4;
  ft.ft_ldrlen = ldrlen;

  qq = (unsigned long *)(REL1_BASE + 0x28);
  ufslen = *qq;
  ft.ft_museg  = (REL1_BASE - 0x200 - 0x2000 - 0x1000) >> 4;
  ft.ft_mulen  = ufslen + 0x2000 + 0x1000 + 0x200; // (unsigned long)&stack_bottom - REL1_BASE + 0x200 + 0x2000;

  ft.ft_mfsseg = 0x7c0 >> 4;

  if (!mfslen)
  {
    boot_flags |= BOOTFLAG_NOVOLIO | BOOTFLAG_RIPL;
    ft.ft_cfiles = 4; // 3;
    // where to place mbi pointer
    //q = 0x7c0 + *p - 4;
    //q = REL1_BASE - 0x200 + 0x11; // + 0x2b;
    q = (0x7c0 + *p + 0xf) & 0xfffffff0;

    ft.ft_mfsseg = 0x7c;
    ft.ft_mfslen = *p;
    ft.ft_ripseg = q >> 4; // 0;
    ft.ft_riplen = 4; // 0;

    // pass mbi structure address to mFSD
    // as a variable at its end
    *((unsigned long *)q) = (unsigned long) m;
  }
  else
  {
    ft.ft_cfiles = 3;
    // if alternative os2boot is specified
    ft.ft_mfsseg = 0x7c;
    ft.ft_mfslen = mfslen;
    ft.ft_ripseg = 0;
    ft.ft_riplen = 0;
  }

  p = (unsigned long *)(REL1_BASE + 0x10); // an address of base in the header

  ft.ft_muOpen.seg       = (*p) >> 4;
  ft.ft_muOpen.off       = (unsigned short)(&mu_Open);

  ft.ft_muRead.seg       = (*p) >> 4;
  ft.ft_muRead.off       = (unsigned short)(&mu_Read);

  ft.ft_muClose.seg      = (*p) >> 4;
  ft.ft_muClose.off      = (unsigned short)(&mu_Close);

  ft.ft_muTerminate.seg  = (*p) >> 4;
  ft.ft_muTerminate.off  = (unsigned short)(&mu_Terminate);

  /* set BPB */
  bpb = (bios_parameters_block *)(REL1_BASE - 0x200 + 0xb);

  if (boot_drive == cdrom_drive) { // booting from CDROM drive
    // fill fake BPB
    grub_memset((void *)bpb, 0, sizeof(bios_parameters_block));

    bpb->sect_size  = 0x800;
    bpb->clus_size  = 0x40;
    bpb->n_sect_ext = geom.total_sectors;
    bpb->media_desc = 0xf8;
    bpb->track_size = 0x3f;
    bpb->heads_cnt  = 0xff;
    bpb->marker     = 0x29;
  }

  bpb->disk_num    = (unsigned char)(boot_drive & 0xff);
  bpb->log_drive   = 0x80 + (drvletter - 'C');
  bpb->hidden_secs = offset((m->boot_device >> 16) & 0xff, 1);

  kprintf("boot_flags=0x%x, boot_drive=0x%x\n", boot_flags ,boot_drive);
  kprintf("cfiles=%u\n", ft.ft_cfiles);

  kprintf("mfslen=%lu, mfsseg=0x%x\n", ft.ft_mfslen, ft.ft_mfsseg);
  kprintf("riplen=%lu, ripseg=0x%x\n", ft.ft_riplen, ft.ft_ripseg);
  kprintf("mulen=%lu, museg=0x%x\n", ft.ft_mulen, ft.ft_museg);
  kprintf("ldrlen=%lu, ldrseg=0x%x\n", ft.ft_ldrlen, ft.ft_ldrseg);

  kprintf("mu_Open=0x%x:0x%x\n", ft.ft_muOpen.seg, ft.ft_muOpen.off);
  kprintf("mu_Read=0x%x:0x%x\n", ft.ft_muRead.seg, ft.ft_muRead.off);
  kprintf("mu_Close=0x%x:0x%x\n", ft.ft_muClose.seg, ft.ft_muClose.off);
  kprintf("mu_Terminate=0x%x:0x%x\n", ft.ft_muTerminate.seg, ft.ft_muTerminate.off);

  kprintf("mbi addr=0x%lx\n", m);
/*
  kprintf("bpb dump:\n");

  for (i = 0; i < 0x200 - 0xb; i++)
    kprintf("0x%02x ", *((char *)bpb + i));
 */
  // detect and disable UNDI, if any
  //kprintf("pxe_probe() returned %d", pxe_probe());
}
