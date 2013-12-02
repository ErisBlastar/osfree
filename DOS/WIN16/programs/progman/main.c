/*
 * Program Manager
 *
 * Copyright 1996 Ulrich Schmid
 * Copyright 2002 Sylvain Petreolle
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdio.h>
#include <string.h>

#define OEMRESOURCE

#include "windows.h"
#include "progman.h"

GLOBALS Globals;

static VOID MAIN_CreateGroups(void);
static VOID MAIN_MenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
static ATOM MAIN_RegisterMainWinClass(void);
static VOID MAIN_CreateMainWindow(void);
static VOID MAIN_CreateMDIWindow(void);
static VOID MAIN_AutoStart(void);
static VOID WineLicense(HWND Wnd);
static VOID WineWarranty(HWND Wnd);

#define BUFFER_SIZE 1000

/***********************************************************************
 *
 *           WinMain
 */

int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE prev, LPSTR cmdline, int show)
{
  MSG      msg;

  Globals.lpszIniFile         = "progman.ini";
  Globals.lpszIcoFile         = "progman.exe";

  Globals.hInstance           = hInstance;
  Globals.hGroups             = 0;
  Globals.hActiveGroup        = 0;

  /* Read Options from `progman.ini' */
  Globals.bAutoArrange =
    GetPrivateProfileInt("Settings", "AutoArrange", 0, Globals.lpszIniFile);
  Globals.bMinOnRun =
    GetPrivateProfileInt("Settings", "MinOnRun", 0, Globals.lpszIniFile);
  Globals.bSaveSettings =
    GetPrivateProfileInt("Settings", "SaveSettings", 1, Globals.lpszIniFile);

  /* Load default icons */
  Globals.hMainIcon    = ExtractIcon(Globals.hInstance, Globals.lpszIcoFile, PROGMAN_ICON_INDEX);
  Globals.hGroupIcon   = ExtractIcon(Globals.hInstance, Globals.lpszIcoFile, GROUP_ICON_INDEX);
  Globals.hDefaultIcon = ExtractIcon(Globals.hInstance, Globals.lpszIcoFile, DEFAULT_ICON_INDEX);
  if (!Globals.hMainIcon)    Globals.hMainIcon = LoadIcon(0, MAKEINTRESOURCE(DEFAULTICON));
  if (!Globals.hGroupIcon)   Globals.hGroupIcon = LoadIcon(Globals.hInstance, MAKEINTRESOURCE(GROUP_ICON_INDEX));
  if (!Globals.hDefaultIcon) Globals.hDefaultIcon = LoadIcon(0, MAKEINTRESOURCE(DEFAULTICON));

  /* Register classes */
  if (!prev)
    {
      if (!MAIN_RegisterMainWinClass()) return(FALSE);
      if (!GROUP_RegisterGroupWinClass()) return(FALSE);
      if (!PROGRAM_RegisterProgramWinClass()) return(FALSE);
    }

  /* Create main window */
  MAIN_CreateMainWindow();
  Globals.hAccel = LoadAccelerators(Globals.hInstance, STRING_ACCEL);

  /* Setup menu, stringtable and resourcenames */
  STRING_LoadMenus();

  MAIN_CreateMDIWindow();

  /* Initialize groups */
  MAIN_CreateGroups();

  /* Start initial applications */
  MAIN_AutoStart();

  /* Message loop */
  while (GetMessage (&msg, 0, 0, 0))
    if (!TranslateAccelerator(Globals.hMainWnd, Globals.hAccel, &msg))
      {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
      }
  return 0;
}

/***********************************************************************
 *
 *           MAIN_CreateGroups
 */

static VOID MAIN_CreateGroups(void)
{
  char buffer[BUFFER_SIZE];
  char szPath[MAX_PATHNAME_LEN];
  char key[20], *ptr;

  /* Initialize groups according the `Order' entry of `progman.ini' */
  GetPrivateProfileString("Settings", "Order", "", buffer, sizeof(buffer), Globals.lpszIniFile);
  ptr = buffer;
  while (ptr < buffer + sizeof(buffer))
    {
      int num, skip, ret;
      ret = sscanf(ptr, "%d%n", &num, &skip);
      if (ret == 0)
        MAIN_MessageBoxIDS_s(IDS_FILE_READ_ERROR_s, Globals.lpszIniFile, IDS_ERROR, MB_OK);
      if (ret != 1) break;

      sprintf(key, "Group%d", num);
      GetPrivateProfileString("Groups", key, "", szPath,
                              sizeof(szPath), Globals.lpszIniFile);
      if (!szPath[0]) continue;

      GRPFILE_ReadGroupFile(szPath);

      ptr += skip;
    }
  /* FIXME initialize other groups, not enumerated by `Order' */
}

