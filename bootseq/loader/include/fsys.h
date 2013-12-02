/*
 *   fsys.h
 */

#include <lip.h>
#include "term.h"

#pragma aux l1       "*"
#pragma aux l2       "*"
#pragma aux errnum   "*"

#ifndef STAGE1_5

#pragma aux printmsg "*"
#pragma aux printd   "*"
#pragma aux printw   "*"
#pragma aux printb   "*"
#pragma aux panic    "*"
#pragma aux reloc    "*"

void printmsg(char *);
void printb(unsigned char);
void printw(unsigned short);
void printd(unsigned long);

void reloc(char *base, char *rel_file, unsigned long shift);
void panic(char *msg, char *file);
extern unsigned int relshift;

#endif

typedef void (*disk_read_hook_t) (int, int, int);
extern  disk_read_hook_t *pdisk_read_hook;
extern  disk_read_hook_t *pdisk_read_func;

void *grub_memset (void *addr, int c, int n);
void *grub_memmove (void *dest, const void *src, int count);
void *grub_memcpy (void *dest, const void *src, int count);
char *grub_strcpy (char *dest, const char *src);
int grub_strcmp (const char *s1, const char *s2);
int grub_memcmp (const char *a1, const char *a2, int count);
int grub_strlen (const char *s);
int grub_isspace (int c);
int grub_tolower (int c);
void grub_printf (const char *format,...);
char *ltoa(int n, char *buf, int radix);
int substring (const char *s1, const char *s2);
int grub_pos (const char c, const char *s);
int devread (int sector, int byte_offset, int byte_len, char *buf);
int rawread (int drive, int sector, int byte_offset, int byte_len, char *buf);

#define printf grub_printf

unsigned int __cdecl
u_open (char *name, unsigned int *size);
int __cdecl
u_dir  (char *dirname);
unsigned int __cdecl
u_read (char *buf, unsigned int count);
unsigned int __cdecl
u_seek (int loffseek);
void __cdecl
u_close (void);
void __cdecl
u_terminate (void);
int __cdecl
u_diskctl (int func, int drive, struct geometry *geometry, int sector, int nsec, int addr);
int __cdecl
u_vbectl(int func, int mode_number, void *info);
struct term_entry * __cdecl
u_termctl(int termno);
int __cdecl
u_boot (int type);
int __cdecl
u_load (char *image, unsigned int size, char *load_addr, struct exe_params *p);
int __cdecl
u_parm (int parm, int action, unsigned int *val);
void __cdecl
u_msg (char *s);
void __cdecl
u_setlip (lip2_t *l);

vbe_mode_set(struct vbe_controller *controller, int mode_number,
             struct vbe_mode *mode, unsigned int *pmif_segoff,
             unsigned int *pmif_len);

/* VBE control functions */
#define VBE_FUNC_RESET            0
#define VBE_FUNC_GET_CTRLR_INFO   1
#define VBE_FUNC_GET_MODE_INFO    2
#define VBE_FUNC_SET_MODE         3
#define VBE_FUNC_GET_PMIF         4

struct pmif
{
  unsigned int *pmif_segoff;
  unsigned int *pmif_len;
};

/* Actions */
#define ACT_GET 0
#define ACT_SET 1

/* Parameter codes */
#define PARM_BOOT_DRIVE            0
#define PARM_CDROM_DRIVE           1
#define PARM_CURRENT_DRIVE         2
#define PARM_CURRENT_PARTITION     3
#define PARM_CURRENT_SLICE         4
#define PARM_SAVED_DRIVE           5
#define PARM_SAVED_PARTITION       6
#define PARM_SAVED_SLICE           7
#define PARM_PART_START            8
#define PARM_PART_LENGTH           9
#define PARM_MBI                  10
#define PARM_ERRNUM               11
#define PARM_FILEPOS              12
#define PARM_FILEMAX              13
#define PARM_EXTENDED_MEMORY      14
#define PARM_PREFIX               15
#define PARM_LDRLEN               16
#define PARM_BUF_DRIVE            17
#define PARM_AT_DRIVE             18
#define PARM_DISK_READ_HOOK       19
#define PARM_DISK_READ_FUNC       20
#define PARM_SECTSIZE             21

