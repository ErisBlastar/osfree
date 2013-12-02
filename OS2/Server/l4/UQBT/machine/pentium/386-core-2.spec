#
# Copyright (C) 1998,2000, The University of Queensland
# Copyright (C) 2001, Sun Microsystems, Inc
#
# See the file "LICENSE.TERMS" for information on usage and
# redistribution of this file, and for a DISCLAIMER OF ALL
# WARRANTIES.
#

# This software may include third party software for which you may need to
# obtain permission from third parties.  You are solely responsible for
# obtaining any required permissions from such third parties.

# 386-core.spec (based on pentium-core.spec, NJMC v0.5)
# 19 Jan 98 - Cristina
#	removed all escape opcodes with bytes D8..DF as the first byte; those
#		742, 923, 951, 1011, 1050, and associated constructors.
# 22 Jan 98 - Cristina
#	added LEA and lfp constructors as they're missing (missing from original 
#		pentium spec also).
#	changed Mem for Eaddr in the constructors CALL.Ev^ov and CALL.Ep^ov 
#		as register indirections are allowed (i.e. mod = 3).
#	added LEAVE constructor 
# 29 Jan 98 - Mike
#	changed order of CS and IP in the call and jmp constructors
# 2 Feb 98 - Cristina
#	MOViw took a r32 instead of a r16.  Changed as per 386-names.spec
# 21 Oct 98 - Mike: lower case register names
# 03 Nov 98 - Mike: fixed LEA; Abs32 is now Mem (was Eaddr)
# 04 Nov 98 - Mike: changed SETb to Eaddr (was Mem)
# 28 Jan 99 - Mike: changed CMPS to not use address size prefix (av)

fields of opcodet (8) row 4:7 col 0:2 page 3:3
                     
r32 0:2
sr16 0:2
r16 0:2
r8 0:2
fsi 0:2
fieldinfo r32 is [names 
	[ "%eax" "%ecx" "%edx" "%ebx" "%esp" "%ebp" "%esi" "%edi" ] ]
fieldinfo sr16 is [sparse [ "%cs"=1, "%ss"=2, "%ds"=3, "%es"=4, "%fs"=5, "%gs"=6 ] ]
fieldinfo r16 is [names [ "%ax" "%cx" "%dx" "%bx" "%sp" "%bp" "%si" "%di" ]]
fieldinfo r8 is [names [ "%al" "%cl" "%dl" "%bl" "%ah" "%ch" "%dh" "%bh" ]]
fieldinfo fsi is [names [ "%st0" "%st1" "%st2" "%st3" "%st4" "%st5" "%st6" "%st7" ] ]

fields of modrm (8) mod 6:7 reg_opcode 3:5 r_m 0:2
fieldinfo reg_opcode is [ names 
	[ "%eax" "%ecx" "%edx" "%ebx" "%esp" "%ebp" "%esi" "%edi" ] ]

fields of sib   (8) ss 6:7 index 3:5 base 0:2
fieldinfo [ base index ] is [ names
	[ "%eax" "%ecx" "%edx" "%ebx" "%esp" "%ebp" "%esi" "%edi" ] ]
fieldinfo ss is [ sparse [ "1" = 0, "2" = 1, "4" = 2, "8" = 3 ] ]
fields of I8   (8) i8  0:7
fields of I16 (16) i16 0:15
fields of I32 (32) i32 0:31
patterns 
arith is any of [ ADD OR
                  ADC SBB
                  AND SUB
                  XOR CMP ], which is row = {0 to 3} & page = [0 1]
[ Eb.Gb Ev.Gv Gb.Eb Gv.Ev AL.Ib eAX.Iv ] is col = {0 to 5}
[ PUSH.ES POP.ES    PUSH.CS esc2
  PUSH.SS POP.SS    PUSH.DS POP.DS
  SEG.ES  DAA	    SEG.CS  DAS
  SEG.SS  AAA       SEG.DS  AAS   ] is row = {0 to 3} & page = [0 1] & col = [6 7]
regops is any of [ INC  DEC 
                   PUSH POP ], which is row = [4 5] & page = [0 1]
[ PUSHA   POPA    BOUND   ARPL    SEG.FS  SEG.GS   OpPrefix AddrPrefix 
  PUSH.Iv IMUL.Iv PUSH.Ib IMUL.Ib INSB    INSv     OUTSB    OUTSv      
] is page = [0 1] & row = 6 & col = {0 to 7} 
Jb is row = 7
cond is any of [ .O .NO .B .NB .Z .NZ .BE .NBE .S .NS .P .NP .L .NL .LE .NLE ], 
      which is page = [0 1] & col = {0 to 7}
[ Eb.Ib Ev.Iv MOVB Ev.Ib TEST.Eb.Gb TEST.Ev.Gv  XCHG.Eb.Gb XCHG.Ev.Gv ] is
               row = 8 & page = 0 & col = {0 to 7}
