/*
 * QEMU System Emulator header
 * 
 * Copyright (c) 2003 Fabrice Bellard
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef VL_H
#define VL_H

/* we put basic includes here to avoid repeating them in device drivers */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include "audio/audio.h"

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef _WIN32
#define lseek _lseeki64
/* #define ENOTSUP 4096 */
/* XXX: find 64 bit version */
#define ftruncate chsize

static inline char *realpath(const char *path, char *resolved_path)
{
    _fullpath(resolved_path, path, _MAX_PATH);
    return resolved_path;
}
#endif

#ifdef QEMU_TOOL

/* we use QEMU_TOOL in the command line tools which do not depend on
   the target CPU type */
#include "config-host.h"
#include <setjmp.h>
#include "osdep.h"
#include "bswap.h"

#else

#include "cpu.h"
//#include "gdbstub.h"

#endif /* !defined(QEMU_TOOL) */

#ifndef glue
#define xglue(x, y) x ## y
#define glue(x, y) xglue(x, y)
#define stringify(s)	tostring(s)
#define tostring(s)	#s
#endif

/* vl.c */
uint64_t muldiv64(uint64_t a, uint32_t b, uint32_t c);

void hw_error(const char *fmt, ...);

int get_image_size(const char *filename);
int load_image(const char *filename, uint8_t *addr);
extern const char *bios_dir;

void pstrcpy(char *buf, int buf_size, const char *str);
char *pstrcat(char *buf, int buf_size, const char *s);
int strstart(const char *str, const char *val, const char **ptr);

extern int vm_running;

typedef void VMStopHandler(void *opaque, int reason);

int qemu_add_vm_stop_handler(VMStopHandler *cb, void *opaque);
void qemu_del_vm_stop_handler(VMStopHandler *cb, void *opaque);

void vm_start(void);
void vm_stop(int reason);

typedef void QEMUResetHandler(void *opaque);

void qemu_register_reset(QEMUResetHandler *func, void *opaque);
void qemu_system_reset_request(void);
void qemu_system_shutdown_request(void);
void qemu_system_powerdown_request(void);
#if !defined(TARGET_SPARC)
// Please implement a power failure function to signal the OS
#define qemu_system_powerdown() do{}while(0)
#else
void qemu_system_powerdown(void);
#endif

void main_loop_wait(int timeout);

extern int audio_enabled;
extern int sb16_enabled;
extern int adlib_enabled;
extern int gus_enabled;
extern int ram_size;
extern int bios_size;
extern int rtc_utc;
extern int cirrus_vga_enabled;
extern int graphic_width;
extern int graphic_height;
extern int graphic_depth;
extern const char *keyboard_layout;
extern int kqemu_allowed;
extern int win2k_install_hack;

/* XXX: make it dynamic */
#if defined (TARGET_PPC)
#define BIOS_SIZE ((512 + 32) * 1024)
#elif defined(TARGET_MIPS)
#define BIOS_SIZE (128 * 1024)
#else
#define BIOS_SIZE ((256 + 64) * 1024)
#endif

/* keyboard/mouse support */

#define MOUSE_EVENT_LBUTTON 0x01
#define MOUSE_EVENT_RBUTTON 0x02
#define MOUSE_EVENT_MBUTTON 0x04

typedef void QEMUPutKBDEvent(void *opaque, int keycode);
typedef void QEMUPutMouseEvent(void *opaque, int dx, int dy, int dz, int buttons_state);

void qemu_add_kbd_event_handler(QEMUPutKBDEvent *func, void *opaque);
void qemu_add_mouse_event_handler(QEMUPutMouseEvent *func, void *opaque);

void kbd_put_keycode(int keycode);
void kbd_mouse_event(int dx, int dy, int dz, int buttons_state);

/* keysym is a unicode code except for special keys (see QEMU_KEY_xxx
   constants) */
