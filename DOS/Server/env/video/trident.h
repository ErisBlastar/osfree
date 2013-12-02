/* 
 * (C) Copyright 1992, ..., 2007 the "DOSEMU-Development-Team".
 *
 * for details see file COPYING.DOSEMU in the DOSEMU distribution
 */

extern void vga_init_trident(void);

extern u_char trident_ext_video_port_in(ioport_t port);
extern void trident_ext_video_port_out(ioport_t port, u_char value);

extern void trident_set_bank_read(u_char bank);
extern void trident_set_bank_write(u_char bank);

extern void trident_save_ext_regs(u_char xregs[], u_short xregs16[]);
extern void trident_restore_ext_regs(u_char xregs[], u_short xregs16[]);

extern void trident_set_old_regs(void);
extern void trident_set_new_regs(void);
extern void trident_allow_svga(void);
extern void trident_disallow_svga(void);
extern u_char trident_get_svga(void);


