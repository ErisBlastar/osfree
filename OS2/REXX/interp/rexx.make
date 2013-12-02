#   File:       rexx.make
#   Target:     rexx
#   Created:    Sunday, March 18, 2001 07:43:32 AM


MAKEFILE        = rexx.make
�MondoBuild�    = {MAKEFILE}  # Make blank to avoid rebuilds when makefile is modified

ObjDir          = :
Includes        = 

Sym-68K         = -sym on

COptions        = {Includes} {Sym-68K} -d MAC -d FGC -d NO_EXTERNAL_QUEUES -model far


### Source Files ###

SrcFiles        =  �
				  alloca.c �
				  builtin.c �
				  cmath.c �
				  cmsfuncs.c �
				  convert.c �
				  dbgfuncs.c �
				  debug.c �
				  doscmd.c �
				  envir.c �
				  error.c �
				  expr.c �
				  extlib.c �
				  extstack.c �
				  files.c �
				  funcs.c �
				  getopt.c �
				  instore.c �
				  interp.c �
				  interprt.c �
				  lexsrc.c �
				  library.c �
				  macros.c �
				  mac.c �
				  memory.c �
				  misc.c �
				  mt_notmt.c �
				  nosaa.c �
				  options.c �
				  os2funcs.c �
				  parsing.c �
				  rexx.c �
				  rexxbif.c �
				  rexxext.c �
				  shell.c �
				  signals.c �
				  stack.c �
				  strings.c �
				  strmath.c �
				  tracing.c �
				  unxfuncs.c �
				  variable.c �
				  wrappers.c �
				  yaccsrc.c


### Object Files ###

ObjFiles-68K    =  �
				  "{ObjDir}alloca.c.o" �
				  "{ObjDir}builtin.c.o" �
				  "{ObjDir}cmath.c.o" �
				  "{ObjDir}cmsfuncs.c.o" �
				  "{ObjDir}convert.c.o" �
				  "{ObjDir}dbgfuncs.c.o" �
				  "{ObjDir}debug.c.o" �
				  "{ObjDir}doscmd.c.o" �
				  "{ObjDir}envir.c.o" �
				  "{ObjDir}error.c.o" �
				  "{ObjDir}expr.c.o" �
				  "{ObjDir}extlib.c.o" �
				  "{ObjDir}extstack.c.o" �
				  "{ObjDir}files.c.o" �
				  "{ObjDir}funcs.c.o" �
				  "{ObjDir}getopt.c.o" �
				  "{ObjDir}instore.c.o" �
				  "{ObjDir}interp.c.o" �
				  "{ObjDir}interprt.c.o" �
				  "{ObjDir}lexsrc.c.o" �
				  "{ObjDir}library.c.o" �
				  "{ObjDir}mac.c.o" �
				  "{ObjDir}macros.c.o" �
				  "{ObjDir}memory.c.o" �
				  "{ObjDir}misc.c.o" �
				  "{ObjDir}mt_notmt.c.o" �
				  "{ObjDir}nosaa.c.o" �
				  "{ObjDir}options.c.o" �
				  "{ObjDir}os2funcs.c.o" �
				  "{ObjDir}parsing.c.o" �
				  "{ObjDir}rexx.c.o" �
				  "{ObjDir}rexxbif.c.o" �
				  "{ObjDir}rexxext.c.o" �
				  "{ObjDir}shell.c.o" �
				  "{ObjDir}signals.c.o" �
				  "{ObjDir}stack.c.o" �
				  "{ObjDir}strings.c.o" �
				  "{ObjDir}strmath.c.o" �
				  "{ObjDir}tracing.c.o" �
				  "{ObjDir}unxfuncs.c.o" �
				  "{ObjDir}variable.c.o" �
				  "{ObjDir}wrappers.c.o" �
				  "{ObjDir}yaccsrc.c.o"

### Libraries ###

LibFiles-68K    =  �
				  "{Libraries}MathLib.far.o" �
				  "{CLibraries}StdCLib.far.o" �
				  "{Libraries}SIOW.far.o" �
				  "{Libraries}MacRuntime.o" �
				  "{Libraries}IntEnv.far.o" �
				  "{Libraries}ToolLibs.far.o" �
				  "{Libraries}Interface.o"