#define QEMU_KEY_ESC1(c) ((c) | 0xe100)
#define QEMU_KEY_BACKSPACE  0x007f
#define QEMU_KEY_UP         QEMU_KEY_ESC1('A')
#define QEMU_KEY_DOWN       QEMU_KEY_ESC1('B')
#define QEMU_KEY_RIGHT      QEMU_KEY_ESC1('C')
#define QEMU_KEY_LEFT       QEMU_KEY_ESC1('D')
#define QEMU_KEY_HOME       QEMU_KEY_ESC1(1)
#define QEMU_KEY_END        QEMU_KEY_ESC1(4)
#define QEMU_KEY_PAGEUP     QEMU_KEY_ESC1(5)
#define QEMU_KEY_PAGEDOWN   QEMU_KEY_ESC1(6)
#define QEMU_KEY_DELETE     QEMU_KEY_ESC1(3)

#define QEMU_KEY_CTRL_UP         0xe400
#define QEMU_KEY_CTRL_DOWN       0xe401
#define QEMU_KEY_CTRL_LEFT       0xe402
#define QEMU_KEY_CTRL_RIGHT      0xe403
#define QEMU_KEY_CTRL_HOME       0xe404
#define QEMU_KEY_CTRL_END        0xe405
#define QEMU_KEY_CTRL_PAGEUP     0xe406
#define QEMU_KEY_CTRL_PAGEDOWN   0xe407

void kbd_put_keysym(int keysym);

/* async I/O support */

typedef void IOReadHandler(void *opaque, const uint8_t *buf, int size);
typedef int IOCanRWHandler(void *opaque);

int qemu_add_fd_read_handler(int fd, IOCanRWHandler *fd_can_read, 
                             IOReadHandler *fd_read, void *opaque);
void qemu_del_fd_read_handler(int fd);

/* character device */

#define CHR_EVENT_BREAK 0 /* serial break char */
#define CHR_EVENT_FOCUS 1 /* focus to this terminal (modal input needed) */

typedef void IOEventHandler(void *opaque, int event);

typedef struct CharDriverState {
    int (*chr_write)(struct CharDriverState *s, const uint8_t *buf, int len);
    void (*chr_add_read_handler)(struct CharDriverState *s, 
                                 IOCanRWHandler *fd_can_read, 
                                 IOReadHandler *fd_read, void *opaque);
    IOEventHandler *chr_event;
    void (*chr_send_event)(struct CharDriverState *chr, int event);
    void *opaque;
} CharDriverState;

void qemu_chr_printf(CharDriverState *s, const char *fmt, ...);
int qemu_chr_write(CharDriverState *s, const uint8_t *buf, int len);
void qemu_chr_send_event(CharDriverState *s, int event);
void qemu_chr_add_read_handler(CharDriverState *s, 
                               IOCanRWHandler *fd_can_read, 
                               IOReadHandler *fd_read, void *opaque);
void qemu_chr_add_event_handler(CharDriverState *s, IOEventHandler *chr_event);
                               
/* consoles */

typedef struct DisplayState DisplayState;
typedef struct TextConsole TextConsole;

extern TextConsole *vga_console;

TextConsole *graphic_console_init(DisplayState *ds);
int is_active_console(TextConsole *s);
CharDriverState *text_console_init(DisplayState *ds);
void console_select(unsigned int index);

/* serial ports */

#define MAX_SERIAL_PORTS 4

extern CharDriverState *serial_hds[MAX_SERIAL_PORTS];

/* parallel ports */

#define MAX_PARALLEL_PORTS 3

extern CharDriverState *parallel_hds[MAX_PARALLEL_PORTS];

/* network redirectors support */

#define MAX_NICS 8

typedef struct NetDriverState {
    int index; /* index number in QEMU */
    uint8_t macaddr[6];
    char ifname[16];
    void (*send_packet)(struct NetDriverState *nd, 
                        const uint8_t *buf, int size);
    void (*add_read_packet)(struct NetDriverState *nd, 
                            IOCanRWHandler *fd_can_read, 
                            IOReadHandler *fd_read, void *opaque);
    /* tun specific data */
    int fd;
    /* slirp specific data */
} NetDriverState;

