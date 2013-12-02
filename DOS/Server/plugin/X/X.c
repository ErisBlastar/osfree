/* 
 * (C) Copyright 1992, ..., 2007 the "DOSEMU-Development-Team".
 *
 * for details see file COPYING.DOSEMU in the DOSEMU distribution
 */

/* 
 * DANG_BEGIN_MODULE
 *
 * REMARK
 * This module contains the video interface for the X Window 
 * System. It has mouse and selection 'cut' support.
 *
 * /REMARK
 * DANG_END_MODULE
 *
 * DANG_BEGIN_CHANGELOG
 *
 * $Id: X.c 1769 2007-05-04 05:59:48Z bartoldeman $
 *
 * 951205: bon@elektron.ikp.physik.th-darmstadt.de
 *   Merged in again the selection stuff. Hope introduce hidden bugs 
 *        are removed!  
 *
 *         TODO: highlighting should be smarter and either discard area
 *               or follow ir when scrolling (text-mode
 *
 * 5/24/95, Started to get the graphics modes to work
 * Erik Mouw (J.A.K.Mouw@et.tudelft.nl) 
 * and Arjan Filius (I.A.Filius@et.tudelft.nl)
 *
 * changed:
 *  X_setmode()
 *  X_redraw_screen()
 *  X_update_screen()
 *  set_mouse_position()
 * in video/X.c
 *
 * and:
 *  set_video_mode()
 * in video/int10.c
 * int10()
 *
 * added:
 *  graphics_cmap_init()
 *
 * Now dosemu can work in graphics mode with X, but at the moment only in mode
 * 0x13 and very slow.
 *
 * 1995-08-30: Merged with code for pasting, removed some restrictions 
 * from Pasi's code: cut and paste and x_mouse, 
 * (bon@elektron.ikp.physik.th-darmstadt.de)
 *
 * 1995-08-22: Lots of cleaning and rewriting, more comments, MappingNotify 
 * event support, selection support (cut only), fixed some cursor redrawing 
 * bugs. -- Pasi Eronen (pe@iki.fi)
 *
 *
 * 1996/04/29: Added the ability to use MIT-SHM X extensions to speed
 * up graphics handling... compile-time option.  Typically halves system
 * overhead for local graphics. -- Adam D. Moss (aspirin@tigerden.com) (adm)
 *
 * 1996/05/03: Graphics are MUCH faster now due to changes in vgaemu.c. --adm
 *
 * 1996/05/05: Re-architected the process of delivering changes from
 * vgaemu.c to the client (X.c).  This will allow further optimization
 * in the future (I hope). --adm
 *
 * 1996/05/06: Began work on shared-colourmap and mismatched-colourdepth
 * graphics support. --adm
 *
 * 1996/05/20: More work on colourmap, bugfixes, speedups, and better
 * mouse support in graphics modes. --adm
 *
 * 1996/08/18: Make xdos work with TrueColor displays (again?) -- Thomas Pundt
 * (pundtt@math.uni-muenster.de)
 *
 * 1997/01/19: Made mouse functionable for win31-in-xdos (hiding X-cursor,
 * calibrating win31 cursor) -- Hans
 *
 * 1997/02/03: Adapted Steffen Winterfeld's (graphics) true color patch from
 * 0.63.1.98 to 0.64.3.2.
 * The code now looks a bit kludgy, because it was written for for the .98,
 * but it works. Scaling x2 is only set for 320x200.  -- Hans
 *
 * 1997/03/03: Added code to turn off MIT-SHM for network connections.
 * -- sw (Steffen Winterfeldt <wfeldt@suse.de>)
 *
 * 1997/04/21: Changed the interface to vgaemu - basically we do
 * all image conversions in remap.c now. Added & enhanced support
 * for 8/15/16/24/32 bit displays (you can resize the graphics
 * window now).
 * Palette updates in text-modes work; the code is not
 * perfect yet (no screen update is done), so the change will
 * take effect only after some time...
 * -- sw (Steffen Winterfeldt <wfeldt@suse.de>)
 *
 * 1997/06/06: Fixed the code that was supposed to turn off MIT-SHM for
 * network connections. Thanks to Leonid V. Kalmankin <leonid@cs.msu.su>
 * for finding the bug and testing the fix.
 * -- sw (Steffen Winterfeldt <wfeldt@suse.de>)
 *              
 * 1997/06/15: Added gamma correction for graphics modes.
 * -- sw
 *
 * 1998/02/22: Rework of mouse support in graphics modes; put in some
 * code to enable mouse grabbing. Can be activated via Ctrl+Alt+<some configurable key>;
 * default is "Home".
 * -- sw
 *
 * 1998/03/08: Further work at mouse support in graphics modes.
 * Added config capabilities via DOS tool (X_change_config).
 * -- sw
 *
 * 1998/03/15: Another try to get rid of RESIZE_KLUGE. Fixed continuous
 * X-cursor redefinition in graphics modes. Worked on color allocation.
 * -- sw
 *
 * 1998/04/05: We are using backing store now in text modes.
 * -- sw
 *
 * 1998/05/28: some re-arangements of the mouse code.
 * -- EB
 *
 * 1998/09/16: Continued work on color allocation.
 * Added graphics support for 16 color X servers (e.g. XF86_VGA16).
 * Text mode now works on all X servers incl. 16 color & mono servers.
 * -- sw
 *
 * 1998/09/20: Removed quite a lot of DAC/color init code. Were (hopefully)
 * all unnecessary. :-)
 * -- sw
 *
 * 1998/11/01: Added so far unsupported video modes (CGA, 1 bit & 2 bit VGA).
 * -- sw
 *
 * 1998/12/12: Palette/font changes in text modes improved.
 * -- sw
 *
 * 1999/01/05: Splitted X_update_screen() into two separate parts.
 * -- sw
 *
 * 1998/11/07: Updated Steffen's code to newest X.c :
 *
 *  * 1998/08/09: Started to add code to support KDE/Qt frontend.
 *  * Look for DOSEMU_WINDOW_ID.
 *  * -- sw
 *
 * -- Antonio Larrosa (antlarr@arrakis.es)
 *
 * 2000/05/31: Cleaned up mouse handling with help of Bart Oldeman.
 *  -- Eric Biederman 
 *
 * 2003/02/02: Implemented full-screen mode. Thanks to
 *             Michael Graffam <mgraffam@idsi.net> for his XF86 VidMode
 *             Extension example.
 *  -- Bart Oldeman
 *
 * 2002/11/30: Began "VRAM" user font processing (X_draw_string...)
 *  Probably not nicely integrated into the remapper -> no resizing!
 *  Experimental FONT_LAB define available, too, to override int 0x43
 *  in graphics mode. Bonus: Colorable OVERSCAN area in text mode.
 *  Changed int10, vgaemu, seqemu, vgaemu.h, attremu, X. Sigh!
 *  TODO: Code clean-up... And probably some optimizations.
 *  TODO: Use video RAM instead of vga_font_ram -> more compatibility!
 *  Introduded X_textscroll (see also vgaemu.h, int10.c).
 *  -- Eric (eric@coli.uni-sb.de) (activate: #define FONT_LAB2 1)
 *
 * DANG_END_CHANGELOG
 */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * some configurable options
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Debug level for the X interface.
 * 0 - normal / 1 - useful / 2 - too much
 */
#define DEBUG_X		0

/* show X mouse in graphics modes */
#undef DEBUG_MOUSE_POS

/* define if you wish to have a visual indication of the area being updated */
#undef DEBUG_SHOW_UPDATE_AREA

/* use backing store in text modes */
#define X_USE_BACKING_STORE

/* Do you want speaker support in X? */
#define CONFIG_X_SPEAKER 1

/* not yet -- sw */
#undef HAVE_DGA

#define CONFIG_X_MOUSE 1


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define x_msg(x...) X_printf("X: " x)

#if DEBUG_X >= 1
#define x_deb(x...) X_printf("X: " x)
#else
#define x_deb(x...)
#endif

#if DEBUG_X >= 2
#define x_deb2(x...) X_printf("X: " x)
#else
#define x_deb2(x...)
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <config.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <sys/mman.h>           /* root@sjoerd:for mprotect*/

#ifdef HAVE_XVIDMODE
#include <X11/extensions/xf86vmode.h>
#endif

#include "emu.h"
#include "timers.h"
#include "bios.h"
#include "video.h"
#include "memory.h"
#include "remap.h"
#include "vgaemu.h"
#include "vgatext.h"
#include "render.h"
#include "keyb_server.h"
#include "keyb_clients.h"
#include "X.h"
#include "dosemu_config.h"
#include "utilities.h"
#include "dos2linux.h"

#ifdef HAVE_MITSHM
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif

#ifdef HAVE_DGA
#include <X11/extensions/xf86dga.h>
#endif

#ifdef HAVE_XKB
#include <X11/XKBlib.h>
#endif

#if CONFIG_X_MOUSE
#include "mouse.h"
#include <time.h>
#endif

#if CONFIG_SELECTION
#define CONFIG_X_SELECTION 1
#include "screen.h"
#endif

#if CONFIG_X_SPEAKER
#include "speaker.h"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct { unsigned char r, g, b; } c_cube;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int (*OldXErrorHandler)(Display *, XErrorEvent *) = NULL;

#ifdef HAVE_XKB
int using_xkb = FALSE;
int xkb_event_base = 0;
int xkb_error_base = 0;
#endif

#ifdef HAVE_MITSHM
static int shm_ok = 0;
static int shm_major_opcode = 0;
static XShmSegmentInfo shminfo;
#endif

#ifdef HAVE_DGA
static int dga_ok = 0;
static int dga_active = 1;
static unsigned char *dga_addr = NULL;
static unsigned dga_scan_len = 0;
static unsigned dga_width = 0;
static unsigned dga_height = 0;
static unsigned dga_bank = 0;
static unsigned dga_mem = 0;
static unsigned _dga_width = 0;
static unsigned _dga_height = 0;
static Window dga_window;
#endif

#ifdef HAVE_XVIDMODE
static int xf86vm_ok = 0;
static int modecount;          
static XF86VidModeModeInfo **vidmode_modes;
#endif