#pragma aux u_open   "*"
#pragma aux u_dir    "*"
#pragma aux u_read   "*"
#pragma aux u_seek   "*"
#pragma aux u_close  "*"
#pragma aux u_terminate "*"
#pragma aux u_diskctl   "*"
#pragma aux u_vbectl    "*"
#pragma aux u_boot   "*"
#pragma aux u_load   "*"
#pragma aux u_parm   "*"
#pragma aux u_msg    "*"
#pragma aux u_setlip "*"

extern struct multiboot_info *m; // pointer to the multiboot structure

extern char *preldr_path; // preldr files path
extern char *fsd_dir;     // uFSD's dir
extern char *cfg_file;    // preldr config file name
extern char *fsys_list[]; // file systems list
extern int  num_fsys;

extern lip1_t *l1;

void panic(char *msg, char *file);

#pragma aux mu_Open           "*"
#pragma aux mu_Read           "*"
#pragma aux mu_Close          "*"
#pragma aux mu_Terminate      "*"

#pragma aux ft                "*"
#pragma aux boot_drive        "*"
#pragma aux boot_flags        "*"
#pragma aux install_partition "*"

#pragma aux printmsg "*"
#pragma aux printb   "*"
#pragma aux printw   "*"
#pragma aux printd   "*"

#pragma aux filemax    "*"
#pragma aux grub_open  "*"
#pragma aux grub_read  "*"
#pragma aux grub_seek  "*"
#pragma aux grub_close "*"
#pragma aux freeldr_open  "*"
#pragma aux freeldr_dir   "*"
#pragma aux freeldr_read  "*"
#ifndef STAGE1_5
#pragma aux freeldr_seek  "*"
#endif
#pragma aux freeldr_close "*"

int  stage0_mount (void);
int  stage0_read (char *buf, int len);
int  stage0_dir (char *dirname);
void stage0_close(void);
int  stage0_embed(int *start_sector, int needed_sectors);

int  freeldr_open (char *filename);
int  freeldr_dir  (char *dirname);
int  freeldr_read (char *buf, int len);
#ifndef STAGE1_5
int  freeldr_seek (int offset);
#endif
void freeldr_close (void);

int  grub_open (char *filename);
int  grub_read (char *buf, int len);
int  grub_seek (int offset);
void grub_close (void);

extern int    (*pdevread) (int sector, int byte_offset, int byte_len, char *buf);
extern int    (*prawread) (int drive, int sector, int byte_offset, int byte_len, char *buf);
extern int    (*psubstring)    (const char *s1, const char *s2);
extern int    (*pgrub_memcmp)  (const char *s1, const char *s2, int n);
extern void * (*pgrub_memmove) (void *_to, const void *_from, int _len);
extern void * (*pgrub_memset)  (void *start, char c, int len);
extern int    (*pgrub_strcmp)  (const char *, const char *);
extern int    (*pgrub_isspace) (int c);
extern int    (*pgrub_tolower) (int c);
extern int    (*pgrub_read)    (char *buf, int len);

int mkroot (char *arg);

extern int *pmem_lower; // = 639;
extern int *pmem_upper; // = 16384;

/* filesystem common variables */
extern int *pfilepos;
extern int *pfilemax;

/* disk buffer parameters */
extern int             *pbuf_drive; // = -1;
extern int             *pbuf_track;
extern struct geometry *pbuf_geom;

extern grub_error_t    *perrnum;
//int print_possibilities;

extern unsigned long *psaved_drive;
extern unsigned long *psaved_partition;

extern unsigned long *pcurrent_drive;
extern unsigned long *pcurrent_partition;
extern int           *pcurrent_slice;
extern unsigned long *ppart_start;
extern unsigned long *ppart_length;
extern int           *pfsmax;

extern int           *pprint_possibilities;
extern void          (*pprint_a_completion) (char *);
extern void          (*pprintf) (const char *format,...);


//extern int      debug = 0;
extern struct geometry *pbuf_geom;