extern int nb_nics;
extern NetDriverState nd_table[MAX_NICS];

void qemu_send_packet(NetDriverState *nd, const uint8_t *buf, int size);
void qemu_add_read_packet(NetDriverState *nd, IOCanRWHandler *fd_can_read, 
                          IOReadHandler *fd_read, void *opaque);

/* timers */

typedef struct QEMUClock QEMUClock;
typedef struct QEMUTimer QEMUTimer;
typedef void QEMUTimerCB(void *opaque);

/* The real time clock should be used only for stuff which does not
   change the virtual machine state, as it is run even if the virtual
   machine is stopped. The real time clock has a frequency of 1000
   Hz. */
extern QEMUClock *rt_clock;

/* The virtual clock is only run during the emulation. It is stopped
   when the virtual machine is stopped. Virtual timers use a high
   precision clock, usually cpu cycles (use ticks_per_sec). */
extern QEMUClock *vm_clock;

int64_t qemu_get_clock(QEMUClock *clock);

QEMUTimer *qemu_new_timer(QEMUClock *clock, QEMUTimerCB *cb, void *opaque);
void qemu_free_timer(QEMUTimer *ts);
void qemu_del_timer(QEMUTimer *ts);
void qemu_mod_timer(QEMUTimer *ts, int64_t expire_time);
int qemu_timer_pending(QEMUTimer *ts);

extern int64_t ticks_per_sec;
extern int pit_min_timer_count;

void cpu_enable_ticks(void);
void cpu_disable_ticks(void);

/* VM Load/Save */

typedef FILE QEMUFile;

void qemu_put_buffer(QEMUFile *f, const uint8_t *buf, int size);
void qemu_put_byte(QEMUFile *f, int v);
void qemu_put_be16(QEMUFile *f, unsigned int v);
void qemu_put_be32(QEMUFile *f, unsigned int v);
void qemu_put_be64(QEMUFile *f, uint64_t v);
int qemu_get_buffer(QEMUFile *f, uint8_t *buf, int size);
int qemu_get_byte(QEMUFile *f);
unsigned int qemu_get_be16(QEMUFile *f);
unsigned int qemu_get_be32(QEMUFile *f);
uint64_t qemu_get_be64(QEMUFile *f);

static inline void qemu_put_be64s(QEMUFile *f, const uint64_t *pv)
{
    qemu_put_be64(f, *pv);
}

static inline void qemu_put_be32s(QEMUFile *f, const uint32_t *pv)
{
    qemu_put_be32(f, *pv);
}

static inline void qemu_put_be16s(QEMUFile *f, const uint16_t *pv)
{
    qemu_put_be16(f, *pv);
}

static inline void qemu_put_8s(QEMUFile *f, const uint8_t *pv)
{
    qemu_put_byte(f, *pv);
}

static inline void qemu_get_be64s(QEMUFile *f, uint64_t *pv)
{
    *pv = qemu_get_be64(f);
}

static inline void qemu_get_be32s(QEMUFile *f, uint32_t *pv)
{
    *pv = qemu_get_be32(f);
}

static inline void qemu_get_be16s(QEMUFile *f, uint16_t *pv)
{
    *pv = qemu_get_be16(f);
}

static inline void qemu_get_8s(QEMUFile *f, uint8_t *pv)
{
    *pv = qemu_get_byte(f);
}

#if TARGET_LONG_BITS == 64
#define qemu_put_betl qemu_put_be64
#define qemu_get_betl qemu_get_be64
#define qemu_put_betls qemu_put_be64s
#define qemu_get_betls qemu_get_be64s
#else
#define qemu_put_betl qemu_put_be32
#define qemu_get_betl qemu_get_be32
#define qemu_put_betls qemu_put_be32s
#define qemu_get_betls qemu_get_be32s
#endif

int64_t qemu_ftell(QEMUFile *f);
int64_t qemu_fseek(QEMUFile *f, int64_t pos, int whence);

typedef void SaveStateHandler(QEMUFile *f, void *opaque);
typedef int LoadStateHandler(QEMUFile *f, void *opaque, int version_id);