Display *display;		/* used in plugin/?/keyb_X_keycode.c */
static int screen;
static Visual *visual;
/*
  rootwindow: RootWindow(display, DefaultScreen(display));
  parentwindow: parent: was used with kdos
  mainwindow: either equals normalwindow or fullscreenwindow
  drawwindow: window used for drawing and input (keyboard/mouse). Is a subwindow
     of either normalwindow or fullscreenwindow, using XReparentWindow (idea
     borrowed from SDL source code).
  normalwindow: the "DOS in a BOX" with title bar, icon, etc.
  fullscreenwindow: the full screen
*/
static Window rootwindow, mainwindow, parentwindow, normalwindow, fullscreenwindow,
  drawwindow;
static int toggling_fullscreen;
static Boolean our_window;     	/* Did we create the window? */

static XImage *ximage;		/* Used as a buffer for the X-server */

static Cursor X_standard_cursor;

#if CONFIG_X_MOUSE
static Cursor X_mouse_nocursor;
static int snap_X = 0;
static int mouse_cursor_visible = 0;
static int mouse_really_left_window = 1;
#endif

static GC gc;
static Atom proto_atom = None, delete_atom = None;
static int font_width, font_height;

static Boolean is_mapped = FALSE;

static int cmap_colors = 0;		/* entries in colormaps: {text,graphics}_cmap */
static Colormap text_cmap = 0, graphics_cmap = 0;
static Boolean have_shmap = FALSE;

static ColorSpaceDesc X_csd;
static int have_true_color;
unsigned dac_bits;			/* the bits visible to the dosemu app, not the real number */
static int x_res, y_res;		/* emulated window size in pixels */
static int w_x_res, w_y_res;		/* actual window size */
static int saved_w_x_res, saved_w_y_res;	/* saved normal window size */
static unsigned ximage_bits_per_pixel;
static unsigned ximage_mode;
static vga_emu_update_type veut;

static int grab_active = 0, kbd_grab_active = 0;
#if CONFIG_X_MOUSE
static char *grab_keystring = "Home";
static KeySym grab_keysym = NoSymbol;
static int mouse_x = 0, mouse_y = 0;
static int mouse_warped = 0, force_grab = 0;
#endif

static int X_map_mode = -1;
static int X_unmap_mode = -1;

static Atom comm_atom = None;
static Boolean kdos_client = FALSE;    	/* started by kdos */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* general init/destroy functions */
static int X_init(void);
static void X_close(void);
static void X_get_screen_info(void);
#ifdef HAVE_MITSHM
static void X_shm_init(void);
static void X_shm_done(void);
#endif
#ifdef HAVE_DGA
static void X_dga_init(void);
static void X_dga_done(void);
#endif
#ifdef HAVE_XVIDMODE
static void X_xf86vm_init(void);
static void X_xf86vm_done(void);
#endif

static void X_keymap_init(void);

/* error/event handler */
static int NewXErrorHandler(Display *, XErrorEvent *);
static int NewXIOErrorHandler(Display *);
static void X_handle_events(void);

/* interface to xmode.exe */
static int X_change_config(unsigned, void *);	/* modify X config data from DOS */

/* colormap related stuff */
static void graphics_cmap_init(void);
static int MakePrivateColormap(void);
static ColorSpaceDesc MakeSharedColormap(void);
static int try_cube(unsigned long *, c_cube *);

/* palette/color update stuff */
static void refresh_private_palette(DAC_entry *col, int num);

/* ximage/drawing related stuff */
static void create_ximage(void);
static void destroy_ximage(void);
static void put_ximage(int, int, unsigned, unsigned);
static void resize_ximage(unsigned, unsigned);

/* video mode set/modify stuff */
static int X_set_videomode(int, int, int);
static void X_resize_text_screen(void);
static void toggle_fullscreen_mode(void);
static void X_vidmode(int w, int h, int *new_width, int *new_height);
static void lock_window_size(unsigned wx_res, unsigned wy_res);

/* screen update/redraw functions */
static void X_reset_redraw_text_screen(void);
static void X_redraw_text_screen(void);
static int X_update_screen(void);

/* text mode init stuff (font/cursor) */
static void load_cursor_shapes(void);
static Cursor create_invisible_cursor(void);

/* text mode cursor manipulation stuff */
static void X_update_cursor(void);

#if CONFIG_X_MOUSE
/* mouse related code */
static void set_mouse_position(int, int);
static void set_mouse_buttons(int);
static void toggle_mouse_grab(void);
#endif
static void X_show_mouse_cursor(int yes);
static void X_set_mouse_cursor(int yes, int mx, int my, int x_range, int y_range);

void kdos_recv_msg(unsigned char *);
void kdos_send_msg(unsigned char *);
void kdos_close_msg(void);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Interface to video emulation.
 * used in env/video/video.c
 */
struct video_system Video_X = 
{
   NULL,
   X_init,         
   X_close,      
   X_set_videomode,      
   X_update_screen,
   X_update_cursor,
   X_change_config,
   X_handle_events
};