MOV is row = 8 & page = 1
[ MOV.Ew.Sw LEA MOV.Sw.Ew POP.Ev ] is row = 8 & page = 1 & col = {4 to 7}
XCHG is row = 9 & page = 0
NOP is XCHG & col = 0
[ CBW CWDQ CALL.aP WAIT PUSHF POPF SAHF LAHF ] is row = 9 & page = 1 & col = {0 to 7}
[ MOV.AL.Ob  MOV.eAX.Ov  MOV.Ob.AL  MOV.Ov.eAX MOVSB    MOVSv     CMPSB   CMPSv
  TEST.AL.Ib TEST.eAX.Iv STOSB      STOSv      LODSB    LODSv     SCASB   SCASv
] is row = 10 & page = [0 1] & col = {0 to 7} 
MOVib is row = 11 & page = 0
MOViv is row = 11 & page = 1
[ B.Eb.Ib B.Ev.Ib RET.Iw  RET     LES LDS MOV.Eb.Ib MOV.Ev.Iv      	
  B.Eb.1  B.Ev.1  B.Eb.CL B.Ev.CL AAM AAD _         XLAT
]   is row = [12 13] & page = 0 & col = {0 to 7}
[ ENTER LEAVE RET.far.Iw RET.far INT3 INT.Ib INTO IRET ]
    is row = 12      & page = 1 & col = {0 to 7}
ESC is row = 13      & page = 1
[ LOOPNE  LOOPE   LOOP    JCXZ    IN.AL.Ib  IN.eAX.Ib  OUT.Ib.AL OUT.Ib.eAX
  LOCK    _       REPNE   REP     HLT       CMC        grp3.Eb   grp3.Ev

  CALL.Jv JMP.Jv  JMP.Ap  JMP.Jb  IN.AL.DX  IN.eAX.DX  OUT.DX.AL OUT.DX.eAX
  CLC     STC     CLI     STI     CLD       STD        grp4      grp5       
] is page = [0 1] & row = [14 15] & col = {0 to 7}
[ grp6       grp7       LAR        LSL       
  MOV.Eb.Gb  MOV.Gv.Ev  MOV.Gb.Eb  MOV.Ev.Gv ]
is esc2; page = 0 & row = [0 1] & col = {0 to 3}
CLTS is esc2; page = 0 & row = 0 & col = 6
[ MOV.Rd.Cd  MOV.Rd.Dd  MOV.Cd.Rd  MOV.Dd.Rd  MOV.Rd.Td _  MOV.Td.Rd ]
is esc2; page = 0 & row = 3 & col = {0 to 6}
[ WRMSR RDTSC RDMSR ] is esc2; page = 0 & row = 4 & col = {0 to 2}
Jv   is esc2; row = 8
SETb is esc2; row = 9
[ PUSH.FS       POP.FS        CPUID  BT   SHLD.Ib SHLD.CL _            _
  CMPXCHG.Eb.Gb CMPXCHG.Ev.Gv LSS    BTR  LFS     LGS     MOVZX.Gv.Eb  MOVZX.Gv.Ew ]
is esc2; page = 0 & row = [10 11] & col = {0 to 7}
[ XADD.Eb.Gb XADD.Ev.Gv grp9 ] is esc2; page = 0 & row = 12 & col = [0 1 7]
[INVD WBINVD] is esc2; row = 0 & page = 1 & col = [0 1]
[ PUSH.GS POP.GS RSM  BTS   SHRD.Ib SHRD.CL _ IMUL.Gv.Ev] 
is esc2; row = 10 & page = 1 & col = {0 to 7}
[ grp8 BTC BSF BSR MOVSX.Gv.Eb MOVSX.Gv.Ew] 
is esc2; page = 1 & row = 11 & col = {2 to 7}
BSWAP is esc2; row = 12 & page = 1
patterns
  arithI    is any of [ ADDi ORi ADCi SBBi ANDi SUBi XORi CMPi ], 	# group 1
		       which is (Eb.Ib | Ev.Iv | Ev.Ib); reg_opcode = {0 to 7} ...
  bshifts   is B.Eb.1  | B.Eb.CL # D0 D2
  vshifts   is B.Ev.1  | B.Ev.CL # D1 D3
  immshifts is B.Eb.Ib | B.Ev.Ib # C0 C1
  rot       is any of [ ROL ROR RCL RCR SHLSAL SHR _ SAR], 
                       which is (bshifts | vshifts | immshifts); 
                                                         reg_opcode = {0 to 7} ...
  grp3ops   is any of 
      [ TEST.Ib.Iv _ NOT NEG MUL.AL.eAX IMUL.AL.eAX DIV.AL.eAX IDIV.AL.eAX ],
		       which is (grp3.Eb | grp3.Ev); reg_opcode = {0 to 7} ...
  grp4ops   is any of [ INC.Eb DEC.Eb ], 
                       which is grp4; reg_opcode = [0 1] ...
  grp5ops   is any of [ INC.Ev DEC.Ev CALL.Ev CALL.Ep JMP.Ev JMP.Ep PUSH.Ev _ ], 
		       which is grp5; reg_opcode = {0 to 7} ...
  grp6ops   is any of [ SLDT STR LLDT LTR VERR VERW _ _ ], 
		       which is grp6; reg_opcode = {0 to 7} ...
  grp7ops   is any of [ SGDT SIDT LGDT LIDT SMSW _ LMSW INVLPG ], 
		       which is grp7; reg_opcode = {0 to 7} ...
  bittestI  is any of [ BTi BTSi BTRi BTCi ], 
                       which is grp8; reg_opcode = {4 to 7} ...
  CMPXCHG8B is                  grp9; reg_opcode = 1 ...