int qemu_loadvm(const char *filename);
int qemu_savevm(const char *filename);
int register_savevm(const char *idstr, 
                    int instance_id, 
                    int version_id,
                    SaveStateHandler *save_state,
                    LoadStateHandler *load_state,
                    void *opaque);
void qemu_get_timer(QEMUFile *f, QEMUTimer *ts);
void qemu_put_timer(QEMUFile *f, QEMUTimer *ts);

/* block.c */
typedef struct BlockDriverState BlockDriverState;
typedef struct BlockDriver BlockDriver;

extern BlockDriver bdrv_raw;
extern BlockDriver bdrv_cow;
extern BlockDriver bdrv_qcow;
extern BlockDriver bdrv_vmdk;
extern BlockDriver bdrv_cloop;
extern BlockDriver bdrv_dmg;
extern BlockDriver bdrv_bochs;
extern BlockDriver bdrv_vpc;
extern BlockDriver bdrv_vvfat;

void bdrv_init(void);
BlockDriver *bdrv_find_format(const char *format_name);
int bdrv_create(BlockDriver *drv, 
                const char *filename, int64_t size_in_sectors,
                const char *backing_file, int flags);
BlockDriverState *bdrv_new(const char *device_name);
void bdrv_delete(BlockDriverState *bs);
int bdrv_open(BlockDriverState *bs, const char *filename, int snapshot);
int bdrv_open2(BlockDriverState *bs, const char *filename, int snapshot,
               BlockDriver *drv);
void bdrv_close(BlockDriverState *bs);
int bdrv_read(BlockDriverState *bs, int64_t sector_num, 
              uint8_t *buf, int nb_sectors);
int bdrv_write(BlockDriverState *bs, int64_t sector_num, 
               const uint8_t *buf, int nb_sectors);
void bdrv_get_geometry(BlockDriverState *bs, int64_t *nb_sectors_ptr);
int bdrv_commit(BlockDriverState *bs);
void bdrv_set_boot_sector(BlockDriverState *bs, const uint8_t *data, int size);

#define BDRV_TYPE_HD     0
#define BDRV_TYPE_CDROM  1
#define BDRV_TYPE_FLOPPY 2
#define BIOS_ATA_TRANSLATION_AUTO 0
#define BIOS_ATA_TRANSLATION_NONE 1
#define BIOS_ATA_TRANSLATION_LBA  2

void bdrv_set_geometry_hint(BlockDriverState *bs, 
                            int cyls, int heads, int secs);
void bdrv_set_type_hint(BlockDriverState *bs, int type);
void bdrv_set_translation_hint(BlockDriverState *bs, int translation);
void bdrv_get_geometry_hint(BlockDriverState *bs, 
                            int *pcyls, int *pheads, int *psecs);
int bdrv_get_type_hint(BlockDriverState *bs);
int bdrv_get_translation_hint(BlockDriverState *bs);
int bdrv_is_removable(BlockDriverState *bs);
int bdrv_is_read_only(BlockDriverState *bs);
int bdrv_is_inserted(BlockDriverState *bs);
int bdrv_is_locked(BlockDriverState *bs);
void bdrv_set_locked(BlockDriverState *bs, int locked);
void bdrv_set_change_cb(BlockDriverState *bs, 
                        void (*change_cb)(void *opaque), void *opaque);
void bdrv_get_format(BlockDriverState *bs, char *buf, int buf_size);
void bdrv_info(void);
BlockDriverState *bdrv_find(const char *name);
void bdrv_iterate(void (*it)(void *opaque, const char *name), void *opaque);
int bdrv_is_encrypted(BlockDriverState *bs);
int bdrv_set_key(BlockDriverState *bs, const char *key);
void bdrv_iterate_format(void (*it)(void *opaque, const char *name), 
                         void *opaque);
const char *bdrv_get_device_name(BlockDriverState *bs);

int qcow_get_cluster_size(BlockDriverState *bs);
int qcow_compress_cluster(BlockDriverState *bs, int64_t sector_num,
                          const uint8_t *buf);