struct render_system Render_X =
{
   refresh_private_palette,
   put_ximage,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* utility function for opening a connection and making certain
 * I am either using or not using the X keyboard Extension.
 */
static Display *XKBOpenDisplay(char *display_name)
{
	Display *dpy;
#ifndef HAVE_XKB
	dpy = XOpenDisplay(display_name);
#else /* HAVE_XKB */
	int use_xkb;
	int major_version, minor_version;
	
	using_xkb = FALSE;

	major_version = XkbMajorVersion;
	minor_version = XkbMinorVersion;
	use_xkb = XkbLibraryVersion(&major_version, &minor_version);
	/* If I can't use the keyboard extension make
	 * sure the library doesn't either.
	 */
	XkbIgnoreExtension(!use_xkb);
	dpy = XOpenDisplay(display_name);
	if (dpy == NULL) {
		return NULL;
	}
	if (!use_xkb) {
		return dpy;
	}
	if (!XkbQueryExtension(dpy, NULL, 
			       &xkb_event_base, &xkb_error_base,
			       &major_version, &minor_version)) {
		return dpy;
	}
	using_xkb = TRUE;
#endif
	return dpy;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * DANG_BEGIN_FUNCTION X_init
 *
 * description:
 * Initialize everything X-related.
 *
 * DANG_END_FUNCTION
 */
int X_init()
{
  XGCValues gcv;
  XClassHint xch;
  XSetWindowAttributes attr;
  char *display_name; 
  char *s;
  int i, remap_src_modes;

  X_printf("X: X_init\n");

  /* Open X connection. */
  display_name = config.X_display ? config.X_display : getenv("DISPLAY");
  display = XKBOpenDisplay(display_name);
  if(display == NULL) {
    error("X: Can't open display \"%s\".\n"
    "Either the connection was refused and you do not have enough\n"
    "access rights to connect to your X server or there is\n"
    "something wrong with the contents of your DISPLAY variable.\n"
    "If the connection was refused then please consult your system\n"
    "administator or read the X documentation for a solution\n"
    "(use xauth, xhost, or ssh -X).\n",
        display_name ? display_name : "");
    leavedos(99);
  }

  /* collect some data about our X server; do it before anything else */
  X_get_screen_info();

  OldXErrorHandler = XSetErrorHandler(NewXErrorHandler);
  XSetIOErrorHandler(NewXIOErrorHandler);

#ifdef HAVE_MITSHM
  X_shm_init();
#endif

#ifdef HAVE_DGA
  X_dga_init();
#endif

#ifdef HAVE_XVIDMODE
  X_xf86vm_init();
#endif
  
  /* see if we find out something useful about our X server... -- sw */
  X_keymap_init();

  text_cmap = DefaultColormap(display, screen);		/* always use the global palette */
  graphics_cmap_init();				/* graphics modes are more sophisticated */

  /* init graphics mode support */
  remap_src_modes = remapper_init(&ximage_mode, ximage_bits_per_pixel,
				  have_true_color, have_shmap);
  if(!remap_src_modes) {
    error("X: No graphics modes supported on this type of screen!\n");
    /* why do we need a blank screen? */
    leavedos(24);
  }

  if (!vga.char_width) {
    /* set to default init values */
    font_width = vga.char_width = 9;
    font_height = vga.char_height = 16;
    vga.text_width = CO;
    vga.text_height = LI;
  }
  saved_w_x_res = w_x_res = x_res = CO * font_width;
  saved_w_y_res = w_y_res = y_res = LI * font_height;

  s = getenv("DOSEMU_WINDOW_ID");
  if(s && (i = strtol(s, NULL, 0)) > 0) {
    mainwindow = i;
    our_window = FALSE;
    kdos_client = TRUE;
    have_focus = TRUE;
    X_printf("X: X_init: using existing window (id = 0x%x)\n", i);
  }
  else {
    XSetWindowAttributes xwa;
    our_window = TRUE;
    kdos_client = FALSE;
    mainwindow = normalwindow = XCreateSimpleWindow(
      display, rootwindow,
      0, 0,			/* Position */
      w_x_res, w_y_res,		/* Size */
      0, 0,			/* Border width & color */
      0				/* Background color */
    );
    drawwindow = XCreateSimpleWindow(
      display, mainwindow,
      0, 0,			/* Position */
      w_x_res, w_y_res,		/* Size */
      0, 0,			/* Border width & color */
      0				/* Background color */
    );
    xwa.override_redirect = True;
    xwa.background_pixel = 0;
    fullscreenwindow = XCreateWindow(
      display, rootwindow,
      0, 0,			/* Position */
      DisplayWidth(display, screen), DisplayHeight(display, screen), /* Size */
      0,			/* Border width */
      CopyFromParent, CopyFromParent, CopyFromParent,
      CWBackPixel | CWOverrideRedirect,
      &xwa
    );
  }

  {
    Window root, *child_list = NULL;
    unsigned childs = 0;

    if(XQueryTree(display, mainwindow, &root, &parentwindow, &child_list, &childs)) {
      if(child_list) XFree(child_list);
    }
    else {
      parentwindow = rootwindow;
    }
    X_printf("X: X_init: parent window: 0x%x\n", (unsigned) parentwindow);
  }

  /*
   * used to communicate with kdos
   */
  if(kdos_client) {
    comm_atom = XInternAtom(display, "DOSEMU_COMM_ATOM", False);
    X_printf("X: X_init: got Atom: DOSEMU_COMM_ATOM = 0x%x\n", (unsigned) comm_atom);
  }

  X_printf("X: X_init: screen = %d, root = 0x%x, mainwindow = 0x%x\n",
    screen, (unsigned) rootwindow, (unsigned) mainwindow
  );

  load_cursor_shapes();

  gc = XCreateGC(display, drawwindow, 0, &gcv);

  attr.event_mask = StructureNotifyMask;
  XChangeWindowAttributes(display, fullscreenwindow, CWEventMask, &attr);
  attr.event_mask |=
    KeyPressMask | KeyReleaseMask | KeymapStateMask | FocusChangeMask;
  XChangeWindowAttributes(display, mainwindow, CWEventMask, &attr);

  attr.event_mask =
    ButtonPressMask | ButtonReleaseMask |
    EnterWindowMask | LeaveWindowMask | PointerMotionMask |
    ExposureMask |
    ColormapChangeMask;

  attr.cursor = X_standard_cursor;

  XChangeWindowAttributes(display, drawwindow, CWEventMask | CWCursor, &attr);

  XStoreName(display, mainwindow, config.X_title);
  XSetIconName(display, mainwindow, config.X_icon_name);
  xch.res_name  = "XDosEmu";
  xch.res_class = "XDosEmu";

  if (our_window) {
    XWMHints wmhint;
    wmhint.window_group = mainwindow;
    wmhint.input = True;
    wmhint.flags = WindowGroupHint | InputHint;
    XSetWMProperties(display, mainwindow, NULL, NULL, 
      dosemu_argv, dosemu_argc, NULL, &wmhint, &xch);
  }
  else {
    XSetClassHint(display, mainwindow, &xch);
  }
  /* Delete-Window-Message black magic copied from xloadimage. */
  proto_atom  = XInternAtom(display, "WM_PROTOCOLS", False);
  delete_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
  if ((proto_atom != None) && (delete_atom != None)) {
    XChangeProperty(display, mainwindow,
      proto_atom, XA_ATOM, 32,
      PropModePrepend, (char *) &delete_atom, 1
    );
  }

  /* don't map window if set */
  if(getenv("DOSEMU_HIDE_WINDOW") == NULL) {
    XMapWindow(display, mainwindow);
    XMapWindow(display, drawwindow);
  }

  if(have_true_color || have_shmap)
    Render_X.refresh_private_palette = NULL;
  register_render_system(&Render_X);
  X_load_text_font(display, 0, drawwindow, config.X_font,
		   &font_width, &font_height);
  if(!use_bitmap_font)
    X_resize_text_screen();

  /* initialize VGA emulator */
  if(vga_emu_init(remap_src_modes, &X_csd)) {
    error("X: X_init: VGAEmu init failed!\n");
    leavedos(99);
  }

  if(config.X_mgrab_key) grab_keystring = config.X_mgrab_key;
  if(*grab_keystring) grab_keysym = XStringToKeysym(grab_keystring);
  if(grab_keysym != NoSymbol) {
    X_printf("X: X_init: mouse grabbing enabled, use Ctrl+Mod1+%s to activate\n", grab_keystring);
  }
  else {
    X_printf("X: X_init: mouse grabbing disabled\n");
  }

  if (config.X_fullscreen)
    toggle_fullscreen_mode();

#if CONFIG_X_SPEAKER
  register_speaker(display, X_speaker_on, X_speaker_off);
#endif

  return 0;
}

/*
 * DANG_BEGIN_FUNCTION X_close
 *
 * description:
 * Destroy the window, unload font, pixmap and colormap.
 *
 * DANG_END_FUNCTION
 */
void X_close()
{
  X_printf("X: X_close\n");

  if(display == NULL) return;

#if CONFIG_X_SPEAKER
  /* turn off the sound, and */
  speaker_off();
  /* reset the speaker to it's default */
  register_speaker(NULL, NULL, NULL);
#endif

  if (kdos_client) kdos_close_msg();

#ifdef HAVE_XVIDMODE
  X_xf86vm_done();
#endif

  X_load_text_font(display, 0, drawwindow, NULL, NULL, NULL);
  if(our_window) {
    XDestroyWindow(display, drawwindow);
    XDestroyWindow(display, normalwindow);
    XDestroyWindow(display, fullscreenwindow);
  }

  destroy_ximage();

  vga_emu_done();
  
  if(graphics_cmap) XFreeColormap(display, graphics_cmap);

  XFreeGC(display, gc);

  if(X_csd.pixel_lut != NULL) { free(X_csd.pixel_lut); X_csd.pixel_lut = NULL; }

  remapper_done();

#ifdef HAVE_DGA
  X_dga_done();
#endif

#ifdef HAVE_MITSHM
  X_shm_done();
#endif

  if(OldXErrorHandler != NULL) {
    XSetErrorHandler(OldXErrorHandler);
    OldXErrorHandler = NULL;
  }

  XCloseDisplay(display);
}


/*
 * Collect some information about the X server.
 */
void X_get_screen_info()
{
  XImage *xi;
  char *s;

  screen = DefaultScreen(display);
  rootwindow = RootWindow(display, screen);
  visual = DefaultVisual(display, DefaultScreen(display));

  have_true_color = visual->class == TrueColor || visual->class == DirectColor ? 1 : 0;

  switch(visual->class) {
    case StaticGray:  s = "StaticGray"; break;
    case GrayScale:   s = "GrayScale"; break;
    case StaticColor: s = "StaticColor"; break;
    case PseudoColor: s = "PseudoColor"; break;
    case TrueColor:   s = "TrueColor"; break;
    case DirectColor: s = "DirectColor"; break;
    default:          s = "Unknown";
  }
  X_printf("X: visual class is %s\n", s);

  if(have_true_color) {
    X_printf("X: using true color visual\n");
  }
  else {
    int i = DefaultDepth(display, DefaultScreen(display));

    if(i > 8) i = 8;				/* we will never try to get more */
    if((cmap_colors = 1 << i) == 1) {
      X_printf("X: Oops, your screen has only *one* color?\n");
    }
    else {
      X_printf("X: colormaps have %d colors\n", cmap_colors);
    }
  }

  xi = XCreateImage( /* this is just generated to allow measurements */
    display,
    visual,
    DefaultDepth(display, DefaultScreen(display)),
    ZPixmap, 0, NULL, 6 * 8, 2, 32, 0
  );

  if(xi != NULL) {
    if(xi->bytes_per_line % 6 == 0 && (xi->bytes_per_line / 6) == xi->bits_per_pixel) {
      ximage_bits_per_pixel = xi->bits_per_pixel;
      X_printf("X: pixel size is %d bits/pixel\n", ximage_bits_per_pixel);
    }
    else {
      ximage_bits_per_pixel = xi->bits_per_pixel;
      X_printf("X: could not determine pixel size, guessing %d bits/pixel\n", ximage_bits_per_pixel);
    }

    XDestroyImage(xi);
  }
  else {
    ximage_bits_per_pixel = DefaultDepth(display, DefaultScreen(display));
    X_printf("X: could not determine pixel size, guessing %d bits/pixel\n", ximage_bits_per_pixel);
  }

  X_csd.bits = ximage_bits_per_pixel;
  X_csd.bytes = (ximage_bits_per_pixel + 7) >> 3;
  X_csd.r_mask = visual->red_mask;
  X_csd.g_mask = visual->green_mask;
  X_csd.b_mask = visual->blue_mask;
  color_space_complete(&X_csd);
  if(X_csd.bits == 16 && (X_csd.r_bits + X_csd.g_bits + X_csd.b_bits) == 15) {
    X_csd.bits = ximage_bits_per_pixel = 15;
  }
}


#ifdef HAVE_MITSHM
/*
 * DANG_BEGIN_FUNCTION X_shm_init
 *
 * description:
 * Check availability of the MIT-SHM shared memory extension.
 *
 * DANG_END_FUNCTION
 */
void X_shm_init()
{
  int event_base, error_base, major_version, minor_version;
  Bool shared_pixmaps;

  shm_ok = 0;

  if(!config.X_mitshm) return;

  if(!XQueryExtension(display, "MIT-SHM", &shm_major_opcode, &event_base, &error_base)) {
    X_printf("X: X_shm_init: server does not support MIT-SHM\n");
    return;
  }

  X_printf("X: MIT-SHM major opcode: %d\n", shm_major_opcode);

  if(!XShmQueryVersion(display, &major_version, &minor_version, &shared_pixmaps)) {
    X_printf("X: X_shm_init: XShmQueryVersion() failed\n");
    return;
  }

  X_printf("X: using MIT-SHM\n");

  shm_ok = 1;
}


/*
 * DANG_BEGIN_FUNCTION X_shm_init
 *
 * description:
 * Turn off usage of the MIT-SHM shared memory extension.
 *
 * DANG_END_FUNCTION
 */
void X_shm_done()
{
  shm_ok = 0;
}
#endif


#ifdef HAVE_DGA
/*
 * Check for XFree86-DGA.
 */
void X_dga_init()
{
  int event_base, error_base, major_version, minor_version;

  dga_ok = 0;

  if(!XF86DGAQueryExtension(display, &event_base, &error_base)) {
    X_printf("X: server does not support XFree86-DGA\n");
    return;
  }

  X_printf("X: XFree86-DGA ErrorBase: %d\n", error_base);

  if(!XF86DGAQueryVersion(display, &major_version, &minor_version)) {
    X_printf("X: XF86DGAQueryVersion() failed\n");
    return;
  }

  X_printf("X: using XFree86-DGA\n");

  dga_ok = 1;
}


/*
 * Turn off XFree86-DGA.
 */
void X_dga_done()
{
  dga_ok = 0;
}
#endif

#ifdef HAVE_XVIDMODE
static void X_xf86vm_init(void)
{
  unsigned int eventB, errorB, ver, rev;
  if (XF86VidModeQueryExtension(display, &eventB, &errorB))
  {
    XF86VidModeQueryVersion(display, &ver, &rev);
    X_printf("X: VidMode Extension version %u.%u\n", ver, rev);
    XF86VidModeGetAllModeLines(display, 0, &modecount, &vidmode_modes);
    xf86vm_ok = 1;
  }
  else
  {
    X_printf("X: xvidmode: No VidMode Extension\n");
  }
}

static void X_xf86vm_done(void)
{
  if (mainwindow == fullscreenwindow)
    X_vidmode(-1, -1, &w_x_res, &w_y_res);
  xf86vm_ok = 0;
}

#endif 

/*
 * Handle 'auto'-entries in dosemu.conf, namely
 * $_X_keycode & $_layout
 *
 */
static void X_keymap_init()
{
  char *s = ServerVendor(display);

  if(s) X_printf("X: X_keymap_init: X server vendor is \"%s\"\n", s);
  if(config.X_keycode == 2 && s) {	/* auto */
#if defined(HAVE_XKB)
    /* All I need to know is that I'm using the X keyboard extension */
    config.X_keycode = using_xkb;
#else
    config.X_keycode = 0;

    /*
     * We could check some typical keycode/keysym translation; but
     * for now we just check the server vendor. XFree & XiGraphics
     * work. I don't know about Metro Link... -- sw
     */
    if(
      strstr(s, "The XFree86 Project") ||
      strstr(s, "Xi Graphics")
    ) config.X_keycode = 1;
#endif /* HAVE_UNICODE_KEYB && HAVE_XKB */
  }
  X_printf(
    "X: X_keymap_init: %susing DOSEMU's internal keycode translation\n",
    config.X_keycode ? "" : "we are not "
  );
}


/*
 * This function provides an interface to reconfigure parts
 * of X and the VGA emulation during a DOSEMU session.
 * It is used by the xmode.exe program that comes with DOSEMU.
 */
static int X_change_config(unsigned item, void *buf)
{
  int err = 0;

  X_printf("X: X_change_config: item = %d, buffer = %p\n", item, buf);

  switch(item) {

    case CHG_TITLE:
      /* low-level write */
      if (buf) {
	X_printf("X: X_change_config: win_name = %s\n", (char *) buf);
	/* always change the normal window's title - never the full-screen one */
	XStoreName(display, normalwindow, buf);
	break;
      }
      /* high-level write (shows name of emulator + running app) */
      /* fall through */
       
    case CHG_TITLE_EMUNAME:
    case CHG_TITLE_APPNAME:
    case CHG_TITLE_SHOW_APPNAME:
    case CHG_WINSIZE:
    case CHG_BACKGROUND_PAUSE:
    case GET_TITLE_APPNAME:
      change_config(item, buf, grab_active, kbd_grab_active);
      break;

    case CHG_FONT:
      X_load_text_font(display, 0, drawwindow, buf,
		       &font_width, &font_height);
      if (use_bitmap_font) {
        register_render_system(&Render_X);
        font_width = vga.char_width;
        font_height = vga.char_height;
        if(vga.mode_class == TEXT) X_resize_text_screen();
      } else {
        if (w_x_res != vga.text_width * font_width ||
            w_y_res != vga.text_height * font_height) {
          if(vga.mode_class == TEXT) X_resize_text_screen();
        }
      }
      break;

    case CHG_MAP:
      X_map_mode = *((int *) buf);
      X_printf("X: X_change_config: map window at mode 0x%02x\n", X_map_mode);
      if(X_map_mode == -1) {
	XMapWindow(display, mainwindow);
	XMapWindow(display, drawwindow);
      }
      break;

    case CHG_UNMAP:
      X_unmap_mode = *((int *) buf);
      X_printf("X: X_change_config: unmap window at mode 0x%02x\n", X_unmap_mode);
      if(X_unmap_mode == -1) {
	XUnmapWindow(display, drawwindow);
	XUnmapWindow(display, mainwindow);
      }
      break;

    case CHG_FULLSCREEN:
      X_printf("X: X_change_config: fullscreen %i\n", *((int *) buf));
      if (*((int *) buf) == (mainwindow == normalwindow))
	toggle_fullscreen_mode();
      break;

    default:
      err = 100;
  }

  return err;
}

/*
 * fatal error handler
 */
int NewXIOErrorHandler(Display *dsp)
{
  error("Fatal X error, leaving\n");
  leavedos(99);
  return 0;
}

/*
 * The error handler just catches the first MIT-SHM errors.
 * This is needed to check for MIT-SHM.
 */
int NewXErrorHandler(Display *dsp, XErrorEvent *xev)
{
#ifdef HAVE_MITSHM
  if(xev->request_code == shm_major_opcode) {
    X_printf("X::NewXErrorHandler: error using shared memory\n");
    shm_ok = 0;
  }
  else
#endif
  {
    /* the default X handler causes DOSEMU to exit uncleanly
       so we call leavedos() here */
    leavedos(99);
  }
#ifdef HAVE_MITSHM
  return 0;
#endif
}

/*
 * called exclusively by mouse INT33 funcs 1 and 2 of src/base/mouse/mouse.c
 * NOTE: We need this in addition to X_set_mouse_cursor to 'autoswitch'
 * on/off hiding the X cursor in graphics mode, when the DOS application
 * is drawing the cursor itself. We rely on the fact, that an application
 * wanting the mouse driver to draw the cursor will always use
 * INT33 functions 1 and 2, while others won't.
 *
 * So please, don't 'optimize' X_show_mouse_cursor away (;-), its
 * not bogus.		--Hans, 2000/06/01
 */
static void X_show_mouse_cursor(int yes)
{
   if (yes || vga.mode_class != GRAPH) {
      if (mouse_cursor_visible) return;
      if (grab_active) {
         XDefineCursor(display, drawwindow, X_mouse_nocursor);
      } else {
         XDefineCursor(display, drawwindow, X_standard_cursor);
      }
      mouse_cursor_visible = 1;
   } else {
      if (!mouse_cursor_visible) return;
      XDefineCursor(display, drawwindow, X_mouse_nocursor);
      mouse_cursor_visible = 0;
   }
}

static void toggle_kbd_grab(void)
{
  if(kbd_grab_active ^= 1) {
    X_printf("X: keyboard grab activated\n");
    if (mainwindow != fullscreenwindow) {
      XGrabKeyboard(display, drawwindow, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    }
  }
  else {
    X_printf("X: keyboard grab released\n");
    if (mainwindow != fullscreenwindow) {
      XUngrabKeyboard(display, CurrentTime);
    }
  }
  X_change_config(CHG_TITLE, NULL);
}

static void toggle_mouse_grab(void)
{
  if(grab_active ^= 1) {
    config.mouse.use_absolute = 0;
    X_printf("X: mouse grab activated\n");
    if (mainwindow != fullscreenwindow) {
      XGrabPointer(display, drawwindow, True, PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                   GrabModeAsync, GrabModeAsync, drawwindow,  None, CurrentTime);
    }
    X_set_mouse_cursor(mouse_cursor_visible, mouse_x, mouse_y, w_x_res, w_y_res);
    mouse_enable_native_cursor(1);
  }
  else {
    config.mouse.use_absolute = 1;
    X_printf("X: mouse grab released\n");
    if (mainwindow != fullscreenwindow) {
      XUngrabPointer(display, CurrentTime);
    }
    X_set_mouse_cursor(mouse_cursor_visible, mouse_x, mouse_y, w_x_res, w_y_res);
    mouse_enable_native_cursor(0);
  }
  clear_selection_data();
  X_change_config(CHG_TITLE, NULL);
}

/*
 * DANG_BEGIN_FUNCTION X_set_mouse_cursor
 *
 * description:
 * called by mouse.c to hide/display the mouse and set it's position.
 * This is currently the only callback from mouse.c to X.
 *
 * DANG_END_FUNCTION
 *
 */  
static void X_set_mouse_cursor(int action, int mx, int my, int x_range, int y_range)
{
#if CONFIG_X_MOUSE
        int yes = action & 1;
	static Cursor *last_cursor = 0;
	Cursor *mouse_cursor_on, *mouse_cursor_off, *new_cursor;
	int x,y;

        if (action & 2)
          X_show_mouse_cursor(yes);

        /* Figure out what cursor we want to show for visible/invisible */
	mouse_cursor_on = &X_standard_cursor;
	mouse_cursor_off = &X_mouse_nocursor;

	if (vga.mode_class != GRAPH) {
		/* So cut and past still works with a hidden cursor */
		mouse_cursor_off = &X_standard_cursor;
	}
	if (grab_active) {
		mouse_cursor_on = &X_mouse_nocursor;
		mouse_cursor_off = &X_mouse_nocursor;
	}
	/* See if we are visible/invisible */
	new_cursor = (yes)? mouse_cursor_on: mouse_cursor_off;

	/* Update the X cursor as needed */
	if (new_cursor != last_cursor) {
		XDefineCursor(display, drawwindow, *new_cursor);
		last_cursor = new_cursor;
	}

	if (grab_active || snap_X || !have_focus)
		return;

	/* Move the X cursor if needed */
	x = (x_range * mouse_x)/w_x_res;
	y = (y_range * mouse_y)/w_y_res;
	if ((mx != x) || (my != y)) {
		XWarpPointer(display, None, drawwindow, 0, 0, 0, 0,
			     (w_x_res * mx)/x_range,
			     (w_y_res * my)/y_range);
			/* ignore event caused by the pointer warp */
		mouse_warped = 1;
	}
#endif /* CONFIG_X_MOUSE */
}

/* From SDL: Called after unmapping a window - waits until the window is unmapped */
static void X_wait_unmapped(Window win)
{
  XEvent event;
  do {
    XMaskEvent(display, StructureNotifyMask, &event);
  } while ( (event.type != UnmapNotify) || (event.xunmap.event != win) );
}

static void toggle_fullscreen_mode(void)
{
  unsigned resize_height, resize_width;

  XUnmapWindow(display, mainwindow);
  X_wait_unmapped(mainwindow);
  if (mainwindow == normalwindow) {
    int shift_x = 0, shift_y = 0;

    X_printf("X: entering fullscreen mode\n");
    toggling_fullscreen = 2;
    saved_w_x_res = w_x_res;
    saved_w_y_res = w_y_res;
    if (!grab_active) {
      toggle_mouse_grab();
      force_grab = 1;
    }
    X_vidmode(x_res, y_res, &resize_width, &resize_height);
    mainwindow = fullscreenwindow;
    if (vga.mode_class == GRAPH || use_bitmap_font) {
      XResizeWindow(display, mainwindow, resize_width, resize_height);
      XResizeWindow(display, drawwindow, resize_width, resize_height);
    } else {
      shift_x = (resize_width - w_x_res) / 2;
      shift_y = (resize_height - w_y_res) / 2;
    }
    XMapWindow(display, mainwindow);
    XRaiseWindow(display, mainwindow);
    XReparentWindow(display, drawwindow, mainwindow, shift_x, shift_y);
    XGrabPointer(display, drawwindow, True,
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                 GrabModeAsync, GrabModeAsync, drawwindow,  None,
                 CurrentTime);
    XGrabKeyboard(display, drawwindow, True, GrabModeAsync,
                  GrabModeAsync, CurrentTime);
  } else {
    X_printf("X: entering windowed mode!\n");
    w_x_res = saved_w_x_res;
    w_y_res = saved_w_y_res;
    XUngrabKeyboard(display, CurrentTime);
    XUngrabPointer(display, CurrentTime);
    if (force_grab && grab_active) {
      toggle_mouse_grab();
    }
    force_grab = 0;
    mainwindow = normalwindow;
    X_vidmode(-1, -1, &resize_width, &resize_height);
    if (vga.mode_class == GRAPH || use_bitmap_font) {
      XResizeWindow(display, mainwindow, resize_width, resize_height);
      XResizeWindow(display, drawwindow, resize_width, resize_height);
    }
    XMapWindow(display, mainwindow);
    XReparentWindow(display, drawwindow, mainwindow, 0, 0);
  }
  if(vga.mode_class == TEXT && !use_bitmap_font) {
    X_resize_text_screen();
  } else {	/* GRAPH or builtin font */
    resize_ximage(resize_width, resize_height);
    dirty_all_video_pages();
    X_update_screen();
  }
}

/*
 * DANG_BEGIN_FUNCTION X_handle_events
 *
 * description:
 * Handle pending X events (called from SIGALRM handler).
 *
 * DANG_END_FUNCTION
 */
static void X_handle_events(void)
{
   XEvent e, rel_evt;
   unsigned resize_width = w_x_res, resize_height = w_y_res, resize_event = 0;
   int keyrel_pending = 0;

#if CONFIG_X_MOUSE
   {
     static int lastingraphics = 0;
     if(vga.mode_class == GRAPH) {
       if(! lastingraphics) {
         lastingraphics = 1;
         X_show_mouse_cursor(0);
       }
     }
     else {
       if(lastingraphics) {
         lastingraphics = 0;
         X_show_mouse_cursor(1);
       }
     }
   }
#endif	/* CONFIG_X_MOUSE */

  while (XPending(display) > 0) 
    {
      XNextEvent(display,&e);

      switch(e.type) 
	{
       case Expose:
          /*
           * Well, if we're exposed we are most certainly mapped, too. :-)
           *
           * This is actually a kludge to work around some strange
           * effect related to the DOSEMU_WINDOW_ID stuff.
           *
           * The problem arises that, when the window is not created
           * by DOSEmu, it is typically already mapped. For some really
           * strange reason however, you cannot simply set is_mapped to TRUE.
           * Apparently there is some (black) magic going on somewhere in
           * DOSEmu that assumes that some initialisation should be
           * done while is_mapped is FALSE. I couldn't locate the
           * exact position, though.
           * 
           * -- 1998/08/09 sw
           */
          is_mapped = TRUE;

	  X_printf("X: expose event\n");
	  if(vga.mode_class == TEXT) {
	    if(e.xexpose.count == 0) X_redraw_text_screen();
	  }
	  else {	/* GRAPH */
	    if(!resize_event) put_ximage(
	      e.xexpose.x, e.xexpose.y,
	      e.xexpose.width, e.xexpose.height
	    );
	  }
	  break;

	case UnmapNotify:
	  X_printf("X: window unmapped\n");
	  is_mapped = FALSE;
	  break;
	  
	case MapNotify:
	  X_printf("X: window mapped\n");
	  is_mapped = TRUE;
	  break;
	  
	case FocusIn:
	  X_printf("X: focus in\n");
	  if (vga.mode_class == TEXT) text_gain_focus();
	  if (config.X_background_pause && !dosemu_user_froze) unfreeze_dosemu ();
	  have_focus = TRUE;
	  break;

	case FocusOut:
	  X_printf("X: focus out\n");
	  if (mainwindow == fullscreenwindow) break;
	  if (vga.mode_class == TEXT) text_lose_focus();
	  output_byte_8042(port60_buffer | 0x80);
	  if (config.X_background_pause && !dosemu_user_froze) freeze_dosemu ();
	  have_focus = FALSE;
	  break;

	case DestroyNotify:
	  X_printf("X: window got destroyed\n");
	  leavedos(99);
	  break;
	  
	case ClientMessage:
	  /* If we get a client message which has the value of the delete
	   * atom, it means the window manager wants us to die.
	   */
	  if(e.xclient.message_type == proto_atom && *e.xclient.data.l == delete_atom) {
	    X_printf("X: got window delete message\n");
	    /* XXX - Is it ok to call this from a SIGALRM handler? */
	    leavedos(0);
	    break;
	  }

	  if(e.xclient.message_type == comm_atom) {
	    kdos_recv_msg(e.xclient.data.b);
	  }
	  break;

    /* Selection-related events */
#if CONFIG_X_SELECTION
	case SelectionClear:
	case SelectionNotify: 
	case SelectionRequest:
	  X_handle_selection(display, drawwindow, &e);
	  break;
#endif /* CONFIG_X_SELECTION */
		   
    /* Keyboard events */

	case KeyPress:
	  /* Autorepeat generates the unwanted release events, we filter
	   * them here. */
	  if (keyrel_pending && e.xkey.keycode == rel_evt.xkey.keycode &&
		e.xkey.time == rel_evt.xkey.time) {
	    X_printf("X_KBD: Ignoring fake release event, keycode=%#x\n",
		rel_evt.xkey.keycode);
	    keyrel_pending = 0;
	  }

          if((e.xkey.state & ControlMask) && (e.xkey.state & Mod1Mask)) {
            KeySym keysym = XKeycodeToKeysym(display, e.xkey.keycode, 0);
            if (keysym == grab_keysym) {
              force_grab = 0;
              toggle_mouse_grab();
              break;
            } else if (keysym == XK_k) {
              toggle_kbd_grab();
              break;
            } else if (keysym == XK_f) {
              toggle_fullscreen_mode();
              break;
            }
          }
/* 
      Clears the visible selection if the cursor is inside the selection
*/
#if CONFIG_X_SELECTION
	  clear_if_in_selection();
#endif
	  X_process_key(&e.xkey);
	  break;

	case KeyRelease:
	  if (keyrel_pending) {
	    X_printf("X: duplicate KeyRelease event???\n");
	    X_process_key(&rel_evt.xkey);
	  }
	  rel_evt = e;
	  keyrel_pending = 1;
	  break;

	case KeymapNotify:
          X_printf("X: KeymapNotify event\n");
          /* don't process keys when doing fullscreen switching (this generates
             two events for fullscreen and one back to windowed mode )*/
          X_process_keys(&e.xkeymap);
	  break;

    /* A keyboard mapping has been changed (e.g., with xmodmap). */
	case MappingNotify:  
	  X_printf("X: MappingNotify event\n");
	  XRefreshKeyboardMapping(&e.xmapping);
	  break;

/* Mouse events */
#if CONFIG_X_MOUSE  
	case ButtonPress:
#if 0 /* *** special debug code *** --sw */
        if(e.xbutton.button == Button1) {
          static unsigned flup = 0;
          v_printf("------: %u\n", ++flup);
        }
#endif /* *** end sdc *** */

#if CONFIG_X_SELECTION
	if (vga.mode_class == TEXT && !grab_active) {
	  if (e.xbutton.button == Button1)
	    start_selection(x_to_col(e.xbutton.x, w_x_res),
			    y_to_row(e.xbutton.y, w_y_res));
	  else if (e.xbutton.button == Button3)
	    start_extend_selection(x_to_col(e.xbutton.x, w_x_res),
				   y_to_row(e.xbutton.y, w_y_res));
	}
#endif /* CONFIG_X_SELECTION */
	  set_mouse_position(e.xmotion.x,e.xmotion.y); /*root@sjoerd*/
	  set_mouse_buttons(e.xbutton.state|(0x80<<e.xbutton.button));
	  break;
	  
	case ButtonRelease:
	  set_mouse_position(e.xmotion.x,e.xmotion.y);  /*root@sjoerd*/
#if CONFIG_X_SELECTION
	  if (vga.mode_class == TEXT) 
	    X_handle_selection(display, drawwindow, &e);
#endif /* CONFIG_X_SELECTION */
	  set_mouse_buttons(e.xbutton.state&~(0x80<<e.xbutton.button));
	  break;
	  
	case MotionNotify:
#if CONFIG_X_SELECTION
	  extend_selection(x_to_col(e.xmotion.x, w_x_res),
			   y_to_row(e.xmotion.y, w_y_res));
#endif /* CONFIG_X_SELECTION */
	  set_mouse_position(e.xmotion.x, e.xmotion.y);
	  break;
	  
	case EnterNotify:
	  /* Here we trigger the kludge to snap in the cursor
	   * drawn by win31 and align it with the X-cursor.
	   * (Hans)
	   */
          X_printf("X: Mouse entering window event\n");
          if (mouse_really_left_window)
          {
            X_printf("X: Mouse really entering window\n");
	    if(!grab_active) snap_X=3;
	    set_mouse_position(e.xcrossing.x, e.xcrossing.y);
	    set_mouse_buttons(e.xcrossing.state);
          }
	  break;
	  
	case LeaveNotify:                   /* Should this do anything? */
	  X_printf("X: Mouse leaving window, coordinates %d %d\n", e.xcrossing.x, e.xcrossing.y);
          /* some stupid window managers send this event if you click a
             mouse button when the mouse is in the window. Let's ignore the
             next "EnterNotify" if the mouse doesn't really leave the window */
          mouse_really_left_window = 1;
          if (e.xcrossing.x >= 0 && e.xcrossing.x < w_x_res &&
              e.xcrossing.y >= 0 && e.xcrossing.y < w_y_res) {
            X_printf("X: bogus LeaveNotify event\n");
            mouse_really_left_window = 0;
          }
	  break;

        case ConfigureNotify:
          /* printf("X: configure event: width = %d, height = %d\n", e.xconfigure.width, e.xconfigure.height); */
 	/* DANG_BEGIN_REMARK
 	 * DO NOT REMOVE THIS TEST!!!
 	 * It is magic, without it EMS fails on my machine under X.
 	 * Perhaps someday when we don't use a buggy /proc/self/mem..
 	 * -- EB 18 May 1998
	 * A slightly further look says it's not the test so much as
	 * suppressing noop resize events...
	 * -- EB 1 June 1998
 	 * DANG_END_REMARK
 	 */
          if ((e.xconfigure.width != resize_width)
                       || (e.xconfigure.height != resize_height)) {
            resize_event = 1;
            resize_width = e.xconfigure.width;
            resize_height = e.xconfigure.height;
          }
          break;

#endif /* CONFIG_X_MOUSE */
/* some weirder things... */
	}
    }

    if (keyrel_pending) {
#if CONFIG_X_SELECTION
      clear_if_in_selection();
#endif
      X_process_key(&rel_evt.xkey);
      keyrel_pending = 0;
    }

    if(ximage != NULL && resize_event) {
      if(ximage->width == resize_width && ximage->height == resize_height) resize_event = 0;
    }

    if(resize_event && mainwindow == normalwindow) {
      XResizeWindow(display, drawwindow, resize_width, resize_height);
      resize_ximage(resize_width, resize_height);
      dirty_all_video_pages();
      if (vga.mode_class == TEXT)
	vga.reconfig.mem = 1;
      X_update_screen();
    }

#if CONFIG_X_MOUSE  
  do_mouse_irq();
#endif  
}


/*
 * DANG_BEGIN_FUNCTION graphics_cmap_init
 *
 * description:
 * Allocate a colormap for graphics modes and initialize it.
 * Do mostly nothing on true color displays.
 * Otherwise, do:
 *   - if colormaps have less than 256 entries (notably 16 or 2 colors),
 *     don't use a private colormap
 *   - if a shared map is requested and there are less than 36 colors (3/4/3)
 *     available, use a private colormap
 *
 * Note: Text modes always use the screen's default colormap.
 *
 * DANG_END_FUNCTION
 *
 * Note: The above is not fully true. If on 16 or 2 color displays the
 * allocation of a shared map fails for some reason, the code still tries
 * to get a private map (and will likely fail). -- sw
 */
void graphics_cmap_init()
{
  /* Note: should really deallocate and reallocate our colours every time
   we switch graphic modes... --adm */

  if(have_true_color) return;

  if(graphics_cmap == 0) {	/* only the first time! */
    have_shmap = FALSE;

    if(config.X_sharecmap || cmap_colors < 256) {		/* notably on 16 color and mono screens */
      graphics_cmap = DefaultColormap(display, screen);
      X_csd = MakeSharedColormap();

      if(X_csd.bits == 1) {
        X_printf("X: graphics_cmap_init: couldn't get enough free colors; trying private colormap\n");
        have_shmap = FALSE;
      }
      else {
        if(X_csd.bits < 4 * 5 * 4) {
          X_printf("X: graphics_cmap_init: couldn't get many free colors (%d). May look bad.\n", X_csd.bits);
        }
      }

      if(X_csd.bits != 1) have_shmap = TRUE;
    }

    if(!have_shmap) {
      if(MakePrivateColormap()) {
        X_printf("X: graphics_cmap_init: using private colormap.\n");
      }
      else {
        graphics_cmap = 0;
        error("X: graphics_cmap_init: Couldn't get a colormap for graphics modes!\n");
      }
    }
    else {
      X_printf("X: graphics_cmap_init: using shared colormap.\n");
    }
  }
  else {
    X_printf("X: graphics_cmap_init: unexpectedly called\n");
  }
}


/*
 * Create a private colormap. The colormap is *not* initialized.
 */
int MakePrivateColormap()
{
  int i;
  unsigned long pixels[256];		/* ok, cmap_colors always <= 256 */

  graphics_cmap = XCreateColormap(display, rootwindow, visual, AllocNone);

  i = XAllocColorCells(display, graphics_cmap, True, NULL, 0, pixels, cmap_colors);
  if(!i) {
    X_printf("X: failed to allocate private color map (no PseudoColor visual)\n");
    return 0;
  }

  return 1;
}


/*
 * Create a shared colormap. The colormap's features are
 * described with a ColorSpaceDesc object (cf. graphics_cmap_init).
 */
ColorSpaceDesc MakeSharedColormap()
{
  ColorSpaceDesc csd;
  int i, j;
  static unsigned long pix[256];
  static c_cube c_cubes[] = {
    { 6, 8, 5 }, { 6, 7, 5 }, { 6, 6, 5 }, { 5, 7, 5 },
    { 5, 6, 5 }, { 4, 8, 4 }, { 5, 6, 4 }, { 5, 5, 4 },
    { 4, 5, 4 }, { 4, 5, 3 }, { 4, 4, 3 }, { 3, 4, 3 }
  };

  csd.bytes = 1;
  csd.pixel_lut = NULL;
  csd.r_mask = csd.g_mask = csd.b_mask = 0;

  for(i = j = 0; i < sizeof(c_cubes) / sizeof(*c_cubes); i++) {
    if((j = try_cube(pix, c_cubes + i))) break;
  }

  if(!j) {
    X_printf("X: MakeSharedColormap: failed to allocate shared color map\n");
    csd.r_bits = 1;
    csd.g_bits = 1;
    csd.b_bits = 1;
  }
  else {
    csd.r_bits = c_cubes[i].r;
    csd.g_bits = c_cubes[i].g;
    csd.b_bits = c_cubes[i].b;
  }

  csd.r_shift = 1;
  csd.g_shift = csd.r_bits * csd.r_shift;
  csd.b_shift = csd.g_bits * csd.g_shift;

  csd.bits = csd.r_bits * csd.g_bits * csd.b_bits;

  if(csd.bits > 1) {
    csd.pixel_lut = malloc(csd.bits * sizeof(*csd.pixel_lut));
    for(i = 0; i < csd.bits; i++) csd.pixel_lut[i] = pix[i];
  }

  X_printf("X: MakeSharedColormap: RGBCube %d - %d - %d (%d colors)\n", csd.r_bits, csd.g_bits, csd.b_bits, csd.bits);

  return csd;
}


/*
 * Try to allocate a color cube (cf. MakeSharedColormap).
 */
int try_cube(unsigned long *p, c_cube *c)
{
  unsigned r, g, b;
  int i = 0;
  XColor xc;

  xc.flags = DoRed | DoGreen | DoBlue;

  for(b = 0; b < c->b; b++) for(g = 0; g < c->g; g++) for(r = 0; r < c->r; r++) {
    xc.red = (0xffff * r) / c->r;
    xc.green = (0xffff * g) / c->g;
    xc.blue = (0xffff * b) / c->b;
    if(XAllocColor(display, graphics_cmap, &xc)) {
      p[i++] = xc.pixel;
    }
    else {
      while(--i >= 0) XFreeColors(display, graphics_cmap, p + i, 1, 0);
      return 0;
    }
  }
  return i;
}


/*
 * Update the private palette to match the current DAC entries.
 */
void refresh_private_palette(DAC_entry *col, int num)
{
  XColor xcolor[256];
  RGBColor c;
  int i, k;
  unsigned bits, shift;

  /*
   * Colormap size is limited to cmap_colors, not 256. This leads to
   * incorrect displays at screens with less than 8 bit color depth.
   * --> So don't use private palettes with less than 256 colors.
   */

  for(i = k = 0; k < num; k++) {
    if(col[k].index < cmap_colors) {
      c.r = col[k].r; c.g = col[k].g; c.b = col[k].b;
      bits = dac_bits;
      gamma_correct(&remap_obj, &c, &bits);
      shift = 16 - bits;
      xcolor[i].flags = DoRed | DoGreen | DoBlue;
      xcolor[i].pixel = col[k].index;
      xcolor[i].red   = c.r << shift;
      xcolor[i].green = c.g << shift;
      xcolor[i].blue  = c.b << shift;
#if 0
      X_printf(
        "X: refresh_private_palette: color 0x%02x (0x%02x 0x%02x 0x%02x) -> (0x%04x 0x%04x 0x%04x)\n",
        (unsigned) col[k].index, (unsigned) c.r, (unsigned) c.g, (unsigned) c.b,
        (unsigned) xcolor[i].red, (unsigned) xcolor[i].green, (unsigned) xcolor[i].blue
      );
#else
      X_printf("X: refresh_private_palette: color 0x%02x\n", (unsigned) col[k].index);
#endif
      i++;
    }
    else {
      X_printf("X: refresh_private_palette: color 0x%02x not updated\n", (unsigned) col[k].index);
    }
  }

  if(graphics_cmap && i) XStoreColors(display, graphics_cmap, xcolor, i);
}


/*
 * Create an image. Will use shared memory, if available.
 * The image is used in graphics modes only.
 */
void create_ximage()
{
#ifdef HAVE_MITSHM
  if(shm_ok) {
    ximage = XShmCreateImage(
      display,
      visual,
      DefaultDepth(display,DefaultScreen(display)),
      ZPixmap, NULL,
      &shminfo,
      w_x_res, w_y_res
    );

    if(ximage == NULL) {
      X_printf("X: XShmCreateImage() failed\n");
      shm_ok = 0;
    }
    else {
      shminfo.shmid = shmget(IPC_PRIVATE, ximage->bytes_per_line * w_y_res, IPC_CREAT | 0777);
      if(shminfo.shmid < 0) {
        X_printf("X: shmget() failed\n");
        XDestroyImage(ximage);
        ximage = NULL;
        shm_ok = 0;
      }
      else {
        shminfo.shmaddr = (char *) shmat(shminfo.shmid, 0, 0);
        if(shminfo.shmaddr == ((char *) -1)) {
          X_printf("X: shmat() failed\n");
          XDestroyImage(ximage);
          ximage = NULL;
          shm_ok = 0;
        }
        else {
          shminfo.readOnly = False;
          XShmAttach(display, &shminfo);
          shmctl(shminfo.shmid, IPC_RMID, 0 );
          ximage->data = shminfo.shmaddr;
          XSync(display, False);	/* MIT-SHM will typically fail here... */
        }
      }
    }
  }
  if(!shm_ok)
#endif
  {
    ximage = XCreateImage(
      display,
      visual,
      DefaultDepth(display,DefaultScreen(display)),
      ZPixmap, 0, NULL,
      w_x_res, w_y_res,
      32, 0
    );
    if(ximage != NULL) {
      ximage->data = malloc(ximage->bytes_per_line * w_y_res);
      if(ximage->data == NULL) {
        X_printf("X: failed to allocate memory for XImage of size %d x %d\n", w_x_res, w_y_res);
      }
    }
    else {
      X_printf("X: failed to create XImage of size %d x %d\n", w_x_res, w_y_res);
    }
  }
  XSync(display, False);
}


/*
 * Destroy an image.
 */
void destroy_ximage()
{
  if(ximage == NULL) return;

#ifdef HAVE_MITSHM
  if(shm_ok) XShmDetach(display, &shminfo);
#endif

  XDestroyImage(ximage);

#ifdef HAVE_MITSHM
  if(shm_ok) shmdt(shminfo.shmaddr);
#endif

  ximage = NULL;
}


/*
 * Display part of an image.
 */
void put_ximage(int x, int y, unsigned width, unsigned height)
{
#ifdef HAVE_MITSHM
  if(shm_ok)
    XShmPutImage(display, drawwindow, gc, ximage, x, y, x, y, width, height, True);
  else
#endif /* HAVE_MITSHM */
    XPutImage(display, drawwindow, gc, ximage, x, y, x, y, width, height);
}


/*
 * Resize an image.
 */
void resize_ximage(unsigned width, unsigned height)
{
  X_printf("X: resize_ximage %d x %d --> %d x %d\n", w_x_res, w_y_res, width, height);
  destroy_ximage();
  w_x_res = width;
  w_y_res = height;
  create_ximage();
  remap_obj.dst_resize(&remap_obj, width, height, ximage->bytes_per_line);
  remap_obj.dst_image = ximage->data;
}

/*
 * Resize the window to given (*) size and lock it at that size
 * In text mode, you have to resize the mapper, too
 * ((*): given size is size of the embedded image)
 */
static void lock_window_size(unsigned wx_res, unsigned wy_res)
{
  XSizeHints sh;
  int x_fill, y_fill;

  sh.width = sh.min_width = sh.max_width = wx_res;
  sh.height = sh.min_height = sh.max_height = wy_res;

  sh.flags = PSize  | PMinSize | PMaxSize;
  if(config.X_fixed_aspect || config.X_aspect_43) sh.flags |= PAspect;
  if (use_bitmap_font) {
    sh.flags |= PResizeInc;
    sh.max_width = 32767;
    sh.max_height = 32767;
    sh.min_width = 0;
    sh.min_height = 0;
    sh.width_inc = 1;
    sh.height_inc = 1;
  }
  sh.min_aspect.x = w_x_res;
  sh.min_aspect.y = w_y_res;
  sh.max_aspect = sh.min_aspect;
  XSetNormalHints(display, normalwindow, &sh);
  XSync(display, False);

  x_fill = w_x_res;
  y_fill = w_y_res;
  if (mainwindow == fullscreenwindow)
    X_vidmode(x_res, y_res, &x_fill, &y_fill);

  XResizeWindow(display, mainwindow, x_fill, y_fill);

  if(vga.mode_class == TEXT && !use_bitmap_font) {
    x_fill = w_x_res;
    y_fill = w_y_res;
  }

  XResizeWindow(display, drawwindow, x_fill, y_fill);
  X_printf("Resizing our window to %dx%d image\n", x_fill, y_fill);

  if (use_bitmap_font) {
    resize_text_mapper(ximage_mode);
    resize_ximage(x_fill, y_fill);    /* destroy, create, dst-map */
    *remap_obj.dst_color_space = X_csd;
  }
}

/* 
 * DANG_BEGIN_FUNCTION X_set_videomode
 *
 * description:
 * This is the interface function called by the video subsystem
 * to set a video mode.
 *
 * NOTE: The actual mode is taken from the global variable "video_mode".
 *
 * description:
 * Set the video mode.
 * If mode_class is -1, this will only reinitialize the current mode.
 * The other arguments are ignored in this case.
 *
 * DANG_END_FUNCTION
 */
int X_set_videomode(int mode_class, int text_width, int text_height)
{
  int mode = video_mode;
  XSizeHints sh; /* for graphics modes, text size locking is above */
#ifdef X_USE_BACKING_STORE
  XSetWindowAttributes xwa;
#endif

  if(mode_class != -1) {	/* tell vgaemu we're going to another mode */
    if(!vga_emu_setmode(mode, text_width, text_height)) {
      v_printf("vga_emu_setmode(%d, %d, %d) failed\n",
               mode, text_width, text_height);
      return 0;
    } else if (use_bitmap_font) {
      font_width = vga.char_width;
      font_height = vga.char_height;
    }
  }
                               
  X_printf("X: X_setmode: %svideo_mode 0x%x (%s), size %d x %d (%d x %d pixel)\n",
    mode_class != -1 ? "" : "re-init ",
    (int) mode, vga.mode_class ? "GRAPH" : "TEXT",
    vga.text_width, vga.text_height, vga.width, vga.height
  );

  if(X_unmap_mode != -1 && (X_unmap_mode == vga.mode || X_unmap_mode == vga.VESA_mode)) {
    XUnmapWindow(display, drawwindow);
    XUnmapWindow(display, mainwindow);
    X_unmap_mode = -1;
  }

  destroy_ximage();

  mouse_x = mouse_y = 0;

#ifdef X_USE_BACKING_STORE
  /*
   * We use it only in text modes; in graphics modes we are fast enough and
   * it would likely only slow down the whole thing. -- sw
   */
  if(vga.mode_class == TEXT && !use_bitmap_font) {
    xwa.backing_store = Always;
    xwa.backing_planes = -1;
    xwa.save_under = True;
  }
  else {	/* GRAPH */
    xwa.backing_store = NotUseful;
    xwa.backing_planes = 0;
    xwa.save_under = False;
  }

  XChangeWindowAttributes(display, drawwindow, CWBackingStore | CWBackingPlanes | CWSaveUnder, &xwa);
#endif

  if(vga.mode_class == TEXT) {
    XSetWindowColormap(display, drawwindow, text_cmap);

    X_reset_redraw_text_screen();

    dac_bits = vga.dac.bits;

    if (!use_bitmap_font) {
      w_x_res = x_res = vga.text_width * font_width;
      w_y_res = y_res = vga.text_height * font_height;
    } else {
      font_width = vga.char_width;
      font_height = vga.char_height;
      x_res = vga.width;
      w_x_res = (x_res <= 320) ? (2 * x_res) : x_res;
      y_res = vga.height;
      w_y_res = (y_res <= 240) ? (2 * y_res) : y_res;
    }

    saved_w_x_res = w_x_res;
    saved_w_y_res = w_y_res;
    lock_window_size(w_x_res, w_y_res);
    if(mainwindow == fullscreenwindow) {
      X_vidmode(x_res, y_res, &w_x_res, &w_y_res);
    }
    if (!use_bitmap_font) {
      w_x_res = saved_w_x_res;
      w_y_res = saved_w_y_res;
    }
  }
  else {	/* GRAPH */

    if(!have_true_color) {
      XSetWindowColormap(display, drawwindow, graphics_cmap);
    }

    dac_bits = vga.dac.bits;

    x_res = vga.width;
    y_res = vga.height;

    get_mode_parameters(&w_x_res, &w_y_res, ximage_mode, &veut);
    if(mainwindow == fullscreenwindow) {
      saved_w_x_res = w_x_res;
      saved_w_y_res = w_y_res;
      X_vidmode(x_res, y_res, &w_x_res, &w_y_res);
    }

    create_ximage();

    remap_obj.dst_image = ximage->data;
    *remap_obj.dst_color_space = X_csd;
    remap_obj.dst_resize(&remap_obj, w_x_res, w_y_res, ximage->bytes_per_line);

    sh.width = w_x_res;
    sh.height = w_y_res;
    if(!(remap_obj.state & ROS_SCALE_ALL)) {
      sh.width_inc = x_res;
      sh.height_inc = y_res;
    }
    else {
      sh.width_inc = 1;
      sh.height_inc = 1;
    }
    sh.min_aspect.x = w_x_res;
    sh.min_aspect.y = w_y_res;
    sh.max_aspect = sh.min_aspect;

    if(remap_obj.state & ROS_SCALE_ALL) {
      sh.min_width = 0;
      sh.min_height = 0;
    }
    else {
      sh.min_width = w_x_res;
      sh.min_height = w_y_res;
    }
    if(remap_obj.state & ROS_SCALE_ALL) {
      sh.max_width = 32767;
      sh.max_height = 32767;
    }
    else if(remap_obj.state & ROS_SCALE_2) {
      sh.max_width = x_res << 1;
      sh.max_height = y_res << 1;
    }
    else {
      sh.max_width = w_x_res;
      sh.max_height = w_y_res;
    }

    sh.flags = PResizeInc | PSize  | PMinSize | PMaxSize;
    if(config.X_fixed_aspect || config.X_aspect_43) sh.flags |= PAspect;

    XSetNormalHints(display, normalwindow, &sh);
    XResizeWindow(display, mainwindow, w_x_res, w_y_res);
    XResizeWindow(display, drawwindow, w_x_res, w_y_res);
  }

  /* unconditionally update the palette */

  if(X_map_mode != -1 && (X_map_mode == vga.mode || X_map_mode == vga.VESA_mode)) {
    XMapWindow(display, mainwindow);
    XMapWindow(display, drawwindow);
    X_map_mode = -1;
  }

  return 1;
}

/*
 * Resize the X display to the appropriate size.
 */
void X_resize_text_screen() 
{
  if (!use_bitmap_font) {
    w_x_res = x_res = vga.text_width * font_width;
    w_y_res = y_res = vga.text_height * font_height;
  } else {
    font_width = vga.char_width;
    font_height = vga.char_height;
    x_res = vga.width;
    w_x_res = (x_res <= 320) ? (2 * x_res) : x_res;
    y_res = vga.height;
    w_y_res = (y_res <= 240) ? (2 * y_res) : y_res;
  }
  saved_w_x_res = w_x_res;
  saved_w_y_res = w_y_res;
  
  lock_window_size(w_x_res, w_y_res);
 
  X_redraw_text_screen();
}

/*
 * Change to requested video mode or the closest greater one.
 */
static void X_vidmode(int w, int h, int *new_width, int *new_height)
{
  int nw, nh, dw, dh, mx, my, shift_x, shift_y;
  
  nw = dw = DisplayWidth(display, screen);
  nh = dh = DisplayHeight(display, screen);
  
#ifdef HAVE_XVIDMODE
  if (xf86vm_ok) {
    static XF86VidModeModeLine vidmode_modeline;
    static int viewport_x, viewport_y, dotclock;
    int vx = 0, vy = 0;
    int i, j, restore_dotclock = 0;

    if (w == -1 && h == -1) { /* need to perform reset to windowed mode */
      w = vidmode_modeline.hdisplay;
      h = vidmode_modeline.vdisplay;
      vx = viewport_x;
      vy = viewport_y;
      restore_dotclock = 1;
    } else if (mainwindow != fullscreenwindow) {
      XF86VidModeGetModeLine(display,screen,&dotclock,&vidmode_modeline);
      XF86VidModeGetViewPort(display,screen,&viewport_x,&viewport_y);
      mainwindow = fullscreenwindow;
    }
    j = -1;
    for (i=0; i<modecount; i++) {
      if ((vidmode_modes[i]->hdisplay >= w) && 
          (vidmode_modes[i]->vdisplay >= h) &&
          (vidmode_modes[i]->hdisplay <= nw) &&
          (vidmode_modes[i]->vdisplay <= nh) &&
	  (!restore_dotclock || vidmode_modes[i]->dotclock == dotclock) &&
	  ( j == -1 ||
	    vidmode_modes[i]->dotclock >= vidmode_modes[j]->dotclock ||
	    vidmode_modes[i]->hdisplay != nw ||
	    vidmode_modes[i]->vdisplay != nh)) {
        nw = vidmode_modes[i]->hdisplay;
        nh = vidmode_modes[i]->vdisplay;
        j = i;
      }
    }
    if (j == -1) {
      error("X: vidmode for (%d,%d) not found!\n", w, h);
      *new_width = w;
      *new_height = h;
      return;
    }

    X_printf("X: vidmode asking for (%d,%d); setting (%d,%d)\n", w, h, nw, nh);
    XF86VidModeSwitchToMode(display,screen,vidmode_modes[j]);
    XF86VidModeSetViewPort (display,screen,vx,vy);
  }
#endif

  if (mainwindow == normalwindow) {
    nw = w_x_res;
    nh = w_y_res;
  }
          
  mx = min(mouse_x, nw - 1);
  my = min(mouse_y, nh - 1);
  shift_x = 0;
  shift_y = 0;
  if(vga.mode_class == TEXT && !use_bitmap_font) {
    shift_x = (nw - w_x_res)/2;
    shift_y = (nh - w_y_res)/2;
    XMoveWindow(display, drawwindow, shift_x, shift_y);
  }

  if (!grab_active && (mx != 0 || my != 0) && have_focus)
    XWarpPointer(display, None, drawwindow, 0, 0, 0, 0, mx, my);
  *new_width = nw;
  *new_height = nh;
}

/*
 * Sync prev_screen & screen_adr.
 */
void X_reset_redraw_text_screen()
{
  if(!is_mapped) return;
  reset_redraw_text_screen();
}

void X_redraw_text_screen()
{
  if(!is_mapped) return;
  redraw_text_screen();
}

int X_update_screen()
{
  if(vga.reconfig.re_init) {
    vga.reconfig.re_init = 0;
    dirty_all_video_pages();
    dirty_all_vga_colors();
    X_set_videomode(-1, 0, 0);
  }
  return is_mapped ? update_screen(&veut) : 0;
}


/*
 * Load the mouse cursor shapes.
 */
void load_cursor_shapes()
{
  Colormap cmap;
  XColor fg, bg;
  Font cfont;

  /* Use a white on black cursor as the background is normally dark. */
  cmap = DefaultColormap(display, screen);
  XParseColor(display, cmap, "white", &fg);
  XParseColor(display, cmap, "black", &bg);

  cfont = XLoadFont(display, "cursor");
  if (!cfont) {
    error("X: Unable to open font \"cursor\", aborting!\n");
    leavedos(99);
  }

#if CONFIG_X_MOUSE
  X_standard_cursor = XCreateGlyphCursor(
    display, cfont, cfont, 
    XC_top_left_arrow, XC_top_left_arrow+1, &fg, &bg
  );

  X_mouse_nocursor = create_invisible_cursor();
#endif /* CONFIG_X_MOUSE */

  XUnloadFont(display, cfont);
}


/*
 * Makes an invisible cursor (to be used when no cursor should be drawn).
 */
Cursor create_invisible_cursor()
{
  Pixmap mask;
  Cursor cursor;
  static char map[16] = {0};

  mask = XCreatePixmapFromBitmapData(display, drawwindow, map, 1, 1, 0, 0, 1);
  cursor = XCreatePixmapCursor(display, mask, mask, (XColor *) map, (XColor *) map, 0, 0);
  XFreePixmap(display, mask);
  return cursor;
}


/*
 * Redraw the cursor if it's necessary.
 * Do nothing in graphics modes.
 */
void X_update_cursor() 
{
  /* no hardware cursor emulation in graphics modes (erik@sjoerd) */
  if(vga.mode_class == GRAPH) return;
  update_cursor();
}


#if CONFIG_X_MOUSE
/* 
 * DANG_BEGIN_FUNCTION set_mouse_position
 *
 * description:
 * Place the mouse on the right position.
 * 
 * DANG_END_FUNCTION
 */ 
void set_mouse_position(int x, int y)
{
  int dx, dy, center_x, center_y, x0, y0;

  /* this is to ignore the event generated by XWarpPointer if !grab_active,
     which may give a mismatch in x and y */
  if(mouse_warped) {
    mouse_warped = 0;
    return;
  }

  x0 = x;
  y0 = y;

  if(grab_active) {
    center_x = w_x_res >> 1;
    center_y = w_y_res >> 1;
    if (x == center_x && y == center_y) return;  /* assume pointer warp event */
    dx = x - center_x;
    dy = y - center_y;
    x0 = dx + mouse_x;
    y0 = dy + mouse_y;
    XWarpPointer(display, None, drawwindow, 0, 0, 0, 0, center_x, center_y);
    mouse_move_relative(dx, dy);
  } else if(snap_X) {
    /*
     * win31 cursor snap kludge, we temporary force the DOS cursor to the
     * upper left corner (0,0). If we after that release snapping,
     * normal X-events will move the cursor to the exact position. (Hans)
     */
     x0 = y0 = 0;
     dx = -3 * x_res; dy = -3 * y_res;		/* enough ??? -- sw */
     mouse_move_relative(dx, dy);
     snap_X--;
  } else {
    mouse_move_absolute(x, y, w_x_res, w_y_res);
  }

  mouse_x = x0;
  mouse_y = y0;
}


/*
 * Appears to set the mouse buttons. Called from the
 * X event handler.
 */
void set_mouse_buttons(int state)
{
  mouse_move_buttons(state & Button1Mask, state & Button2Mask, state & Button3Mask);
}
#endif /* CONFIG_X_MOUSE */

/* do this even without CONFIG_XMOUSE; otherwise xterm mouse
   events may be tried... */
static int X_mouse_init(void)
{
  mouse_t *mice = &config.mouse;
  if (Video != &Video_X || !mice->intdrv)
    return FALSE;
  mice->type = MOUSE_X;
  mice->use_absolute = 1;
  mice->native_cursor = 0;	/* we have the X cursor */
  m_printf("MOUSE: X Mouse being set\n");
  return TRUE;
}

static struct mouse_client Mouse_X =  {
  "X",          /* name */
  X_mouse_init, /* init */
  NULL,		/* close */
  NULL,		/* run */
  X_set_mouse_cursor /* set_cursor */
};


/*
 * Doesn't really belong here!
 * It will go into a separate file. -- sw
 */

#define KDOS_CLOSE_MSG	1

void kdos_recv_msg(unsigned char *buf)
{
  fprintf(stderr, "got Msg %d\n", buf[0]);
}

void kdos_send_msg(unsigned char *buf)
{
  XEvent e;

  if(!kdos_client) return;

  e.xclient.type = ClientMessage;
  e.xclient.serial = 0;
  e.xclient.display = display;
  e.xclient.window = parentwindow;
  e.xclient.message_type = comm_atom;
  e.xclient.format = 8;
  memcpy(e.xclient.data.b, buf, sizeof e.xclient.data.b);

  XSendEvent(display, parentwindow, False, 0, &e);
}

void kdos_close_msg()
{
  unsigned char m[20] = { KDOS_CLOSE_MSG, };
  kdos_send_msg(m);
}

CONSTRUCTOR(static void init(void))
{
	if (Video) return;

	config.X = 1;	/* activate X mode if dosemu was */
	Video = &Video_X;
	register_keyboard_client(&Keyboard_X);
	register_mouse_client(&Mouse_X);
}