patterns
  [ D8 D9 DA DB DC DD DE DF ] is ESC & col = {0 to 7}
  [ FADD FMUL FCOM FCOMP FSUB FSUBR FDIV FDIVR ] is reg_opcode = {0 to 7}
  [ FLD _ FST FSTP FLDENV FLDCW FSTENV FSTCW ]   is reg_opcode = {0 to 7} ...
  [ FNOP ]                         is D9; mod = 3 & reg_opcode = 2 & r_m = [0]
  [ FCHS FABS _ _ FTST FXAM _ _  ] is D9; mod = 3 & reg_opcode = 4 & r_m = {0 to 7}
  [ F2XM1 FYL2X FPTAN FPATAN FXTRACT FPREM1 FDECSTP FINCSTP ]
                                   is D9; mod = 3 & reg_opcode = 6 & r_m = {0 to 7}
  FXCH                             is D9; mod = 3 & reg_opcode = 1
  Fconstants is any of [ FLD1 FLDL2T FLDL2E FLDPI FLDLG2 FLDLN2 FLDZ _ ], which
                                   is D9; mod = 3 & reg_opcode = 5 & r_m = {0 to 7}
  [ FPREM FYL2XP1 FSQRT FSINCOS FRNDINT FSCALE FSIN FCOS ]
                                   is D9; mod = 3 & reg_opcode = 7 & r_m = {0 to 7}
  [ FIADD FIMUL FICOM FICOMP FISUB FISUBR FIDIV FIDIVR ] is reg_opcode = {0 to 7} ...
  FUCOMPP                          is DA; mod = 3 & reg_opcode = 5 & r_m = 1
  [ FILD _ FIST FISTP FBLD FLD.ext FBSTP FSTP.ext ] is reg_opcode = {0 to 7} ...
  [ FCLEX FINIT ]                  is DB; mod = 3 & reg_opcode = 4 & r_m = [2 3]
  [ FRSTOR _ FSAVE FSTSW ]          is reg_opcode = {4 to 7} ...
  [ FFREE _ FST.st FSTP.st FUCOM FUCOMP  _ _ ]  is mod = 3 & reg_opcode = {0 to 7}
  [ FADDP _ FUBSRP FDIVRP FMULP _ FSUBP FDIVP ] is mod = 3 & reg_opcode = {0 to 7}
  FCOMPP    is DE; mod = 3 & reg_opcode = 3 & r_m = 1
  FSTSW.AX  is DF; mod = 3 & reg_opcode = 4 & r_m = 0
patterns
  .STi      is DD; mod = 3
#  Fcmpi  is any of [.STi2 P.STi2 ], which is [ DB DF ]
  Fstack is any of [ .ST.STi .STi.ST P.STi.ST ], which is [ D8 DC DE ]; mod = 3
  Fint   is any of [.I32 .I16], which is [DA DE]
  Fmem   is any of [.R32 .R64], which is [D8 DC]
  FlsI   is any of [.lsI16 .lsI32], which is [DF DB]
  FlsR   is any of [.lsR32 .lsR64], which is [D9 DD]
#  [FUCOMI  FCOMI ] is DB ; mod = 3 & reg_opcode = {5 to 6}
#  [FUCOMIP FCOMIP] is DF ; mod = 3 & reg_opcode = {5 to 6}
   patterns FCOMs is FCOM | FCOMP
#   patterns FCOMIs is FCOMI | FCOMIP
#   patterns FUCOMIs is FUCOMI | FUCOMIP
   patterns FISTs is FIST | FISTP
   patterns
FSTs is FST | FSTP
FSTs.st is FST.st | FSTP.st
   patterns FUCOMs is FUCOM | FUCOMP


   patterns lfp is LDS | LES | LFS | LGS | LSS
   patterns REPs is REP | REPNE
   patterns STRINGsb is CMPSB | LODSB | MOVSB | SCASB | STOSB
   patterns STRINGsv is CMPSv | LODSv | MOVSv | SCASv | STOSv
    patterns LOOPs is LOOP | LOOPE | LOOPNE
   patterns POPs is POP.ES | POP.SS | POP.DS | POP.FS | POP.GS 
            POPv is POPA | POPF
   patterns PUSHs is PUSH.CS | PUSH.SS | PUSH.DS | PUSH.ES | PUSH.FS | PUSH.GS
            PUSHv is PUSHA | PUSHF
  patterns shdIb is SHRD.Ib | SHLD.Ib
           shdCL is SHRD.CL | SHLD.CL
patterns ow is OpPrefix
         od is epsilon
         ov is ow | od
patterns aw is AddrPrefix
         ad is epsilon
         av is aw | ad
placeholder for opcodet is HLT
placeholder for modrm is HLT
placeholder for sib is HLT
placeholder for I8  is HLT
placeholder for I16 is HLT; HLT
placeholder for I32 is HLT; HLT; HLT; HLT
relocatable reloc
constructors
  rel8  reloc : Rel8  { reloc = L + i8!  } is i8;  L: epsilon
  rel16 reloc : Rel16 { reloc = L + i16! } is i16; L: epsilon
  rel32 reloc : Rel32 { reloc = L + i32! } is i32; L: epsilon
relocatable d a