#ifndef QEMU_TOOL

typedef void QEMUMachineInitFunc(int ram_size, int vga_ram_size, 
                                 int boot_device,
             DisplayState *ds, const char **fd_filename, int snapshot,
             const char *kernel_filename, const char *kernel_cmdline,
             const char *initrd_filename);

typedef struct QEMUMachine {
    const char *name;
    const char *desc;
    QEMUMachineInitFunc *init;
    struct QEMUMachine *next;
} QEMUMachine;

int qemu_register_machine(QEMUMachine *m);

typedef void SetIRQFunc(void *opaque, int irq_num, int level);
typedef void IRQRequestFunc(void *opaque, int level);

/* ISA bus */

extern target_phys_addr_t isa_mem_base;

typedef void (IOPortWriteFunc)(void *opaque, uint32_t address, uint32_t data);
typedef uint32_t (IOPortReadFunc)(void *opaque, uint32_t address);

int register_ioport_read(int start, int length, int size, 
                         IOPortReadFunc *func, void *opaque);
int register_ioport_write(int start, int length, int size, 
                          IOPortWriteFunc *func, void *opaque);
void isa_unassign_ioport(int start, int length);

/* PCI bus */

extern int pci_enabled;

extern target_phys_addr_t pci_mem_base;

typedef struct PCIBus PCIBus;
typedef struct PCIDevice PCIDevice;

typedef void PCIConfigWriteFunc(PCIDevice *pci_dev, 
                                uint32_t address, uint32_t data, int len);
typedef uint32_t PCIConfigReadFunc(PCIDevice *pci_dev, 
                                   uint32_t address, int len);
typedef void PCIMapIORegionFunc(PCIDevice *pci_dev, int region_num, 
                                uint32_t addr, uint32_t size, int type);

#define PCI_ADDRESS_SPACE_MEM		0x00
#define PCI_ADDRESS_SPACE_IO		0x01
#define PCI_ADDRESS_SPACE_MEM_PREFETCH	0x08

typedef struct PCIIORegion {
    uint32_t addr; /* current PCI mapping address. -1 means not mapped */
    uint32_t size;
    uint8_t type;
    PCIMapIORegionFunc *map_func;
} PCIIORegion;

#define PCI_ROM_SLOT 6
#define PCI_NUM_REGIONS 7
struct PCIDevice {
    /* PCI config space */
    uint8_t config[256];

    /* the following fields are read only */
    PCIBus *bus;
    int devfn;
    char name[64];
    PCIIORegion io_regions[PCI_NUM_REGIONS];
    
    /* do not access the following fields */
    PCIConfigReadFunc *config_read;
    PCIConfigWriteFunc *config_write;
    int irq_index;
};

PCIDevice *pci_register_device(PCIBus *bus, const char *name,
                               int instance_size, int devfn,
                               PCIConfigReadFunc *config_read, 
                               PCIConfigWriteFunc *config_write);

void pci_register_io_region(PCIDevice *pci_dev, int region_num, 
                            uint32_t size, int type, 
                            PCIMapIORegionFunc *map_func);

void pci_set_irq(PCIDevice *pci_dev, int irq_num, int level);

uint32_t pci_default_read_config(PCIDevice *d, 
                                 uint32_t address, int len);
void pci_default_write_config(PCIDevice *d, 
                              uint32_t address, uint32_t val, int len);
void generic_pci_save(QEMUFile* f, void *opaque);
int generic_pci_load(QEMUFile* f, void *opaque, int version_id);

extern struct PIIX3State *piix3_state;

PCIBus *i440fx_init(void);
void piix3_init(PCIBus *bus);
void pci_bios_init(void);
void pci_info(void);

/* temporary: will be moved in platform specific file */
void pci_set_pic(PCIBus *bus, SetIRQFunc *set_irq, void *irq_opaque);
PCIBus *pci_prep_init(void);
PCIBus *pci_grackle_init(uint32_t base);
PCIBus *pci_pmac_init(void);
PCIBus *pci_apb_init(target_ulong special_base, target_ulong mem_base);