### Default Rules ###

.c.o  �  .c  {�MondoBuild�}
	{C} {depDir}{default}.c -o {targDir}{default}.c.o {COptions}


### Build Rules ###

rexx  ��  {ObjFiles-68K} {LibFiles-68K} {�MondoBuild�}
	ILink �
		-o {Targ} �
		{ObjFiles-68K} �
		{LibFiles-68K} �
		{Sym-68K} �
		-mf -d �
		-t 'APPL' �
		-c 'siow' �
		-model far �
		-state rewrite �
		-compact -pad 0
	If "{Sym-68K}" =~ /-sym �[nNuU]�/
		ILinkToSYM {Targ}.NJ -mf -sym 3.2 -c 'sade'
	End

rexx  ��  "{RIncludes}"SIOW.r {�MondoBuild�}
	Rez "{RIncludes}"SIOW.r -o {Targ} -append


### Required Dependencies ###

"{ObjDir}alloca.c.o"  �  alloca.c
"{ObjDir}builtin.c.o"  �  builtin.c
"{ObjDir}cmath.c.o"  �  cmath.c
"{ObjDir}cmsfuncs.c.o"  �  cmsfuncs.c
"{ObjDir}convert.c.o"  �  convert.c
"{ObjDir}dbgfuncs.c.o"  �  dbgfuncs.c
"{ObjDir}debug.c.o"  �  debug.c
"{ObjDir}doscmd.c.o"  �  doscmd.c
"{ObjDir}envir.c.o"  �  envir.c
"{ObjDir}error.c.o"  �  error.c
"{ObjDir}expr.c.o"  �  expr.c
"{ObjDir}extlib.c.o"  �  extlib.c
"{ObjDir}extstack.c.o"  �  extstack.c
"{ObjDir}files.c.o"  �  files.c
"{ObjDir}funcs.c.o"  �  funcs.c
"{ObjDir}getopt.c.o"  �  getopt.c
"{ObjDir}instore.c.o"  �  instore.c
"{ObjDir}interp.c.o"  �  interp.c
"{ObjDir}interprt.c.o"  �  interprt.c
"{ObjDir}lexsrc.c.o"  �  lexsrc.c
"{ObjDir}library.c.o"  �  library.c
"{ObjDir}mac.c.o"  �  mac.c
"{ObjDir}macros.c.o"  �  macros.c
"{ObjDir}memory.c.o"  �  memory.c
"{ObjDir}misc.c.o"  �  misc.c
"{ObjDir}mt_notmt.c.o"  �  mt_notmt.c
"{ObjDir}nosaa.c.o"  �  nosaa.c
"{ObjDir}options.c.o"  �  options.c
"{ObjDir}os2funcs.c.o"  �  os2funcs.c
"{ObjDir}parsing.c.o"  �  parsing.c
"{ObjDir}rexx.c.o"  �  rexx.c
"{ObjDir}rexxbif.c.o"  �  rexxbif.c
"{ObjDir}rexxext.c.o"  �  rexxext.c
"{ObjDir}shell.c.o"  �  shell.c
"{ObjDir}signals.c.o"  �  signals.c
"{ObjDir}stack.c.o"  �  stack.c
"{ObjDir}strings.c.o"  �  strings.c
"{ObjDir}strmath.c.o"  �  strmath.c
"{ObjDir}tracing.c.o"  �  tracing.c
"{ObjDir}unxfuncs.c.o"  �  unxfuncs.c
"{ObjDir}variable.c.o"  �  variable.c
"{ObjDir}wrappers.c.o"  �  wrappers.c
"{ObjDir}yaccsrc.c.o"  �  yaccsrc.c


### Optional Dependencies ###
### Build this target to generate "include file" dependencies. ###

Dependencies  �  $OutOfDate
	MakeDepend �
		-append {MAKEFILE} �
		-ignore "{CIncludes}" �
		-objdir "{ObjDir}" �
		-objext .o �
		{Includes} �
		{SrcFiles}