# 32-bit Addressing modes
constructors
  Indir    [r32] : Mem { r32 != 4, r32 != 5 } is mod = 0 & r_m = r32
  Disp8 i8![r32] : Mem { r32 != 4 }         is mod = 1 & r_m = r32; i8
  Disp32  d[r32] : Mem { r32 != 4 }         is mod = 2 & r_m = r32; i32 = d
  Abs32   [a]    : Mem                      is mod = 0 & r_m = 5;   i32 = a
  Reg     r32    : Eaddr                    is mod = 3 & r_m = r32
  Index    [base][index * ss] : Mem { index != 4, base != 5 } is 
                        mod = 0 & r_m = 4; index & base     & ss
  Base     [base]             : Mem { base != 5 } is
                        mod = 0 & r_m = 4; index = 4 & base
  Index8 i8![base][index * ss]: Mem { index != 4 } is 
                        mod = 1 & r_m = 4; index & base     & ss; i8
  Base8   d[base]             : Mem  is
                        mod = 1 & r_m = 4; index = 4 & base;      i8  = d
  Index32 d[base][index * ss] : Mem { index != 4 } is 
                        mod = 2 & r_m = 4; index & base     & ss; i32 = d
  Base32  d[base]             : Mem is
                        mod = 2 & r_m = 4; index = 4 & base;      i32 = d
  ShortIndex    d[index * ss] : Mem { index != 4 } is 
                        mod = 0 & r_m = 4; index & base = 5 & ss; i32 = d
  IndirMem      [d]           : Mem  is
                        mod = 0 & r_m = 4; index = 4 & base = 5;  i32 = d
  E Mem : Eaddr is Mem
constructors
  Indir_32    [r32] : Modrm32 { r32 != 4, r32 != 5 } is mod = 0 & r_m = r32
  Disp8_32 i8![r32] : Modrm32 { r32 != 4 }         is mod = 1 & r_m = r32; i8
  Disp32_32  d[r32] : Modrm32 { r32 != 4 }         is mod = 2 & r_m = r32; i32 = d
  Abs32_32   [a]    : Modrm32                      is mod = 0 & r_m = 5;   i32 = a
  Reg_32     r32    : Modrm32                      is mod = 3 & r_m = r32
  Index_32    [base][index * ss] : Modrm32 { index != 4, base != 5 } is 
                        mod = 0 & r_m = 4; index & base     & ss
  Base_32     [base]             : Modrm32 { base != 5 } is
                        mod = 0 & r_m = 4; index = 4 & base
  Index8_32 i8![base][index * ss]: Modrm32 { index != 4 } is 
                        mod = 1 & r_m = 4; index & base     & ss; i8
  Base8_32   d[base]             : Modrm32  is
                        mod = 1 & r_m = 4; index = 4 & base;      i8  = d
  Index32_32 d[base][index * ss] : Modrm32 { index != 4 } is 
                        mod = 2 & r_m = 4; index & base     & ss; i32 = d
  Base32_32  d[base]             : Modrm32 is
                        mod = 2 & r_m = 4; index = 4 & base;      i32 = d
  ShortIndex_32    d[index * ss] : Modrm32 { index != 4 } is 
                        mod = 0 & r_m = 4; index & base = 5 & ss; i32 = d
  IndirMem_32      [d]           : Modrm32  is
                        mod = 0 & r_m = 4; index = 4 & base = 5;  i32 = d
constructors
  Indir_16    [r32] : Modrm16 { r32 != 4, r32 != 5 } is mod = 0 & r_m = r32
  Disp8_16 i8![r32] : Modrm16 { r32 != 4 }         is mod = 1 & r_m = r32; i8
  Disp32_16  d[r32] : Modrm16 { r32 != 4 }         is mod = 2 & r_m = r32; i32 = d
  Abs32_16   [a]    : Modrm16                      is mod = 0 & r_m = 5;   i32 = a
  Reg_16     r32    : Modrm16                      is mod = 3 & r_m = r32
  Index_16    [base][index * ss] : Modrm16 { index != 4, base != 5 } is 
                        mod = 0 & r_m = 4; index & base     & ss
  Base_16     [base]             : Modrm16 { base != 5 } is
                        mod = 0 & r_m = 4; index = 4 & base
  Index8_16 i8![base][index * ss]: Modrm16 { index != 4 } is 
                        mod = 1 & r_m = 4; index & base     & ss; i8
  Base8_16   d[base]             : Modrm16  is
                        mod = 1 & r_m = 4; index = 4 & base;      i8  = d
  Index32_16 d[base][index * ss] : Modrm16 { index != 4 } is 
                        mod = 2 & r_m = 4; index & base     & ss; i32 = d
  Base32_16  d[base]             : Modrm16 is
                        mod = 2 & r_m = 4; index = 4 & base;      i32 = d
  ShortIndex_16    d[index * ss] : Modrm16 { index != 4 } is 
                        mod = 0 & r_m = 4; index & base = 5 & ss; i32 = d
  IndirMem_16      [d]           : Modrm16  is
                        mod = 0 & r_m = 4; index = 4 & base = 5;  i32 = d
constructors
  Indir_8    [r32] : Modrm8 { r32 != 4, r32 != 5 } is mod = 0 & r_m = r32
  Disp8_8 i8![r32] : Modrm8 { r32 != 4 }         is mod = 1 & r_m = r32; i8
  Disp32_8  d[r32] : Modrm8 { r32 != 4 }         is mod = 2 & r_m = r32; i32 = d
  Abs32_8   [a]    : Modrm8                      is mod = 0 & r_m = 5;   i32 = a
  Reg_8     r32    : Modrm8                      is mod = 3 & r_m = r32
  Index_8    [base][index * ss] : Modrm8 { index != 4, base != 5 } is 
                        mod = 0 & r_m = 4; index & base     & ss
  Base_8     [base]             : Modrm8 { base != 5 } is
                        mod = 0 & r_m = 4; index = 4 & base
  Index8_8 i8![base][index * ss]: Modrm8 { index != 4 } is 
                        mod = 1 & r_m = 4; index & base     & ss; i8
  Base8_8   d[base]             : Modrm8  is
                        mod = 1 & r_m = 4; index = 4 & base;      i8  = d
  Index32_8 d[base][index * ss] : Modrm8 { index != 4 } is 
                        mod = 2 & r_m = 4; index & base     & ss; i32 = d
  Base32_8  d[base]             : Modrm8 is
                        mod = 2 & r_m = 4; index = 4 & base;      i32 = d
  ShortIndex_8    d[index * ss] : Modrm8 { index != 4 } is 
                        mod = 0 & r_m = 4; index & base = 5 & ss; i32 = d
  IndirMem_8      [d]           : Modrm8  is
                        mod = 0 & r_m = 4; index = 4 & base = 5;  i32 = d

