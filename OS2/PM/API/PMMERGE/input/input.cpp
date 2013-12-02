#include <FreePM.hpp>
//#include <habmgr.hpp>
#include <F_hab.hpp>
#include <pmclient.h>

#include "FreePM_err.hpp"
#include "F_def.hpp"
#include "debug.h"

#define  INCL_WIN
#include <os2.h>

//#define debug(...)

/*** Keyboard and mouse input COMMON subsection *************************/

extern "C" BOOL  APIENTRY Win32SetFocus(HWND hwndDesktop,
                            HWND hwndSetFocus)
{
 debug(6, 2)( __FUNCTION__ "is not yet implemented\n");
 return 0;
}


extern "C" BOOL  APIENTRY Win32FocusChange(HWND hwndDesktop,
                              HWND hwndSetFocus,
                              ULONG flFocusChange)
{
 debug(6, 2)( __FUNCTION__ "is not yet implemented\n");
 return 0;
}

