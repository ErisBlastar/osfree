#
# A Makefile for OS/3 build tools
# (c) osFree project,
# valerius, 2006/10/30
#

# Notes:
# 1. UniAPI must come first here because used to produce API headers
#Yacc needs z.lib which is built in the shared/z sub directory. Or maybe not?

DIRS = scripts yacc LEX SED  UNIAPI awk &
       mkmsgf sc somcpp mkctxt critstrs freeinst libmmap os2386 &
       #shared winrc somipc genext2fs qemu-img hlldump  rexxwrap mapsym ipfc ltools renmodul

!include $(%ROOT)/mk/all.mk

TARGETS = subdirs
TARGET = all