constructors
  arith^"iAL"    i8!          is      arith & AL.Ib ; i8
  arith^"iAX"    i16!         is  ow; arith & eAX.Iv; i16
  arith^"iEAX"   i32!         is  od; arith & eAX.Iv; i32
  arithI^"b"     Eaddr, i8!   is      (Eb.Ib; Eaddr) & arithI; i8
  arithI^"w"     Eaddr, i16!  is  ow; (Ev.Iv; Eaddr) & arithI; i16
  arithI^"d"     Eaddr, i32!  is  od; (Ev.Iv; Eaddr) & arithI; i32
  arithI^ov^"b"  Eaddr, i8!   is  ov; (Ev.Ib; Eaddr) & arithI; i8
  arith^"mrb"    Eaddr, r8  is      arith & Eb.Gb; Eaddr & reg_opcode = r8 ...
  arith^"mr"^ov  Eaddr, r32   is  ov; arith & Ev.Gv; Eaddr & reg_opcode = r32 ...
  arith^"rmb"    r8, Eaddr  is      arith & Gb.Eb; Eaddr & reg_opcode = r8 ...
  arith^"rm"^ov  r32, Eaddr   is  ov; arith & Gv.Ev; Eaddr & reg_opcode = r32 ...
constructors
AAA
AAD is AAD; i8 = 10
AAM is AAM; i8 = 10
AAS
# ADC, ADD, AND are in arith group
ARPL     Eaddr, reg16  is  ARPL;      Eaddr & reg_opcode = reg16 ...
constructors
BOUND^ov reg, Mem    is  ov; BOUND; Mem   & reg_opcode = reg ...
BSF^ov   reg, Eaddr  is  ov; BSF;   Eaddr & reg_opcode = reg ...
BSR^ov   reg, Eaddr  is  ov; BSR;   Eaddr & reg_opcode = reg ...
BSWAP    r32         is  BSWAP & ... r32
BT^ov    Eaddr, reg  is  ov; BT;    Eaddr  & reg_opcode = reg ...
BTi^ov   Eaddr, i8!  is  ov; (grp8; Eaddr) & BTi;  i8
BTC^ov   Eaddr, reg  is  ov; BTC;   Eaddr  & reg_opcode = reg ...
BTCi^ov  Eaddr, i8!  is  ov; (grp8; Eaddr) & BTCi; i8
BTR^ov   Eaddr, reg  is  ov; BTR;   Eaddr  & reg_opcode = reg ...
BTRi^ov  Eaddr, i8!  is  ov; (grp8; Eaddr) & BTRi; i8
BTS^ov   Eaddr, reg  is  ov; BTS;   Eaddr  & reg_opcode = reg ...
BTSi^ov  Eaddr, i8!  is  ov; (grp8; Eaddr) & BTSi; i8
CALL.Jv^ow reloc    is  ow; CALL.Jv; rel16(reloc)
CALL.Jv^od reloc    is  od; CALL.Jv; rel32(reloc)
CALL.Ep^ov Mem      is  ov; (grp5; Mem) & CALL.Ep
CALL.aP^ow CS":"IP is  ow; CALL.aP; i16 = IP; i16 = CS
CALL.aP^od CS":"IP is  od; CALL.aP; i32 = IP; i16 = CS
CALL.Ev^ov Eaddr is ov; (grp5; Eaddr) & CALL.Ev
CBW  is ow; CBW
CWDE is od; CBW
CLC
CLD
CLI
CLTS
CMC
# CMP  is in the arith group
CMPSB
CMPSv^ov	 is ov; CMPSv
CMPXCHG.Eb.Gb    Eaddr, reg is     CMPXCHG.Eb.Gb; Eaddr & reg_opcode = reg ...
CMPXCHG.Ev.Gv^ov Eaddr, reg is ov; CMPXCHG.Ev.Gv; Eaddr & reg_opcode = reg ...
CMPXCHG8B        Mem        is     (grp9; Mem) & CMPXCHG8B
CPUID
CWD  is ow; CWDQ
CDQ  is od; CWDQ
DAA
DAS
DEC.Eb     Eaddr  is      (grp4; Eaddr) & DEC.Eb
DEC.Ev^ov  Eaddr  is  ov; (grp5; Eaddr) & DEC.Ev
DEC^ov     r32    is  ov; DEC & r32
DIV^"AL"   Eaddr  is      (grp3.Eb; Eaddr) & DIV.AL.eAX
DIV^"AX"   Eaddr  is  ow; (grp3.Ev; Eaddr) & DIV.AL.eAX
DIV^"eAX"  Eaddr  is  od; (grp3.Ev; Eaddr) & DIV.AL.eAX
ENTER   i16, i8!  is  ENTER; i16; i8

