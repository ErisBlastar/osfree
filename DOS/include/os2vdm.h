#define APIRET short
#define APIENTRY _System
#define CHAR char
#define VOID void
#define PVOID void far *
#define USHORT unsigned short
#define PSZ char far *
#define NULL (void *)0

/*
INT 21 U - OS/2 v1.x FAPI - OS/2 FILE SYSTEM JOIN/SUBST
        AH = 61h
        BP = 6467h ("dg")
        AL = function
            00h list (i.e. get)
            01h add
            02h delete
        BX = drive number
        CX = size of buffer
        SI = type (0002h JOIN, 0003h SUBST)
        ES:DI -> buffer
Return: CF clear if successful
            AX = 0000h
            ES:DI buffer filled, if appropriate
        CF set on error
            AX = error code
Notes:  used by JOIN and SUBST to communicate with the OS/2 file system
        for function 00h (list), the ES:DI buffer is filled with the ASCIZ
          JOIN/SUBST path or an empty string if the drive is not JOINed/SUBSTed
        also supported by OS/2 v2.0+ Virtual DOS Machines
*/

APIRET APIENTRY VdmJoinQuery(CHAR cDrive, USHORT usBufferSize, PVOID Buffer);
APIRET APIENTRY VdmJoinSet(CHAR cDrive, PSZ pszPath);
APIRET APIENTRY VdmJoinDelete(CHAR cDrive);

APIRET APIENTRY VdmSubstQuery(CHAR cDrive, USHORT usBufferSize, PVOID Buffer);
APIRET APIENTRY VdmSubstSet(CHAR cDrive, PSZ pszPath);
APIRET APIENTRY VdmSubstDelete(CHAR cDrive);

/*
INT 21 U - OS/2 v2.0+ Virtual DOS Machine - ENABLE AUTOMATIC TITLE SWITCH
        AH = 64h
        DX = 0000h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
Note:   if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
SeeAlso: AH=64h/DX=0001h,INT 21/AH=4Bh
*/

APIRET APIENTRY VdmEnableAutoTitle(VOID);

/*
--------O-2164--DX0001-----------------------
INT 21 U - OS/2 v2.0+ Virtual DOS Machine - SET SESSION TITLE
        AH = 64h
        DX = 0001h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
        ES:DI -> new ASCIZ title (max 12 char) or "" to restore default title
Note:   if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
SeeAlso: AH=64h/DX=0000h,AH=64h/DX=0002h,INT 15/AH=12h/BH=05h
*/

APIRET APIENTRY VdmSetTitle(PSZ pszTitle);

/*
--------O-2164--DX0002-----------------------
INT 21 U - OS/2 v2.0+ Virtual DOS Machine - GET SESSION TITLE
        AH = 64h
        DX = 0002h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
        ES:DI -> 13-byte buffer for current title
Return: buffer filled (single 00h if title never changed)
Note:   if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
SeeAlso: AH=64h/DX=0000h,AH=64h/DX=0001h,INT 15/AH=12h/BH=05h
*/

APIRET APIENTRY VdmQueryTitle(PSZ pszTitle);

/*
--------O-2164--DX0003-----------------------
INT 21 U - OS/2 v2.1 Virtual DOS Machine - GET LASTDRIVE
        AH = 64h
        DX = 0003h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
Return: AL = highest drive supported
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        used by WinOS2
        not supported by OS/2 Warp 3.0, check list of lists instead (see #01627)
SeeAlso: AH=52h
*/

APIRET APIENTRY VdmQueryLastDrive(CHAR cDrive);

