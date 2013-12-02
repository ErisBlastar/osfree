;
; filesys.asm:
; filesystem access realmode (mu_*-) functions
;

name filesys

ifndef STAGE1_5

public mu_Open
public mu_Read
public mu_Close
public mu_Terminate

extrn printhex4         :far
extrn rs                :dword
extrn base              :dword

endif

public preldr_ds
public preldr_ss_sp
public preldr_es

extrn filemax           :dword
extrn call_pm           :near
extrn freeldr_open      :near
extrn freeldr_read      :near

ifndef STAGE1_5
extrn freeldr_seek      :near
endif

extrn freeldr_close     :near

.386p

include fsd.inc

switch_to_preldr macro
        push ax
        mov  ax,  ds
        mov  cs:ldr_ds, ax
        mov  ax,  es
        mov  cs:ldr_es, ax
        mov  ax, sp
        inc  ax                                  ; skip ax in stack
        inc  ax                                  ;
        mov  word ptr cs:ldr_ss_sp, ax
        mov  ax, ss
        mov  word ptr cs:ldr_ss_sp + 2, ax

        mov  ax,  cs:preldr_ds
        mov  ds,  ax
        mov  ax,  cs:preldr_es
        mov  es,  ax
        pop  ax
        lss  sp,  cs:preldr_ss_sp
endm

switch_to_ldr macro
        push ax
        mov  ax, ds
        mov  cs:preldr_ds, ax
        mov  ax, es
        mov  cs:preldr_es, ax
        mov  ax, sp
        inc  ax                                  ; skip ax in stack
        inc  ax                                  ;
        mov  word ptr cs:preldr_ss_sp, ax
        mov  ax, ss
        mov  word ptr cs:preldr_ss_sp + 2, ax

        mov  ax, cs:ldr_ds
        mov  ds, ax
        mov  ax, cs:ldr_es
        mov  es, ax
        pop  ax
        lss  sp, cs:ldr_ss_sp
endm

_TEXT16 segment dword public 'CODE' use16


ifndef STAGE1_5

;
; Open file using MicroFSD
;
; unsigned short __cdecl
; mu_Open(char far *pName,
;         unsigned long far *pulFileSize);
;

mu_Open proc far
        push bp
        mov  bp, sp

        push ebx
        push edx

        ; char far *pName
        mov  ebx, dword ptr [bp + 06h]

        switch_to_preldr

        ; convert far ptr in ebx to
        ; FLAT ptr.
        mov  eax, ebx
        shr  eax, 16
        shl  eax, 4
        and  ebx, 0ffffh
        ;and  ecx, 0fffffh
        add  ebx, eax

        ; setup 'shift' to pre-loader
        mov  eax, ds:[0x8]
        sub  eax, ds:[0x3c]
        sub  eax, PRELDR_BASE
        neg  eax
        ;mov  edi, eax

        ; switch to PM and call muOpen
        add  eax, offset _TEXT:muOpen
        ;mov  eax, offset _TEXT:muOpen
        ;add  eax, edi

        push eax
        call call_pm
        add  sp, 4

        switch_to_ldr

        cmp  ebx, 0
        jz   noerr1
err1:
        ;xor  dx, dx
        mov  ax, 1
        jmp  nok1
noerr1:
        xor  ax, ax
nok1:
        push es
        push di
        les  di, dword ptr [bp + 0ah]
        mov  es:[di], edx ; size
        pop  di
        pop  es

        pop  edx
        pop  ebx

        ;xor  dx, dx

        pop  bp

        retf
mu_Open endp

;
; unsigned long  __cdecl
; mu_Read(long loffseek,
;         char far *pBuf,
;         unsigned long cbBuf);
;

mu_Read proc far
        push bp
        mov  bp, sp

        push ebx
        push ecx
        push edx

        ; long loffseek
        mov  ebx, dword ptr [bp + 06h]
        ; char far *pBuf
        mov  edx, dword ptr [bp + 0ah]
        ; unsigned long cbBuf
        mov  ecx, dword ptr [bp + 0eh]

        switch_to_preldr

        ; convert a far ptr in edx
        ; to a FLAT ptr
        ;mov  eax, esi
        mov  eax, edx
        shr  edx, 16
        shl  edx, 4
        and  eax, 0ffffh
        add  edx, eax

        ; setup 'shift' to pre-loader
        mov  eax, ds:[0x8]
        sub  eax, ds:[0x3c]
        sub  eax, PRELDR_BASE
        neg  eax
        ;mov  edi, eax

        ;push edx

        ;mov  edx, esi

        ; switch to PM and call muOpen
        add  eax, offset _TEXT:muRead
        ;mov  eax, offset _TEXT:muRead
        ;add  eax, edi
        push eax
        call call_pm
        add  sp, 4

        ;pop  edx

        switch_to_ldr

        pop  edx
        pop  ecx

        ; ebx (count of bytes read) -> dx:ax
        mov  ax, bx
        and  eax, 0ffffh
        shr  ebx, 16
        mov  dx, bx

        pop  ebx

        pop  bp

        retf
mu_Read endp

;
; void __cdecl
; mu_Close(void);
;

mu_Close proc far
        switch_to_preldr

        ; setup 'shift' to pre-loader
        mov  eax, ds:[0x8]
        sub  eax, ds:[0x3c]
        sub  eax, PRELDR_BASE
        neg  eax
        ;mov  edi, eax

        ; switch to PM and call muOpen
        add eax, offset _TEXT:muClose
        ;mov  eax, offset _TEXT:muClose
        ;add  eax, edi
        push eax
        call call_pm
        add  sp, 4

        switch_to_ldr

        retf
mu_Close endp

;
; void __cdecl
; mu_Terminate(void);
;

mu_Terminate proc far
        retf
mu_Terminate endp

ldr_ds       dw 0
ldr_ss_sp    dd 0
ldr_es       dw 0

endif

preldr_ds    dw 0
preldr_ss_sp dd 0
preldr_es    dw 0


_TEXT16 ends

_TEXT   segment dword public 'CODE' use32


ifndef STAGE1_5

muOpen proc near
       mov  eax, ebx
       call freeldr_open

       cmp  eax, 0
       jnz  noerr2
err2:
       mov  ebx, 1
       jmp  quit2
noerr2:
       xor  ebx, ebx
       mov  edx, offset _TEXT:filemax
       mov  edx, [edx]
quit2:
       ret
muOpen endp

muRead proc near
       push edx
       push ecx

       mov  eax, ebx
       call freeldr_seek

       cmp  ecx, 0
       jnz  later1
       mov  ecx, 0ffffffffh
later1:

       mov  eax, edx
       mov  edx, ecx
       call freeldr_read

       mov  ebx, eax

       pop  ecx
       pop  edx

       ret
muRead endp

muClose proc near
       call freeldr_close

       ret
muClose endp


endif


_TEXT   ends

        end