F2XM1
FABS
FADD^Fmem    Mem  is  Fmem; Mem & FADD ...
FADD^Fstack  fsi  is  Fstack & ... (FADD & r_m = fsi)
FIADD^Fint  Mem  is  Fint; Mem & FIADD ...
FBLD        Mem  is  DF; Mem & FBLD
FBSTP       Mem  is  DF; Mem & FBSTP
FCHS
FNCLEX           is  FCLEX
   constructors
FCOMs^Fmem     Mem  is  Fmem; Mem & FCOMs ...       # includes FICOM, FICOMP
FCOMs^.ST.STi  fsi  is  .ST.STi & ... (FCOMs   & r_m = fsi)
FCOMI.ST.STi   fsi  is  DB ; mod = 3 & reg_opcode = 6 & r_m = fsi
FCOMIP.ST.STi  fsi  is  DF ; mod = 3 & reg_opcode = 6 & r_m = fsi
FUCOMI.ST.STi  fsi  is  DB ; mod = 3 & reg_opcode = 5 & r_m = fsi
FUCOMIP.ST.STi fsi  is  DF ; mod = 3 & reg_opcode = 5 & r_m = fsi
FCOMPP
FCOS
FDECSTP
FDIV^Fmem     Mem  is  Fmem; Mem & FDIV ...
FDIV^Fstack   fsi  is  Fstack & ... (FDIV & r_m = fsi)
FIDIV^Fint	Mem  is  Fint; Mem & FIDIV ...
FDIVR^Fmem    Mem  is  Fmem; Mem & FDIVR ...
FDIVR^Fstack  fsi  is  Fstack & ... (FDIVR & r_m = fsi)
FIDIVR^Fint	Mem  is  Fint; Mem & FIDIVR ...
FFREE         fsi  is  DD; FFREE & r_m = fsi
FICOM^Fint   Mem is Fint; Mem & FICOM
FICOMP^Fint  Mem is Fint; Mem & FICOMP
FILD^FlsI Mem is FlsI; Mem & FILD
FILD64    Mem is DF;  Mem & FLD.ext ...
FINIT
   constructors
FISTs^FlsI  Mem  is  FlsI; Mem & FISTs
FISTP64     Mem  is  DF;   Mem & FSTP.ext
FLD^FlsR    Mem  is  FlsR; Mem & FLD
FLD80       Mem  is  DB;   Mem & FLD.ext ...
FLD.STi     fsi  is  D9;    mod = 3 & FLD & r_m = fsi
Fconstants
FLDCW       Mem  is  D9;   Mem & FLDCW
FLDENV      Mem  is  D9;   Mem & FLDENV
FMUL^Fmem   Mem  is  Fmem; Mem & FMUL ...
FMUL^Fstack fsi  is  Fstack & ... (FMUL & r_m = fsi)
FIMUL^Fint	Mem  is  Fint; Mem & FIMUL ...
FNOP
FPATAN
FPREM
FPREM1
FPTAN
FRNDINT
FRSTOR Mem  is DD; Mem & FRSTOR
FNSAVE Mem  is DD; Mem & FSAVE
FSCALE
FSIN
FSINCOS
FSQRT
   constructors
FSTs^FlsR     Mem  is  FlsR; Mem & FSTs
FSTP80        Mem  is  DB;   Mem & FSTP.ext
FSTs.st^.STi  fsi  is  .STi & ... (FSTs.st & r_m = fsi)
FSTCW         Mem  is  D9;   Mem & FSTCW
FSTENV        Mem  is  D9;   Mem & FSTENV
FSTSW         Mem  is  DD;   Mem & FSTSW
FSTSW.AX
FSUB^Fmem     Mem  is  Fmem; Mem & FSUB ...
FSUB^Fstack   fsi  is  Fstack & ... (FSUB & r_m = fsi)
FISUB^Fint	Mem  is  Fint; Mem & FISUB ...
FSUBR^Fmem    Mem  is  Fmem; Mem & FSUBR ...
FSUBR^Fstack  fsi  is  Fstack & ... (FSUBR & r_m = fsi)
FISUBR^Fint	Mem  is  Fint; Mem & FISUBR ...
FTST
   constructors
FUCOMs	  	fsi   is  DD; FUCOMs & r_m = fsi
FUCOMPP
FXAM
FXCH fsi  is  FXCH & ... r_m = fsi
FXTRACT
FYL2X
FYL2XP1