/* openpic.c */
typedef struct openpic_t openpic_t;
void openpic_set_irq(void *opaque, int n_IRQ, int level);
openpic_t *openpic_init (PCIBus *bus, int *pmem_index, int nb_cpus);

/* heathrow_pic.c */
typedef struct HeathrowPICS HeathrowPICS;
void heathrow_pic_set_irq(void *opaque, int num, int level);
HeathrowPICS *heathrow_pic_init(int *pmem_index);

/* vga.c */

#define VGA_RAM_SIZE (4096 * 1024)

struct DisplayState {
    uint8_t *data;
    int linesize;
    int depth;
    int width;
    int height;
    void (*dpy_update)(struct DisplayState *s, int x, int y, int w, int h);
    void (*dpy_resize)(struct DisplayState *s, int w, int h);
    void (*dpy_refresh)(struct DisplayState *s);
};

static inline void dpy_update(DisplayState *s, int x, int y, int w, int h)
{
    s->dpy_update(s, x, y, w, h);
}

static inline void dpy_resize(DisplayState *s, int w, int h)
{
    s->dpy_resize(s, w, h);
}

int vga_initialize(PCIBus *bus, DisplayState *ds, uint8_t *vga_ram_base, 
                   unsigned long vga_ram_offset, int vga_ram_size,
                   unsigned long vga_bios_offset, int vga_bios_size);
void vga_update_display(void);
void vga_invalidate_display(void);
void vga_screen_dump(const char *filename);

/* cirrus_vga.c */
void pci_cirrus_vga_init(PCIBus *bus, DisplayState *ds, uint8_t *vga_ram_base, 
                         unsigned long vga_ram_offset, int vga_ram_size);
void isa_cirrus_vga_init(DisplayState *ds, uint8_t *vga_ram_base, 
                         unsigned long vga_ram_offset, int vga_ram_size);

/* sdl.c */
void sdl_display_init(DisplayState *ds, int full_screen);

/* cocoa.m */
void cocoa_display_init(DisplayState *ds, int full_screen);

/* ide.c */
#define MAX_DISKS 4

extern BlockDriverState *bs_table[MAX_DISKS];

void isa_ide_init(int iobase, int iobase2, int irq,
                  BlockDriverState *hd0, BlockDriverState *hd1);
void pci_cmd646_ide_init(PCIBus *bus, BlockDriverState **hd_table,
                         int secondary_ide_enabled);
void pci_piix3_ide_init(PCIBus *bus, BlockDriverState **hd_table);
int pmac_ide_init (BlockDriverState **hd_table,
                   SetIRQFunc *set_irq, void *irq_opaque, int irq);

/* sb16.c */
void SB16_init (void);

/* adlib.c */
void Adlib_init (void);

/* gus.c */
void GUS_init (void);

/* dma.c */
typedef int (*DMA_transfer_handler) (void *opaque, int nchan, int pos, int size);
int DMA_get_channel_mode (int nchan);
int DMA_read_memory (int nchan, void *buf, int pos, int size);
int DMA_write_memory (int nchan, void *buf, int pos, int size);
void DMA_hold_DREQ (int nchan);
void DMA_release_DREQ (int nchan);
void DMA_schedule(int nchan);
void DMA_run (void);
void DMA_init (int high_page_enable);
void DMA_register_channel (int nchan,
                           DMA_transfer_handler transfer_handler,
                           void *opaque);
/* fdc.c */
#define MAX_FD 2
extern BlockDriverState *fd_table[MAX_FD];

typedef struct fdctrl_t fdctrl_t;

fdctrl_t *fdctrl_init (int irq_lvl, int dma_chann, int mem_mapped, 
                       uint32_t io_base,
                       BlockDriverState **fds);
int fdctrl_get_drive_type(fdctrl_t *fdctrl, int drive_num);

/* ne2000.c */

void isa_ne2000_init(int base, int irq, NetDriverState *nd);
void pci_ne2000_init(PCIBus *bus, NetDriverState *nd);

