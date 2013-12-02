/*
    LXLoader - Loads LX exe files or DLLs for execution or to extract information from.
    Copyright (C) 2007  Sven Ros�n (aka Viking)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    Or see <http://www.gnu.org/licenses/>
*/

#define INCL_ORDINALS
#define INCL_DOS
#define INCL_BSEDOS
#define INCL_DOSEXCEPTIONS
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_ERRORS
#include <os2.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <modlx.h>
#include <loadobjlx.h>

#include <token.h>
#include <modmgr.h>
#include <native_dynlink.h>
#include <io.h>

struct native_module_rec native_module_root; /* Root for module list.*/

//const int LOADING = 1;
//const int DONE_LOADING = 0;

char *mini_native_libpath="c:\\os2\\dll";
int sz_native_mini_libpath; /* Number of elements in mini_libpath (zero-based).*/

const char *dll_suf = ".dll";  /* DLL extension (native libs). */
const char *lib_pre = "";      /* lib prefix. lib<module name>.so*/
char *sep = "\\";              /* Dir separator for OS/2  */

  /* Initializes the root node in the linked list, which itself is not used.
  Only to make sure the list at least always has one element allocated. */
void init_native_dynlink(void) {
        native_module_root.mod_name =  "root";
        native_module_root.file_name_path = "root";
        native_module_root.file_name = "root";
        native_module_root.module_struct = 0;
        native_module_root.next = 0;
}


        /* Register a module with the name. */
struct native_module_rec *
native_register_module(char * name, char * filepath, void * mod_struct)
{
  struct native_module_rec * new_mod;
  struct native_module_rec * prev;

  io_printf("register_module: %s, %p \n", name, mod_struct);
  new_mod = (struct native_module_rec *) malloc(sizeof(struct native_module_rec));
  new_mod->mod_name = (char *)malloc(strlen(name)+1);
  strcpy(new_mod->mod_name, name);
  prev = &native_module_root;

        while(prev->next) /* Find free node at end. */
                prev = (struct native_module_rec *) prev->next;

        prev->next = new_mod;  /*struct native_module_rec module_struct*/
        new_mod->module_struct = mod_struct; /* A pointer to handle for .so file. */

        new_mod->file_name_path = (char *)malloc(strlen(filepath)+1);
        strcpy(new_mod->file_name_path, filepath);

        return new_mod;
}

        /* Searches for the module name which the process proc needs.
           It first sees if it's already loaded and then just returns the found module.
           If it can't be found load_module() searches the mini_libpath inside find_module_path(). */
void * native_find_module(char * name)
{
  void *ptr_mod;
        struct native_module_rec * prev = (struct native_module_rec *) native_module_root.next;

        while(prev) {
                io_printf("find_module: %s == %s, mod=%p \n", name, prev->mod_name, prev->module_struct);
                if(strcmp(name, prev->mod_name)==0) {
                        io_printf("ret find_module: %p\n", prev->module_struct);

                        if(prev->load_status == LOADING) {
                                io_printf("find_module: ERROR, Cycle in loading of %s\n",name);
                                return 0;
                        }
                        return prev->module_struct;
                }
                prev = (struct native_module_rec *) prev->next;
        }

        ptr_mod = native_load_module(name);
        if(ptr_mod != 0) { /* If the module has been loaded, register it. */
                /* register_module(name, ptr_mod); */
                return ptr_mod;
        }

        return 0;
}



unsigned long native_find_module_path(char * name, char * full_path_name)
{
  FILE *f;
  char *p = mini_native_libpath - 1;
  STR_SAVED_TOKENS st;
  char * p_buf = full_path_name;


  StrTokSave(&st);
  if((p = StrTokenize((char*)mini_native_libpath, ";")) != 0) do if(*p)
  {
    p_buf = full_path_name;
    p_buf[0] = 0;
    strcat(p_buf, p);
    strcat(p_buf, "\\");
    strcat(p_buf, name);
    strcat(p_buf, ".dll");
    f = fopen(p_buf, "rb"); /* Tries to open the file, if it works f is a valid pointer.*/
    if(f)
    {
      StrTokStop();
      return NO_ERROR;
    }
  } while((p = StrTokenize(0, ";")) != 0);
  StrTokRestore(&st);

  return ERROR_FILE_NOT_FOUND;
}

        /* Loads a module name which proc needs. */
void * native_load_module(char * name) {

        const int buf_size = 4096;
        char buf[4096];
        char *p_buf = (char *) &buf;

        if (!strcmp(name, "KAL"))
        {
          native_register_module(name, name, (void *)0xffff);
          return (void *)0xffff;
        }

        native_find_module_path(name, p_buf); /* Searches for module name and returns the full path in
                                                                        the buffer p_buf. */

        //io_printf("load_module: '%s' \n", p_buf);

        /* Load LX file from ordinary disk file. */
        if(p_buf ) {
                HMODULE handle;
                APIRET rval;
                char buf[128];
                struct native_module_rec * new_module_el;

                rval = DosLoadModule( buf, sizeof(buf), p_buf, &handle );

                if (rval) {
                        //fprintf(stderr, "Could not open '%s': %s\n", p_buf, buf);
                        return (void *)handle;
                }
                new_module_el = native_register_module(name, p_buf, (void *)handle);
                return (void *)handle;
        }

        io_printf("load_module: Load error!!! of %s in %s\n", name, p_buf);
        return 0;
}