HLT
IDIV        Eaddr  is      (grp3.Eb; Eaddr) & IDIV.AL.eAX
IDIV^"AX"   Eaddr  is  ow; (grp3.Ev; Eaddr) & IDIV.AL.eAX
IDIV^"eAX"  Eaddr  is  od; (grp3.Ev; Eaddr) & IDIV.AL.eAX
IMULb        Eaddr            is      (grp3.Eb;    Eaddr) & IMUL.AL.eAX
IMUL^ov      Eaddr            is  ov; (grp3.Ev;    Eaddr) & IMUL.AL.eAX
IMULrm^ov    reg, Eaddr       is  ov; IMUL.Gv.Ev; Eaddr & reg_opcode = reg ...
IMUL.Ib^ov   reg, Eaddr, i8!  is  ov; IMUL.Ib;    Eaddr & reg_opcode = reg ... ; i8
IMUL.Iv^"w"  reg, Eaddr, i16! is  ow; IMUL.Iv;    Eaddr & reg_opcode = reg ... ; i16
IMUL.Iv^"d"  reg, Eaddr, i32! is  od; IMUL.Iv;    Eaddr & reg_opcode = reg ... ; i32
IN.AL.Ib       i8! is      IN.AL.Ib;  i8
IN.eAX.Ib^ov   i8! is  ov; IN.eAX.Ib; i8
IN.AL.DX
IN.eAX.DX^ov       is  ov; IN.eAX.DX
INC.Eb     Eaddr  is      (grp4; Eaddr) & INC.Eb
INC.Ev^ov  Eaddr  is  ov; (grp5; Eaddr) & INC.Ev
INC^ov     r32    is  ov; INC & r32
INSB
INSv^ov   is  ov; INSv
INT3
INT.Ib i8! is  INT.Ib; i8
INTO
INVD
INVLPG  Mem  is  (grp7; Mem) & INVLPG
IRET
Jb^cond    reloc   is      Jb & cond;     rel8(reloc)
Jv^cond^ow reloc   is  ow; Jv & ... cond; rel16(reloc)
Jv^cond^od reloc   is  od; Jv & ... cond; rel32(reloc)
JMP.Jb     reloc   is      JMP.Jb; rel8(reloc)
JMP.Jv^ow  reloc   is  ow; JMP.Jv; rel16(reloc)
JMP.Jv^od  reloc   is  od; JMP.Jv; rel32(reloc)
JMP.Ap^ow  CS":"IP  is  ow; JMP.Ap; i16 = IP; i16 = CS
JMP.Ap^od  CS":"IP  is  od; JMP.Ap; i32 = IP; i16 = CS
JMP.Ev^ov Eaddr    is  ov; (grp5; Eaddr) & JMP.Ev
JMP.Ep^ov Mem      is  ov; (grp5; Mem  ) & JMP.Ep
LAHF
LAR^ov  reg, Eaddr  is  ov; LAR; Eaddr & reg_opcode = reg ...
LEA^ov  reg, Mem    is  ov; LEA; Mem   & reg_opcode = reg ...
LEAVE	
LODSB
LODSv^ov	 is ov; LODSv
    constructors
lfp     reg!, Mem   is      lfp; Mem & (mod != 3 & reg_opcode = reg) ... 
LOOPs  reloc  is  LOOPs; rel8(reloc)
LSL^ov  reg, Eaddr  is  ov; LSL; Eaddr & reg_opcode = reg ...
LTR  Eaddr  is  (grp6; Eaddr) & LTR
MOV^"mrb"    Eaddr, reg    is      MOV & Eb.Gb; Eaddr & reg_opcode = reg ...
MOV^"mr"^ov  Eaddr, reg    is  ov; MOV & Ev.Gv; Eaddr & reg_opcode = reg ...
MOV^"rmb"    reg, Eaddr    is      MOV & Gb.Eb; Eaddr & reg_opcode = reg ...
MOV^"rm"^ov  reg, Eaddr    is  ov; MOV & Gv.Ev; Eaddr & reg_opcode = reg ...
MOV.Ew.Sw    Mem, sr16     is  ow; MOV.Ew.Sw;   Mem   & reg_opcode = sr16 ...
MOV.Sw.Ew    Mem, sr16     is      MOV.Sw.Ew;   Mem   & reg_opcode = sr16 ...
# assume 32-bit address mode
MOV.AL.Ob      offset      is      MOV.AL.Ob;  i32 = offset
MOV.eAX.Ov^ov  offset      is  ov; MOV.eAX.Ov; i32 = offset
MOV.Ob.AL      offset      is      MOV.Ob.AL;  i32 = offset
MOV.Ov.eAX^ov  offset      is  ov; MOV.Ov.eAX; i32 = offset
MOVib          r8,  i8!    is      MOVib & r8; i8
MOViw          r16, i16!   is  ow; MOViv & r16; i16
MOVid          r32, i32!   is  od; MOViv & r32; i32
MOV.Eb.Ib      Eaddr, i8!  is      MOV.Eb.Ib; Eaddr & reg_opcode = 0 ...; i8
MOV.Eb.Iv^ow   Eaddr, i16! is  ow; MOV.Ev.Iv; Eaddr & reg_opcode = 0 ...; i16
MOV.Eb.Iv^od   Eaddr, i32! is  od; MOV.Ev.Iv; Eaddr & reg_opcode = 0 ...; i32
MOV.Cd.Rd      cr, reg     is  MOV.Cd.Rd; mod = 3 & r_m = reg & reg_opcode = cr
MOV.Rd.Cd      reg, cr     is  MOV.Rd.Cd; mod = 3 & r_m = reg & reg_opcode = cr
MOV.Dd.Rd      dr, reg     is  MOV.Dd.Rd; mod = 3 & r_m = reg & reg_opcode = dr
MOV.Rd.Dd      reg, dr     is  MOV.Rd.Dd; mod = 3 & r_m = reg & reg_opcode = dr
MOVSB
MOVSv^ov is ov; MOVSv
MOVSX.Gv.Eb^ov  r32, Eaddr is ov; MOVSX.Gv.Eb; Eaddr & reg_opcode = r32 ...
MOVSX.Gv.Ew     r16, Eaddr is     MOVSX.Gv.Ew; Eaddr & reg_opcode = r16 ...
MOVZX.Gv.Eb^ov  r32, Eaddr is ov; MOVZX.Gv.Eb; Eaddr & reg_opcode = r32 ...
MOVZX.Gv.Ew     r16, Eaddr is     MOVZX.Gv.Ew; Eaddr & reg_opcode = r16 ...
MUL.AL      Eaddr  is      (grp3.Eb; Eaddr) & MUL.AL.eAX
MUL.AX^ov   Eaddr  is  ov; (grp3.Ev; Eaddr) & MUL.AL.eAX