/*
--------O-2164--DX0004-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - GET SIZE OF PTDA JFT
        AH = 64h
        DX = 0004h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
Return: AX = number of entries in OS/2 JFT for VDM
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        in an OS/2 VDM, the DOS Job File Table in the PSP contains an index
          into the OS/2 JFT in the Per-Task Data Area rather than an SFT index
          because the OS/2 SFT can contain more than 255 entries
--------O-2164--DX0005-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - GET SECOND SFT FLAGS WORD
        AH = 64h
        DX = 0005h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
        DI = DOS file handle
Return: AX = value of second flags word from OS/2 SFT entry for file
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        the OS/2 SFT has two flags words rather than DOS's one word, and this
          function provides access to the word which is not present in DOS
--------O-2164--DX0006-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - UNLOAD DOSKRNL SYMBOLS & LOAD PROGR
        AH = 64h
        DX = 0006h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
        ES:DI -> ASCIZ filespec
        DS = base address for loading
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        this function is only supported by the kernel debugging version of
          OS2KRNL
--------O-2164--DX0007-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - GET WinOS2 CALL GATE ADDRESS
        AH = 64h
        DX = 0007h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
Return: AX = call gate address
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        used by WinOS2 to make direct calls to OS2KRNL, bypassing the overhead
          of DOSKRNL
--------O-2164--DX0008-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - GET LOADING MESSAGE
        AH = 64h
        DX = 0008h (function number)
        CX = 636Ch (magic value, "cl")
        BX = 0000h (indicates special request)
Return: DS:DX -> '$'-terminated message "Loading.  Please wait."
Notes:  if CX is not 636Ch on entry, INT 21/AH=6Ch is invoked, because a bug
          in OS/2 1.x FAPI erroneously called AH=64h instead of AH=6Ch
        this function permits National Language Support for the initial message
          displayed while WinOS2 starts a full-screen session
--------O-2164--CX636C-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - OS/2 API support
        AH = 64h
        CX = 636Ch ("cl")
        BX = API ordinal (see #01747)
        other registers as appropriate for API call
Return: as appropriate for API call
SeeAlso: AH=64h/BX=0025h,AH=64h/BX=00B6h,AH=64h/BX=00CBh

(Table 01747)
Values for OS/2 API ordinal:
 0025h  DOS32StartSession
 0082h  DosGetCP
 00B6h  DosQFSAttach
 00BFh  DosEditName
 00CBh  DosForceDelete
 0144h  Dos32CreateEventSem
 0145h  Dos32OpenEvenSem
 0146h  Dos32CloseEventSem
 0147h  Dos32ResetEventSem
 0148h  Dos32PostEventSem
 0149h  Dos32WaitEventSem
 014Ah  Dos32QueryEventSem
 014Bh  Dos32CreateMutexSem
 014Ch  Dos32OpenMutexSem
 014Dh  Dos32CloseMutexSem
 014Eh  Dos32RequestMutexSem
 014Fh  Dos32ReleaseMutexSem
 0150h  Dos32QueryMutexSem
 0151h  Dos32CreateMuxWaitSem
 0152h  Dos32OpenMuxWaitSem
 0153h  Dos32CloseMuxWaitSem
 0154h  Dos32WaitMuxWaitSem
 0155h  Dos32AddMuxWaitSem
 0156h  Dos32DeleteMuxWaitSem
 0157h  Dos32QueryMuxWaitSem
--------O-2164--BX0025-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - OS/2 API DOS32StartSession
        AH = 64h
        BX = 0025h (API ordinal)
        CX = 636Ch ("cl")
        DS:SI -> STARTDATA structure (see #01748)
Return: AX = return code
SeeAlso: AH=64h/CX=636Ch,AH=64h/BX=00B6h

Format of OS/2 Virtual DOS Machine STARTDATA structure:
Offset  Size    Description     (Table 01748)
 00h    WORD    length of structure (must be 0018h,001Eh,0020h,0032h,or 003Ch)
 02h    WORD    relation of new process to caller (00h independent, 01h child)
 04h    WORD    fore/background (00h foreground, 01h background)
 06h    WORD    trace options (00h-02h, 00h = no trace)
 08h    DWORD   pointer to ASCIZ program title (max 62 chars) or 0000h:0000h
 0Ch    DWORD   pointer to ASCIZ program name (max 128 chars) or 0000h:0000h
 10h    DWORD   pointer to ASCIZ program args (max 144 chars) or 0000h:0000h
 14h    DWORD   "TermQ" (currently reserved, must be 00000000h)
 18h    DWORD   pointer to environment (max 486 bytes) or 0000h:0000h
 1Ch    WORD    inheritance (00h or 01h)
 1Eh    WORD    session type
                00h OS/2 session manager determines type (default)
                01h OS/2 full-screen
                02h OS/2 window
                03h PM
                04h VDM full-screen
                07h VDM window
 20h    DWORD   pointer to ASCIZ icon filename (max 128 chars) or 0000h:0000h
 24h    DWORD   "PgmHandle" (currently reserved, must be 00000000h)
 28h    WORD    "PgmControl"
 2Ah    WORD    initial column
 2Ch    WORD    initial row
 2Eh    WORD    initial width
 30h    WORD    initial height
 32h    WORD    reserved (0)
 34h    DWORD   "ObjectBuffer" (currently reserved, must be 00000000h)
 38h    DWORD   "ObjectBufferLen" (currently reserved, must be 00000000h)
--------O-2164--BX00B6-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - OS/2 API DosQFSAttach
        AH = 64h
        BX = 00B6h (API ordinal)
        CX = 636Ch (magic value "cl")
        DS = user's data segment
        ES:DI -> FSQAttachStruc (see #01749)
Return: CF set on error
            AX = error code (see #01680 at AH=59h/BX=0000h)
        CF clear if successful
            AX = 0000h
            data buffer filled
SeeAlso: AH=64h/CX=636Ch

Format of OS/2 Virtual DOS Machine FSQAttachStruc:
Offset  Size    Description     (Table 01749)
 00h    DWORD   reserved
 04h    DWORD   pointer to the offset of the data buffer length
 08h    DWORD   pointer to the offset of the data buffer
 0Ch    WORD    FSA Info level
 0Eh    WORD    ordinal index into table
 10h    DWORD   pointer to the offset of the device name
Notes:  The segment value of the buffer, buffer length, and device
          name MUST all be the same.  It is defined on entry in the DS
          register.  The details of each info level are defined in the
          OS/2 CP Reference.
--------O-2164--BX00CB-----------------------
INT 21 U - OS/2 v2.1+ Virtual DOS Machine - OS/2 API DosForceDelete
        AH = 64h
        BX = 00CBh (API ordinal)
        CX = 636Ch (magic value "cl")
        DS:DX -> ASCIZ filename
Return: CF clear if successful
            AX destroyed
        CF set on error
            AX = error code (02h,03h,05h) (see #01680 at AH=59h/BX=0000h)
Desc:   delete a file without saving it to the undelete directory
SeeAlso: AH=41h,AH=64h/CX=636Ch
-----
INT 21 U - OS/2 v2.0 - "DosOpen2"
        AX = 6C01h
        BL = open mode as in AL for normal open (see also AH=3Dh)
            bit 7: inheritance
            bits 4-6: sharing mode
            bit 3 reserved
            bits 0-2: access mode
        BH = flags
            bit 6 = auto commit on every write (see also AH=68h)
            bit 5 = return error rather than doing INT 24h
        CX = create attribute (see #01769)
        DL = action if file exists/does not exist (see #01770)
        DH = 00h (reserved)
        DS:SI -> ASCIZ file name
        ES:DI -> EAOP structure
Return: CF set on error
           AX = error code (see #01680 at AH=59h/BX=0000h)
        CF clear if successful
           AX = file handle
           CX = status (see #01768)
Note:   this function is virtually identical to AX=6C00h, but supports OS/2's
          extended attributes
SeeAlso: AX=5704h,AX=6C00h,AH=6Fh"OS/2"
----
--------O-216D-------------------------------
INT 21 U - OS/2 v1.x FAPI - "DosMkDir2"
        AH = 6Dh
        ???
Return: ???
Desc:   create a new directory, with extended attribute information
Note:   also supported by OS/2 v2.0+ Virtual DOS Machines
BUG:    does not work under OS/2 v2.0 because MVDM does not translate the
          real-mode segment pointer in the Extended Attribute structure
          (see #01673) into a protected-mode selector; use AH=39h followed by
          AX=5703h instead
SeeAlso: AH=39h,AX=5702h"OS/2",AX=5703h"OS/2"
-----
--------O-216E-------------------------------
INT 21 U - OS/2 v1.x FAPI - "DosEnumAttrib"
        AH = 6Eh
        DS:SI -> parameter packet (see #01772)
Return: CF clear if successful
            AX = 0000h
            DS:SI buffer updated
        CF set on error
            AX = error code
Note:   also supported by OS/2 v2.0+ Virtual DOS Machines
SeeAlso: AX=5703h,AH=6Fh"OS/2",INT 2F/AX=112Dh

Format of OS/2 DosEnumAttrib parameter packet:
Offset  Size    Description     (Table 01772)
 00h    DWORD   reserved (0)
 04h    DWORD   info level (always 00000001h)
 08h    DWORD   (call) number of entries requested
                (ret) actual number of entries returned
 0Ch    DWORD   length of buffer
 10h    DWORD   pointer to buffer for results
 14h    DWORD   number of first entry to return
 18h    DWORD   -> file handle or ASCIZ pathname
 1Ch    WORD    flag: 00h = previous field is file handle, 01h = pathname
--------D-216F00-----------------------------
--------O-216F00-----------------------------
INT 21 U - OS/2 v1.x FAPI - "DosQMaxEASize" - GET MAXIMUM SIZE OF EXTENDED ATTR
        AX = 6F00h
        DS:SI -> DWORD buffer for maximum size of an extended attribute
Return: CF clear if successful
            AX = 0000h
            buffer filled
        CF set on error
            AX = error code
Note:   also supported by OS/2 v2.0+ Virtual DOS Machines
SeeAlso: AX=5703h,AX=6C01h,AH=6Eh"OS/2"
--------
--------N-215F32-----------------------------
INT 21 u - Named Pipes - LOCAL DosQNmPipeInfo
        AX = 5F32h
        BX = handle
        CX = size of _PIPEINFO structure
        DX = level (must be 0001h)
        DS:SI -> _PIPEINFO structure (see #01701)
Return: CF clear if successful
            _PIPEINFO structure filled in
        CF set on error
            AX = error code
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F33h,AX=5F34h

Format of Named Pipes _PIPEINFO structure:
Offset  Size    Description     (Table 01701)
 00h    WORD    size of outgoing buffer
 02h    WORD    size of incoming buffer
 04h    BYTE    maximum number of instances allowed
 05h    BYTE    current number of instances
 06h    BYTE    length of the name (including terminating NUL)
 07h  N BYTEs   name
--------N-215F33-----------------------------
INT 21 u - Named Pipes - LOCAL DosQNmPHandState
        AX = 5F33h
        BX = handle
Return: CF clear if successful
            AH = pipe mode bit mask (see #01702)
            AL = maximum number of instances
        CF set on error
            AX = error code
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F32h,AX=5F34h

Bitfields for Named Pipes pipe mode:
Bit(s)  Description     (Table 01702)
 7      set if nonblocking, clear if blocking
 6      set if server end, clear if client end
 2      set if write in message mode, clear if write in byte mode
 0      set if read in message mode, clear if read in byte mode
--------N-215F34-----------------------------
INT 21 u - Named Pipes - LOCAL DosSetNmPHandState
        AX = 5F34h
        BX = handle
        CX = pipe mode bit mask
            bit 15: set if nonblocking, clear if blocking
            bit  8: set if read in message mode, clear if read in byte mode
Return: CF clear if successful
        CF set if error
            AX = error code
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F32h,AX=5F33h,AX=5F36h
--------N-215F35-----------------------------
INT 21 u - Named Pipes - LOCAL DosPeekNmPipe
        AX = 5F35h
        BX = handle
        CX = buffer length
        DS:SI -> buffer
Return: CF set on error
            AX = error code
        CF clear if successful (LAN Manager v1-v2)
        AX = 0000h if successful (LAN Manager 3.x)
        ---if successful---
            CX = bytes read
            SI = bytes left in the pipe
            DX = bytes left in the current message
            AX = pipe status (v1-v2) (see #01703)
            DI = pipe status (v3.x)
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F38h,AX=5F39h,AX=5F51h

(Table 01703)
Values for pipe status:
 0001h  disconnected
 0002h  listening
 0003h  connected
 0004h  closing
--------N-215F36-----------------------------
INT 21 u - Named Pipes - LOCAL DosTransactNmPipe
        AX = 5F36h
        BX = handle
        CX = in buffer length
        DS:SI -> in buffer
        DX = out buffer length
        ES:DI -> out buffer
Return: CF clear if successful
            CX = bytes read
        CF set on error
            AX = error code
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F34h,AX=5F37h
--------N-215F37-----------------------------
INT 21 u - Named Pipes - DosCallNmPipe
        AX = 5F37h
        DS:SI -> DosCallNmPipe stack frame (see #01704)
Return: CF clear if successful
            CX = bytes read
        CF set on error
            AX = error code
Note:   this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F36h,AX=5F38h

Format of Named Pipes DosCallNmPipe stack frame:
Offset  Size    Description     (Table 01704)
 00h    DWORD   timeout
 04h    DWORD   -> bytes read WORD (not used!)
 08h    WORD    out buffer length
 0Ah    DWORD   address of out buffer
 0Eh    WORD    in buffer length
 10h    DWORD   address of in buffer
 14h    DWORD   address of pipe name
--------N-215F38-----------------------------
INT 21 u - Named Pipes - LOCAL DosWaitNmPipe - AWAIT AVAIL. OF PIPE INSTANCE
        AX = 5F38h
        DS:DX -> pipe name
        BX:CX = timeout value
Return: CF clear if successful
        CF set if error
            AX = error code
Notes:  when a client gets a return code of ERROR_PIPE_BUSY on attempting to
          open a pipe, it should issue this call to wait until the pipe
          instance becomes available again; on return from this call, the
          client must attempt to open the pipe once again
        this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
SeeAlso: AX=5F37h,AX=5F39h
--------N-215F39-----------------------------
INT 21 U - Named Pipes - LOCAL DosRawReadNmPipe
        AX = 5F39h
        BX = handle
        CX = buffer length
        DS:DX -> buffer
Return: CF clear if successful
            CX = bytes read
        CF set if error
            AX = error code
Notes:  this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
        not documented in the LAN Manager Toolkit
SeeAlso: AX=5F35h,AX=5F3Ah,INT 2F/AX=1186h
--------N-215F3A-----------------------------
INT 21 U - Named Pipes - LOCAL DosRawWriteNmPipe
        AX = 5F3Ah
        BX = handle
        CX = buffer length
        DS:DX -> buffer
Return: CF clear if successful
            CX = bytes written
        CF set if error
            AX = error code
Notes:  this function was introduced by LAN Manager but is also supported by
          the Novell DOS Named Pipe Extender, Banyan VINES, OS/2 Virtual DOS
          Machines, and others
        not documented in the LAN Manager Toolkit
SeeAlso: AX=5F39h,AX=5F3Bh,INT 2F/AX=118Fh
*/

