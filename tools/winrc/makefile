
PROJ = winrc
DESC = Win16 .rc to .c compiler
srcfiles = $(p)ifyacc$(e) &
$(p)rcyacc$(e) &
$(p)rclex$(e) &
$(p)define$(e) &
$(p)resources$(e) &
$(p)util$(e) &
$(p)rc$(e) &
$(p)printres$(e) &
$(p)cmdline$(e) &
$(p)buffer$(e) &
$(p)cleanup$(e) &
# $(p)mac$(e)
$(p)mdc$(e)

ADD_COPT = -i=..$(SEP)include -i=.

!include $(%ROOT)/mk/tools.mk

TARGETS = $(PATH)$(PROJ).exe

$(PATH)rclex.obj: $(PATH)rclex.c

$(PATH)rcyacc.obj: $(PATH)rcyacc.c

$(PATH)ifyacc.obj: $(PATH)ifyacc.c post

#.c: $(PATH)

#.h: $(PATH)

post: .symbolic
  sed -e "s/yy/zz/g" -e "s/YY/ZZ/g" $(PATH)ifyacc.c >$(PATH)rc.z.tab.c
  sed -e "s/yy/zz/g" -e "s/YY/ZZ/g" $(PATH)ifyacc.h >$(PATH)rc.z.tab.h
  $(DC) $(PATH)ifyacc.c
  $(DC) $(PATH)ifyacc.h
  $(RN) $(PATH)rc.z.tab.c $(PATH)ifyacc.c
  $(RN) $(PATH)rc.z.tab.h $(PATH)ifyacc.h