/***********************************************************************
 *
 *           MAIN_AutoStart
 */

VOID MAIN_AutoStart(void)
{
  char buffer[BUFFER_SIZE];
  HLOCAL hGroup, hProgram;

  GetPrivateProfileString("Settings", "AutoStart", "Autostart", buffer,
                          sizeof(buffer), Globals.lpszIniFile);

  for (hGroup = GROUP_FirstGroup(); hGroup; hGroup = GROUP_NextGroup(hGroup))
    if (!lstrcmp(buffer, GROUP_GroupName(hGroup)))
      for (hProgram = PROGRAM_FirstProgram(hGroup); hProgram;
           hProgram = PROGRAM_NextProgram(hProgram))
        PROGRAM_ExecuteProgram(hProgram);
}

/***********************************************************************
 *
 *           MAIN_MainWndProc
 */

static LRESULT CALLBACK MAIN_MainWndProc(HWND hWnd, UINT msg,
                                 WPARAM wParam, LPARAM lParam)
{
#if 0
  printf("M %4.4x %4.4x\n", msg, wParam);
#endif
  switch (msg)
    {
    case WM_INITMENU:
      CheckMenuItem(Globals.hOptionMenu, PM_AUTO_ARRANGE,
                    MF_BYCOMMAND | (Globals.bAutoArrange ? MF_CHECKED : MF_UNCHECKED));
      CheckMenuItem(Globals.hOptionMenu, PM_MIN_ON_RUN,
                    MF_BYCOMMAND | (Globals.bMinOnRun ? MF_CHECKED : MF_UNCHECKED));
      CheckMenuItem(Globals.hOptionMenu, PM_SAVE_SETTINGS,
                    MF_BYCOMMAND | (Globals.bSaveSettings ? MF_CHECKED : MF_UNCHECKED));
      break;

    case WM_COMMAND:
      if (wParam < PM_FIRST_CHILD){
        MAIN_MenuCommand(hWnd, wParam, lParam);
      }
      break;

    case WM_DESTROY:
      PostQuitMessage (0);
      break;
    }
  return(DefFrameProc(hWnd, Globals.hMDIWnd, msg, wParam, lParam));
}

/***********************************************************************
 *
 *           MAIN_MenuCommand
 */