struct native_module_rec * native_get_root() {
        return &native_module_root;
}

struct native_module_rec * native_get_next(struct native_module_rec * el) {
        if(el != 0)
                return (struct native_module_rec *) el->next;
        else
                return 0;
}

char * native_get_name(struct native_module_rec * el) {
        if(el != 0)
                return el->mod_name;
        else
                return 0;
}

struct LX_module * native_get_module(struct native_module_rec * el) {
        if(el != 0)
                return (struct LX_module *) el->module_struct;
        else
                return 0;
}

void native_print_module_table(void) {
        struct native_module_rec * el = native_get_root();
        io_printf("--- Native loaded Module Table ---\n");
        while((el = native_get_next(el))) {
                io_printf("module = %s, module_struct = %p, load_status = %d\n",
                                el->mod_name, el->module_struct, el->load_status);
        }
}

void * native_get_func_ptr_str_modname(char * funcname, char * modname) {
        void * mod_handle;
        char * error;
        void * mydltest;
        APIRET rval;

        io_printf(" Searching func ptr '%s' in '%s' \n", funcname, modname);

        mod_handle = native_find_module(modname);

        rval = DosQueryProcAddr( (HMODULE)mod_handle, 0, funcname, (PFN *)&mydltest);

        if (rval)  {
                fprintf(stderr, "Could not locate symbol '%s': %d\n", funcname, rval);
                //exit(1);
        }
        return mydltest;
}

void * native_get_func_ptr_handle_modname(char * funcname, void * native_mod_handle) {
        char * error;
        void * mydltest;
        APIRET rval;

        rval = DosQueryProcAddr( (HMODULE)native_mod_handle, 0, funcname, (PFN *)&mydltest);
        if (rval)  {
                fprintf(stderr, "Could not locate symbol '%s': %d\n", funcname, rval);
                //exit(1);
        }
        io_printf("native_get_func_ptr_handle_modname( %s, %p)=%p\n",
                        funcname, native_mod_handle, mydltest);
        return mydltest;
}

void * native_get_func_ptr_ord_modname(int ord, char * modname)
{
        void * mod_handle;
        char * error;
        void * mydltest;
        APIRET rval;

        io_printf(" Searching func ptr ordinal=%d in '%s' \n", ord, modname);

        mod_handle = native_find_module(modname);

        rval = DosQueryProcAddr( (HMODULE)mod_handle, ord, NULL, (PFN *)&mydltest);

        if (rval)  {
                fprintf(stderr, "Could not locate ordinal '%d': %d\n", ord, rval);
                //exit(1);
        }
        return mydltest;
}

APIRET APIENTRY  tempDosPutMessage(HFILE hfile,
                                  ULONG cbMsg,
                                  PCHAR pBuf)
{
  io_printf(pBuf);
  return NO_ERROR;
}

#if 0
Writes a specified number of bytes from a buffer to the specified file.

#define INCL_DOSFILEMGR
#include <os2.h>

HFILE     hFile;      /*  File handle from DosOpen. */
PVOID     pBuffer;    /*  Address of the buffer that contains the data to write. */
ULONG     cbWrite;    /*  Number of bytes to write. */
PULONG    pcbActual;  /*  Address of the variable to receive the number of bytes actually written. */
APIRET    ulrc;       /*  Return Code. */
#endif

APIRET APIENTRY tempDosWrite(HFILE hFile, PVOID pBuffer, ULONG cbWrite, PULONG pcbActual)
{
  io_printf(pBuffer);
  return NO_ERROR;
}

APIRET APIENTRY tempDosExit(ULONG action, ULONG result)
{
  io_printf("__FINCTION__ not implemented yet\n");
  return NO_ERROR;
}

APIRET APIENTRY tempDosQuerySysInfo(ULONG iStart, ULONG iLast, PVOID pBuf,
         ULONG cbBuf)
{
  io_printf("__FINCTION__ not implemented yet\n");
  return NO_ERROR;
}


APIRET APIENTRY tempDosFSCtl(PVOID pData, ULONG cbData, PULONG pcbData,
                             PVOID pParms, ULONG cbParms, PULONG pcbParms,
                             ULONG function, PSZ pszRoute, HFILE hFile,
                             ULONG method)
{
  io_printf("__FINCTION__ not implemented yet\n");
  return NO_ERROR;
}

void * native_get_func_ptr_ord_handle(int ord, void * native_mod_handle)
{
        void * mod_handle;
        char * error;
        void * mydltest;
        APIRET rval;

        if (native_mod_handle==(void *)0xffff)
        {
          io_printf("KAL module. Special handling.\n");
          if (ord==1) //DosFSCtl
          {
            mydltest=&tempDosFSCtl;
          } else if (ord==2) //DosWrite
          {
            mydltest=&tempDosWrite;
          } else if (ord==3) // DosExit
          {
            mydltest=&tempDosExit;
          } else if (ord==4) //DosQuerySysInfo
          {
            mydltest=&tempDosQuerySysInfo;
          } else {
            io_printf("Unsupported ordinal in KAL.\n");
          }
          rval=0;
        } else {
          rval = DosQueryProcAddr( (HMODULE)native_mod_handle, ord, NULL, (PFN *)&mydltest);
        }

        if (rval)  {
                fprintf(stderr, "Could not locate ordinal '%d': %d\n", ord, rval);
                //exit(1);
        }
        return mydltest;
}