/* pckbd.c */

void kbd_init(void);

/* mc146818rtc.c */

typedef struct RTCState RTCState;

RTCState *rtc_init(int base, int irq);
void rtc_set_memory(RTCState *s, int addr, int val);
void rtc_set_date(RTCState *s, const struct tm *tm);

/* serial.c */

typedef struct SerialState SerialState;
SerialState *serial_init(int base, int irq, CharDriverState *chr);

/* parallel.c */

typedef struct ParallelState ParallelState;
ParallelState *parallel_init(int base, int irq, CharDriverState *chr);

/* i8259.c */

typedef struct PicState2 PicState2;
extern PicState2 *isa_pic;
void pic_set_irq(int irq, int level);
void pic_set_irq_new(void *opaque, int irq, int level);
PicState2 *pic_init(IRQRequestFunc *irq_request, void *irq_request_opaque);
void pic_set_alt_irq_func(PicState2 *s, SetIRQFunc *alt_irq_func,
                          void *alt_irq_opaque);
int pic_read_irq(PicState2 *s);
void pic_update_irq(PicState2 *s);
uint32_t pic_intack_read(PicState2 *s);
void pic_info(void);
void irq_info(void);

/* APIC */
typedef struct IOAPICState IOAPICState;

int apic_init(CPUState *env);
int apic_get_interrupt(CPUState *env);
IOAPICState *ioapic_init(void);
void ioapic_set_irq(void *opaque, int vector, int level);

/* i8254.c */

#define PIT_FREQ 1193182

typedef struct PITState PITState;

PITState *pit_init(int base, int irq);
void pit_set_gate(PITState *pit, int channel, int val);
int pit_get_gate(PITState *pit, int channel);
int pit_get_out(PITState *pit, int channel, int64_t current_time);

/* pc.c */
extern QEMUMachine pc_machine;

/* ppc.c */
extern QEMUMachine prep_machine;
extern QEMUMachine core99_machine;
extern QEMUMachine heathrow_machine;

/* mips_r4k.c */
extern QEMUMachine mips_machine;

#ifdef TARGET_PPC
ppc_tb_t *cpu_ppc_tb_init (CPUState *env, uint32_t freq);
#endif
void PREP_debug_write (void *opaque, uint32_t addr, uint32_t val);

extern CPUWriteMemoryFunc *PPC_io_write[];
extern CPUReadMemoryFunc *PPC_io_read[];
extern int prep_enabled;
void PPC_debug_write (void *opaque, uint32_t addr, uint32_t val);

/* sun4m.c */
extern QEMUMachine sun4m_machine;
uint32_t iommu_translate(uint32_t addr);

/* iommu.c */
void *iommu_init(uint32_t addr);
uint32_t iommu_translate_local(void *opaque, uint32_t addr);

/* lance.c */
void lance_init(NetDriverState *nd, int irq, uint32_t leaddr, uint32_t ledaddr);

/* tcx.c */
void *tcx_init(DisplayState *ds, uint32_t addr, uint8_t *vram_base,
	       unsigned long vram_offset, int vram_size, int width, int height);
void tcx_update_display(void *opaque);
void tcx_invalidate_display(void *opaque);
void tcx_screen_dump(void *opaque, const char *filename);

/* slavio_intctl.c */
void *slavio_intctl_init();
void slavio_pic_info(void *opaque);
void slavio_irq_info(void *opaque);
void slavio_pic_set_irq(void *opaque, int irq, int level);

/* magic-load.c */
int load_elf(const char *filename, uint8_t *addr);
int load_aout(const char *filename, uint8_t *addr);

/* slavio_timer.c */
void slavio_timer_init(uint32_t addr1, int irq1, uint32_t addr2, int irq2);

/* slavio_serial.c */
SerialState *slavio_serial_init(int base, int irq, CharDriverState *chr1, CharDriverState *chr2);
void slavio_serial_ms_kbd_init(int base, int irq);

