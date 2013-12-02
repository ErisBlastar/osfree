;   $Id: cb_catch.asm 771 2004-02-01 13:55:39Z skaus $

;       ^Break signal catcher and initialization function.
;       Bases on CB_CATCH.ASM
;       It is activated only, if FreeCOM is active.
;       --> FreeCOM polls the ^Break state actively, so the ^C press is
;               recorded, but ignored otherwise

;       There are several possibility to communicate with FreeCOM, because
;       FreeCOM actively polls the ^Break status, this implementation
;       optimizes for the polling rather this handler.

;   $Log$
;   Revision 1.3  2004/02/01 13:52:18  skaus
;   add/upd: CVS $id$ keywords to/of files
;
;   Revision 1.2  2002/04/02 18:09:31  skaus
;   add: XMS-Only Swap feature (FEATURE_XMS_SWAP) (Tom Ehlert)
;
;   Revision 1.1  2001/04/12 00:09:06  skaus
;   chg: New structure
;   chg: If DEBUG enabled, no available commands are displayed on startup
;   fix: PTCHSIZE also patches min extra size to force to have this amount
;      of memory available on start
;   bugfix: CALL doesn't reset options
;   add: PTCHSIZE to patch heap size
;   add: VSPAWN, /SWAP switch, .SWP resource handling
;   bugfix: COMMAND.COM A:\
;   bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
;   add: command MEMORY
;   bugfix: runExtension(): destroys command[-2]
;   add: clean.bat
;   add: localized CRITER strings
;   chg: use LNG files for hard-coded strings (hangForEver(), init.c)
;       via STRINGS.LIB
;   add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
;   add: fixstrs.c: prompts & symbolic keys
;   add: fixstrs.c: backslash escape sequences
;   add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
;   chg: splitted code apart into LIB\*.c and CMD\*.c
;   bugfix: IF is now using error system & STRINGS to report errors
;   add: CALL: /N
;
;   Revision 1.1.1.1.4.2  2001/02/18 21:08:23  skaus
;   add: command WHICH
;   fix: BUILD.BAT and accompanying makefiles for TC++ v1.01
;
;   Revision 1.1.1.1.4.1  2001/02/18 17:59:36  skaus
;   bugfix: KSSF: restore parentPSP on exit
;   chg: Using STRINGS resource for all non-interactive messages
;   chg: moving all assembly files to NASM
;

include model.inc
include stuff.inc

;_DATA segment
;       EXTRN ctrlBreak_ :word
;_DATA ends

_TEXT segment word public 'CODE' use16
        public _initCBreak

        public cbreak_handler_
        public CBreakCounter_

_initCBreak:
       ;; At this point DS is the segment of _ctrlBreak
       mov WORD PTR CS:[?freecomSegment], ds
       ret

?freecomSegment DW 0
CBreakCounter_  DW 0

cbreak_handler_:
ifdef DEBUG
                dec BYTE [CS:strEnd]
                jz noRecurs
                inc BYTE [CS:strEnd]
                jmp short recurs

noRecurs:
                ;pushreg ds, dx, ax, bp
                push ds
                push dx
                push ax
                push bp

                mov dx, strBeg
                mov ax, cs
                mov ds, ax
                mov ah, 9
                int 21h
                inc BYTE PTR [strEnd]

                ;popreg ds, dx, ax, bp
                pop bp
                pop ax
                pop dx
                pop ds
endif

                ;; ^Break of COMAMND --> just set the variable
                inc WORD PTR CS:[CBreakCounter_]

recurs:
                clc                     ;; tell DOS to proceed
                retf 2

ifdef DEBUG
strBeg:
        db 0dh, 0ah, 'COMMAND: ^Break detected.  ', 0dh, 0ah, 0dh, 0ah, '$'
strEnd  db 1
endif

_TEXT ends

      end
