;
;
;

name start

public mbi0

public _small_code_
public _cstart_

_TEXT    segment byte  public 'CODE'  use16
_TEXT    ends
CONST    segment byte  public 'DATA'  use16
CONST    ends
CONST2   segment byte  public 'DATA'  use16
CONST2   ends
_DATA    segment dword public 'DATA'  use16
_DATA    ends

DGROUP   group CONST,CONST2,_DATA

_TEXT    segment byte  public 'CODE'  use16

.286p

_cstart_:
         ret

_TEXT    ends


_DATA    segment dword public 'DATA'  use16

align 4

_small_code_  dd 0
; must be the last dword in the executable
mbi0          dd  0ffffffffh

_DATA    ends

         end