/* slavio_misc.c */
void *slavio_misc_init(uint32_t base, int irq);
void slavio_set_power_fail(void *opaque, int power_failing);

/* esp.c */
void esp_init(BlockDriverState **bd, int irq, uint32_t espaddr, uint32_t espdaddr);

/* sun4u.c */
extern QEMUMachine sun4u_machine;

/* NVRAM helpers */
#include "hw/m48t59.h"

void NVRAM_set_byte (m48t59_t *nvram, uint32_t addr, uint8_t value);
uint8_t NVRAM_get_byte (m48t59_t *nvram, uint32_t addr);
void NVRAM_set_word (m48t59_t *nvram, uint32_t addr, uint16_t value);
uint16_t NVRAM_get_word (m48t59_t *nvram, uint32_t addr);
void NVRAM_set_lword (m48t59_t *nvram, uint32_t addr, uint32_t value);
uint32_t NVRAM_get_lword (m48t59_t *nvram, uint32_t addr);
void NVRAM_set_string (m48t59_t *nvram, uint32_t addr,
                       const unsigned char *str, uint32_t max);
int NVRAM_get_string (m48t59_t *nvram, uint8_t *dst, uint16_t addr, int max);
void NVRAM_set_crc (m48t59_t *nvram, uint32_t addr,
                    uint32_t start, uint32_t count);
int PPC_NVRAM_set_params (m48t59_t *nvram, uint16_t NVRAM_size,
                          const unsigned char *arch,
                          uint32_t RAM_size, int boot_device,
                          uint32_t kernel_image, uint32_t kernel_size,
                          const char *cmdline,
                          uint32_t initrd_image, uint32_t initrd_size,
                          uint32_t NVRAM_image,
                          int width, int height, int depth);

/* adb.c */

#define MAX_ADB_DEVICES 16

#define ADB_MAX_OUT_LEN 16

typedef struct ADBDevice ADBDevice;

/* buf = NULL means polling */
typedef int ADBDeviceRequest(ADBDevice *d, uint8_t *buf_out,
                              const uint8_t *buf, int len);
typedef int ADBDeviceReset(ADBDevice *d);

struct ADBDevice {
    struct ADBBusState *bus;
    int devaddr;
    int handler;
    ADBDeviceRequest *devreq;
    ADBDeviceReset *devreset;
    void *opaque;
};

typedef struct ADBBusState {
    ADBDevice devices[MAX_ADB_DEVICES];
    int nb_devices;
    int poll_index;
} ADBBusState;

int adb_request(ADBBusState *s, uint8_t *buf_out,
                const uint8_t *buf, int len);
int adb_poll(ADBBusState *s, uint8_t *buf_out);

ADBDevice *adb_register_device(ADBBusState *s, int devaddr, 
                               ADBDeviceRequest *devreq, 
                               ADBDeviceReset *devreset, 
                               void *opaque);
void adb_kbd_init(ADBBusState *bus);
void adb_mouse_init(ADBBusState *bus);

/* cuda.c */

extern ADBBusState adb_bus;
int cuda_init(SetIRQFunc *set_irq, void *irq_opaque, int irq);

#endif /* defined(QEMU_TOOL) */

/* monitor.c */
void monitor_init(CharDriverState *hd, int show_banner);
void term_puts(const char *str);
void term_vprintf(const char *fmt, va_list ap);
#ifndef __WATCOMC__
void term_printf(const char *fmt, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
#else
void term_printf(const char *fmt, ...);
#endif
void term_flush(void);
void term_print_help(void);
void monitor_readline(const char *prompt, int is_password,
                      char *buf, int buf_size);

/* readline.c */
typedef void ReadLineFunc(void *opaque, const char *str);

extern int completion_index;
void add_completion(const char *str);
void readline_handle_byte(int ch);
void readline_find_completion(const char *cmdline);
const char *readline_get_history(unsigned int index);
void readline_start(const char *prompt, int is_password,
                    ReadLineFunc *readline_func, void *opaque);

void kqemu_record_dump(void);

#endif /* VL_H */