NEGb    Eaddr  is      (grp3.Eb; Eaddr) & NEG
NEG^ov  Eaddr  is  ov; (grp3.Ev; Eaddr) & NEG
NOP
NOTb    Eaddr  is      (grp3.Eb; Eaddr) & NOT
NOT^ov  Eaddr  is  ov; (grp3.Ev; Eaddr) & NOT
# OR is in the arith group
OUT.Ib.AL       i8! is      OUT.Ib.AL;  i8
OUT.Ib.eAX^ov   i8! is  ov; OUT.Ib.eAX; i8
OUT.DX.AL
OUT.DX.eAX^ov       is  ov; OUT.DX.eAX
OUTSB
OUTSv^ov is ov; OUTSv
POP.Ev^ov   Mem   is  ov; POP.Ev; Mem & reg_opcode = 0 ...
POP^ov      r32   is  ov; POP & r32
   constructors
POPs
POPv^ov  is  ov; POPv
PUSH.Ev^ov  Eaddr  is  ov; (grp5; Eaddr) & PUSH.Ev
PUSH^ov     r32    is  ov; PUSH & r32
PUSH.Ib^ov  i8!    is      PUSH.Ib; i8
PUSH.Iv^ow  i16!   is  ow; PUSH.Iv; i16
PUSH.Iv^od  i32!   is  od; PUSH.Iv; i32
   constructors
PUSHs
PUSHv^ov  is  ov; PUSHv
# ROL ROR RCL RCR SHLSAL SHR SAR
rot^bshifts    Eaddr       is     (bshifts; Eaddr) & rot
rot^vshifts^ov Eaddr       is ov; (vshifts; Eaddr) & rot
rot^B.Eb.Ib    Eaddr, i8!  is     (B.Eb.Ib; Eaddr) & rot; i8
rot^B.Ev.Ib^ov Eaddr, i8!  is ov; (B.Ev.Ib; Eaddr) & rot; i8
RDMSR
REPs^STRINGsb    is REPs; STRINGsb
REPs^STRINGsv^ov is ov; REPs; STRINGsv
RET
RET.far
RET.Iw      i16  is RET.Iw; i16
RET.far.Iw  i16  is RET.far.Iw; i16
RSM
SAHF
# SAL SAR SHL SR above with rot
# SBB is in the arith group
SCASB
SCASv^ov  is  ov; SCASv
SEG.FS      # Make this a one byte NOP for now
SEG.GS      # Allows scanning of Windows startup code
SETb^cond  Eaddr  is  SETb & ... cond; Eaddr
SGDT  Mem  is (grp7; Mem) & SGDT
SIDT  Mem  is (grp7; Mem) & SIDT
  constructors
shdIb^ov  Eaddr, reg, count  is  ov; shdIb; Eaddr & reg_opcode = reg ... ; i8 = count
shdCL^ov  Eaddr, reg, "CL"   is  ov; shdCL; Eaddr & reg_opcode = reg ...
SLDT Eaddr  is  (grp6; Eaddr) & SLDT
SMSW Eaddr  is  (grp7; Eaddr) & SMSW
STC
STD
STI
STOSB
STOSv^ov is ov; STOSv
STR  Mem  is  (grp6; Mem) & STR
# SUB is in the arith group
TEST.AL.Ib      i8          is     TEST.AL.Ib; i8
TEST.eAX.Iv^ow  i16         is ow; TEST.eAX.Iv; i16
TEST.eAX.Iv^od  i32         is od; TEST.eAX.Iv; i32
TEST.Eb.Ib      Eaddr, i8   is     (grp3.Eb; Eaddr) & TEST.Ib.Iv; i8
TEST.Ew.Iw      Eaddr, i16  is ow; (grp3.Ev; Eaddr) & TEST.Ib.Iv; i16
TEST.Ed.Id      Eaddr, i32  is od; (grp3.Ev; Eaddr) & TEST.Ib.Iv; i32
TEST.Eb.Gb      Eaddr, reg  is     TEST.Eb.Gb; Eaddr & reg_opcode = reg ...
TEST.Ev.Gv^ov   Eaddr, reg  is ov; TEST.Ev.Gv; Eaddr & reg_opcode = reg ...
VERR  Eaddr  is (grp6; Eaddr) & VERR
VERW  Eaddr  is (grp6; Eaddr) & VERW
WAIT
WBINVD
WRMSR
XADD.Eb.Gb     Eaddr, reg  is     XADD.Eb.Gb; Eaddr & reg_opcode = reg ...
XADD.Ev.Gv^ov  Eaddr, reg  is ov; XADD.Ev.Gv; Eaddr & reg_opcode = reg ...
XCHG^"eAX"^ov  r32         is ov; XCHG & r32
XCHG.Eb.Gb     Eaddr, reg  is     XCHG.Eb.Gb; Eaddr & reg_opcode = reg ...
XCHG.Ev.Gv^ov  Eaddr, reg  is ov; XCHG.Ev.Gv; Eaddr & reg_opcode = reg ...
XLATB is XLAT
# XOR is in the arith group