static VOID MAIN_MenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
  HLOCAL hActiveGroup    = GROUP_ActiveGroup();
  HLOCAL hActiveProgram  = PROGRAM_ActiveProgram(hActiveGroup);
  HWND   hActiveGroupWnd = GROUP_GroupWnd(hActiveGroup);

  switch(wParam)
    {
      /* Menu File */
    case PM_NEW:
      switch (DIALOG_New((hActiveGroupWnd && !IsIconic(hActiveGroupWnd)) ?
                         PM_NEW_PROGRAM : PM_NEW_GROUP))
      {
      case PM_NEW_PROGRAM:
        if (hActiveGroup) PROGRAM_NewProgram(hActiveGroup);
        break;

      case PM_NEW_GROUP:
        GROUP_NewGroup();
        break;
      }
      break;

    case PM_OPEN:
      if (hActiveProgram)
        PROGRAM_ExecuteProgram(hActiveProgram);
      else if (hActiveGroupWnd)
        OpenIcon(hActiveGroupWnd);
      break;

    case PM_MOVE:
    case PM_COPY:
      if (hActiveProgram)
        PROGRAM_CopyMoveProgram(hActiveProgram, wParam == PM_MOVE);
      break;

    case PM_DELETE:
      if (hActiveProgram)
        {
        if (DIALOG_Delete(IDS_DELETE_PROGRAM_s, PROGRAM_ProgramName(hActiveProgram)))
          PROGRAM_DeleteProgram(hActiveProgram, TRUE);
        }
      else if (hActiveGroup)
        {
        if (DIALOG_Delete(IDS_DELETE_GROUP_s, GROUP_GroupName(hActiveGroup)))
          GROUP_DeleteGroup(hActiveGroup);
        }
      break;

    case PM_ATTRIBUTES:
      if (hActiveProgram)
        PROGRAM_ModifyProgram(hActiveProgram);
      else if (hActiveGroup)
        GROUP_ModifyGroup(hActiveGroup);
      break;

    case PM_EXECUTE:
      DIALOG_Execute();
      break;

    case PM_EXIT:
      PostQuitMessage(0);
      break;

      /* Menu Options */
    case PM_AUTO_ARRANGE:
      Globals.bAutoArrange = !Globals.bAutoArrange;
      CheckMenuItem(Globals.hOptionMenu, PM_AUTO_ARRANGE,
                    MF_BYCOMMAND | (Globals.bAutoArrange ?
                                    MF_CHECKED : MF_UNCHECKED));
      WritePrivateProfileString("Settings", "AutoArrange",
                                Globals.bAutoArrange ? "1" : "0",
                                Globals.lpszIniFile);
      WritePrivateProfileString(NULL,NULL,NULL,Globals.lpszIniFile); /* flush it */
      break;

    case PM_MIN_ON_RUN:
      Globals.bMinOnRun = !Globals.bMinOnRun;
      CheckMenuItem(Globals.hOptionMenu, PM_MIN_ON_RUN,
                    MF_BYCOMMAND | (Globals.bMinOnRun ?
                                    MF_CHECKED : MF_UNCHECKED));
      WritePrivateProfileString("Settings", "MinOnRun",
                                Globals.bMinOnRun ? "1" : "0",
                                Globals.lpszIniFile);
      WritePrivateProfileString(NULL,NULL,NULL,Globals.lpszIniFile); /* flush it */
      break;

    case PM_SAVE_SETTINGS:
      Globals.bSaveSettings = !Globals.bSaveSettings;
      CheckMenuItem(Globals.hOptionMenu, PM_SAVE_SETTINGS,
                    MF_BYCOMMAND | (Globals.bSaveSettings ?
                                    MF_CHECKED : MF_UNCHECKED));
      WritePrivateProfileString("Settings", "SaveSettings",
                                Globals.bSaveSettings ? "1" : "0",
                                Globals.lpszIniFile);
      WritePrivateProfileString(NULL,NULL,NULL,Globals.lpszIniFile); /* flush it */
      break;

      /* Menu Windows */
    case PM_ARRANGE:

      if (hActiveGroupWnd && !IsIconic(hActiveGroupWnd))
        ArrangeIconicWindows(hActiveGroupWnd);
      else
        SendMessage(Globals.hMDIWnd, WM_MDIICONARRANGE, 0, 0);
      break;

      /* Menu Help */
    case PM_CONTENTS:
      if (!WinHelp(Globals.hMainWnd, "progman.hlp", HELP_CONTENTS, 0))
        MAIN_MessageBoxIDS(IDS_WINHELP_ERROR, IDS_ERROR, MB_OK);
      break;

    case PM_HELPONHELP:
      if (!WinHelp(Globals.hMainWnd, "progman.hlp", HELP_HELPONHELP, 0))
        MAIN_MessageBoxIDS(IDS_WINHELP_ERROR, IDS_ERROR, MB_OK);
      break;

    case PM_TUTORIAL:
      WinExec("wintutor.exe", SW_SHOWNORMAL);
      break;

    case PM_LICENSE:
      WineLicense(Globals.hMainWnd);
      break;

    case PM_NO_WARRANTY:
      WineWarranty(Globals.hMainWnd);
      break;

    case PM_ABOUT_WINE:
      ShellAbout(hWnd, "Program Manager", "", 0);
      break;

    default:
        MAIN_MessageBoxIDS(IDS_NOT_IMPLEMENTED, IDS_ERROR, MB_OK);
      break;
    }
}

/***********************************************************************
 *
 *           MAIN_RegisterMainWinClass
 */

static ATOM MAIN_RegisterMainWinClass(void)
{
  WNDCLASS class;

  class.style         = CS_HREDRAW | CS_VREDRAW;
  class.lpfnWndProc   = MAIN_MainWndProc;
  class.cbClsExtra    = 0;
  class.cbWndExtra    = 0;
  class.hInstance     = Globals.hInstance;
  class.hIcon         = Globals.hMainIcon;
  class.hCursor       = LoadCursor (0, IDC_ARROW);
  class.hbrBackground = GetStockObject (NULL_BRUSH);
  class.lpszMenuName  = 0;
  class.lpszClassName = STRING_MAIN_WIN_CLASS_NAME;

  return RegisterClass(&class);
}

/***********************************************************************
 *
 *           MAIN_CreateMainWindow
 */

