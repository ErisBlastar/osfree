#
#
#
#

PROJ = rexx
DEST = ..$(SEP)build$(SEP)lib
srcfiles = &
$(p)client$(e)   &
$(p)drexx$(e)    &
$(p)rexxsaa$(e)  &
$(p)funcs$(e)    &
$(p)builtin$(e)  &
$(p)error$(e)    &
$(p)variable$(e) &
$(p)interprt$(e) &
$(p)debug$(e)    &
$(p)dbgfuncs$(e) &
$(p)memory$(e)   &
$(p)parsing$(e)  &
$(p)files$(e)    &
$(p)misc$(e)     &
$(p)unxfuncs$(e) &
$(p)arxfuncs$(e) &
$(p)os2funcs$(e) &
$(p)cmsfuncs$(e) &
$(p)shell$(e)    &
$(p)rexxext$(e)  &
$(p)stack$(e)    &
$(p)tracing$(e)  &
$(p)interp$(e)   &
$(p)cmath$(e)    &
$(p)convert$(e)  &
$(p)strings$(e)  &
$(p)library$(e)  &
$(p)strmath$(e)  &
$(p)signals$(e)  &
$(p)macros$(e)   &
$(p)envir$(e)    &
$(p)mt_os2$(e) &
$(p)instore$(e)  &
$(p)expr$(e)     &
$(p)yaccsrc$(e)  &
$(p)lexsrc$(e)   &
$(p)wrappers$(e) &
$(p)options$(e)  &
$(p)os_os2$(e)   &
$(p)extstack$(e) &
$(p)rexxbif$(e)

!include $(%ROOT)mk/dirs.mk
!include $(MYDIR)..$(SEP)regina.mk

TARGETS = $(PATH)$(PROJ).lib

$(PATH)$(PROJ).lib: $(OBJS)
 $(LIB) $(LIBOPT) $^@ +$(OBJS)