static VOID MAIN_CreateMainWindow(void)
{
  int  left , top, right, bottom, width, height, show;
  char buffer[100];

  Globals.hMDIWnd   = 0;
  Globals.hMainMenu = 0;

  /* Get the geometry of the main window */
  GetPrivateProfileString("Settings", "Window", "",
                          buffer, sizeof(buffer), Globals.lpszIniFile);
  if (5 == sscanf(buffer, "%d %d %d %d %d", &left, &top, &right, &bottom, &show))
  {
    width  = right - left;
    height = bottom - top;
  }
  else
  {
    left = top = width = height = CW_USEDEFAULT;
    show = SW_SHOWNORMAL;
  }

  /* Create main Window */
  Globals.hMainWnd =
    CreateWindow (STRING_MAIN_WIN_CLASS_NAME, "",
                  WS_OVERLAPPEDWINDOW, left, top, width, height,
                  0, 0, Globals.hInstance, 0);

  ShowWindow (Globals.hMainWnd, show);
  UpdateWindow (Globals.hMainWnd);
}

/***********************************************************************
 *
 *           MAIN_CreateMDIWindow
 */

static VOID MAIN_CreateMDIWindow(void)
{
  CLIENTCREATESTRUCT ccs;
  RECT rect;

  /* Get the geometry of the MDI window */
  GetClientRect(Globals.hMainWnd, &rect);

  ccs.hWindowMenu  = Globals.hWindowsMenu;
  ccs.idFirstChild = PM_FIRST_CHILD;

  /* Create MDI Window */
  Globals.hMDIWnd =
    CreateWindow (STRING_MDI_WIN_CLASS_NAME, "",
                  WS_CHILD, rect.left, rect.top,
                  rect.right - rect.left, rect.bottom - rect.top,
                  Globals.hMainWnd, 0,
                  Globals.hInstance, &ccs);

  ShowWindow (Globals.hMDIWnd, SW_SHOW);
  UpdateWindow (Globals.hMDIWnd);
}

/**********************************************************************/
/***********************************************************************
 *
 *           MAIN_MessageBoxIDS
 */
int MAIN_MessageBoxIDS(UINT ids_text, UINT ids_title, WORD type)
{
  char text[MAX_STRING_LEN];
  char title[MAX_STRING_LEN];

  LoadString(Globals.hInstance, ids_text, text, sizeof(text));
  LoadString(Globals.hInstance, ids_title, title, sizeof(title));

  return(MessageBox(Globals.hMainWnd, text, title, type));
}

/***********************************************************************
 *
 *           MAIN_MessageBoxIDS_s
 */
int MAIN_MessageBoxIDS_s(UINT ids_text, LPCSTR str, UINT ids_title, WORD type)
{
  char text[MAX_STRING_LEN];
  char title[MAX_STRING_LEN];
  char newtext[MAX_STRING_LEN + MAX_PATHNAME_LEN];

  LoadString(Globals.hInstance, ids_text, text, sizeof(text));
  LoadString(Globals.hInstance, ids_title, title, sizeof(title));
  wsprintf(newtext, text, str);

  return(MessageBox(Globals.hMainWnd, newtext, title, type));
}

/***********************************************************************
 *
 *           MAIN_ReplaceString
 */

VOID MAIN_ReplaceString(HLOCAL *handle, LPSTR replace)
{
  HLOCAL newhandle = LocalAlloc(LMEM_FIXED, _fstrlen(replace) + 1);
  if (newhandle)
    {
      LPSTR  newstring = LocalLock(newhandle);
      lstrcpy(newstring, replace);
      LocalFree(*handle);
      *handle = newhandle;
    }
  else MAIN_MessageBoxIDS(IDS_OUT_OF_MEMORY, IDS_ERROR, MB_OK);
}

VOID WineLicense(HWND Wnd)
{
  char cap[20], text[1024];
  LoadString(Globals.hInstance, IDS_LICENSE, text, sizeof text);
  LoadString(Globals.hInstance, IDS_LICENSE_CAPTION, cap, sizeof cap);
  MessageBox(Wnd, text, cap, MB_ICONINFORMATION | MB_OK);
}

VOID WineWarranty(HWND Wnd)
{
  char cap[20], text[1024];
  LoadString(Globals.hInstance, IDS_WARRANTY, text, sizeof text);
  LoadString(Globals.hInstance, IDS_WARRANTY_CAPTION, cap, sizeof cap);
  MessageBox(Wnd, text, cap, MB_ICONEXCLAMATION | MB_OK);
}

/* Local Variables:    */
/* c-file-style: "GNU" */
/* End:                */
