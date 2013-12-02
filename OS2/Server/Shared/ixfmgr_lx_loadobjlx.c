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

#define INCL_DOS
#define INCL_DOSEXCEPTIONS
#include <os2.h>


#include <io.h>
#include <cfgparser.h>
#include <loadobjlx.h>
#include <fixuplx.h>
#include <modlx.h>
#include <modmgr.h>
#include <ixfmgr.h>
#include <sys/mman.h>
#include <native_dynlink.h>
#include <os2errcodes.h>

#include <string.h>

extern struct t_mem_area os2server_root_mem_area;

#if 0
  /* Loads the objects for code and data, for programs. NOT used. */
int load_code_data_obj_lx(struct LX_module * lx_exe_mod, struct t_os2process * proc) {
        struct o32_obj * kod_obj = (struct o32_obj *) get_code(lx_exe_mod);

        struct o32_obj * stack_obj = (struct o32_obj *) get_data_stack(lx_exe_mod);

        void * vm_code_obj = 0;
        void * vm_data_obj = 0;

        if((kod_obj != 0) && (kod_obj == stack_obj)) { /* Data and code in same object. */
                io_printf("load_code_data_obj_lx: Code and stack/data is the same object!\n");

                /* Allocate virtual memory at the address that kod_obj requires. */
                vm_code_obj = (void*) vm_alloc_obj_lx(lx_exe_mod, kod_obj);

                /* Register the allocated memory with memmgr. */
                alloc_mem_area(&proc->root_mem_area, (void *) kod_obj->o32_base, kod_obj->o32_size);

                proc->code_mmap = vm_code_obj;

                if(vm_code_obj == MAP_FAILED) {
                        io_printf("Error mapping memory for(code/data)\n");
                        return 0;
                }
                /* Load code object. */
                load_obj_lx(lx_exe_mod, kod_obj, vm_code_obj);
                return 1;
        }

        /* Allocate virtual memory at the address that kod_obj requires. */
        vm_code_obj = (void*) vm_alloc_obj_lx(lx_exe_mod, kod_obj);


        proc->code_mmap = vm_code_obj;

        if(vm_code_obj == MAP_FAILED) {
                io_printf("Error mapping memory for (code)\n");
                return 0;
        }
        /* Register the allocated memory with memmgr. */
        alloc_mem_area(&proc->root_mem_area, (void *) kod_obj->o32_base, kod_obj->o32_size);
        load_obj_lx(lx_exe_mod, kod_obj, vm_code_obj);

        if(stack_obj == 0) return 0;
        vm_data_obj = (void*) vm_alloc_obj_lx(lx_exe_mod, stack_obj);

        /* Register the allocated memory with memmgr. */
        alloc_mem_area(&proc->root_mem_area, (void *) stack_obj->o32_base, stack_obj->o32_size);

        proc->stack_mmap = vm_data_obj;

        if(vm_data_obj == MAP_FAILED) {
                io_printf("Error mapping memory for (data/stack)\n");
                return 0;
        }
        load_obj_lx(lx_exe_mod, stack_obj, vm_data_obj);

        print_o32_obj_info(*kod_obj, " Info about kod_obj ");
        print_o32_obj_info(*stack_obj, " Info about stack_obj ");

        return 1;
}
#endif


  /*   Allocates virtual memory for an object at an absolute virtual address.
       Some GNU/Linux specific flags to mmap(MAP_GROWSDOWN). */
unsigned int vm_alloc_obj_lx(struct LX_module * lx_exe_mod, struct o32_obj * lx_obj) {

        void * mmap_obj = 0;

#if 0 /*!defined(__OS2__) && !defined(__LINUX__) */
        mmap_obj = mmap((void *)lx_obj->o32_base, lx_obj->o32_size,
                                 PROT_WRITE | PROT_READ | PROT_EXEC  ,       /* | PROT_EXEC */
                                  MAP_GROWSDOWN | MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0/*lx_exe_mod->fh*/,
                                 0 /*lx_exe_mod->lx_head_e32_exe->e32_datapage*/);
#else
// Under OS/2 return always unique address
  #if defined(__LINUX__) || defined(__WIN32__)
        mmap_obj = malloc(lx_obj->o32_size);
  /* void * l4_alloc_mem(int base, int size, int flags) */
  #else
    #ifdef L4API_l4v2
        #include <l4_alloc_mem.h>
        mmap_obj = l4_alloc_mem(lx_obj->o32_base, lx_obj->o32_size,
                                 PAG_COMMIT|PAG_EXECUTE|PAG_READ|PAG_WRITE);
    #else
        DosAllocMem(&mmap_obj, lx_obj->o32_size, PAG_COMMIT|PAG_EXECUTE|PAG_READ|PAG_WRITE);
    #endif
  #endif
#endif
        return (unsigned int) mmap_obj;
}

        /* Loads an object to the memory area pointed by vm_ptr_obj. */
int load_obj_lx(struct LX_module * lx_exe_mod,
                                struct o32_obj * lx_obj, void *vm_ptr_obj)
{
  unsigned long int tmp_code;
  unsigned long int tmp_vm;
  unsigned long int tmp_vm_code;
  struct o32_map * obj_pg_sta;
  int ofs_page_sta;
  int startpage = lx_obj->o32_pagemap;
  int lastpage  = lx_obj->o32_pagemap + lx_obj->o32_mapsize;
  unsigned int data_pages_offs =  get_e32_datapage(lx_exe_mod);
  unsigned int code_mmap_pos = 0;
  int page_nr=0;

  /*struct o32_map * obj_pg_ett = get_obj_map(lx_exe_mod ,startpage);*/
  /*  Reads in all pages from kodobject to designated virtual memory. */
  for(page_nr=startpage; page_nr<lastpage; page_nr++)
  {
    obj_pg_sta = get_obj_map(lx_exe_mod ,page_nr);
    ofs_page_sta = (obj_pg_sta->o32_pagedataoffset << get_e32_pageshift(lx_exe_mod))
                                                                + data_pages_offs;

    lx_exe_mod->lx_fseek(lx_exe_mod, ofs_page_sta, SEEK_SET);

    tmp_code = (unsigned long int) code_mmap_pos;
    tmp_vm = (unsigned long int) vm_ptr_obj;
    tmp_vm_code = tmp_code + tmp_vm;

    lx_exe_mod->lx_fread((void *)tmp_vm_code,
                         obj_pg_sta->o32_pagesize, 1, lx_exe_mod);
    code_mmap_pos += obj_pg_sta->o32_pagesize;
  }
  return 0;
}


/* @brief Applies fixups to all objects except impored functions.
   Used for programs and dlls.
   ret_rc is an OS/2 error number. */

int do_fixup_code_data_lx(struct LX_module * lx_exe_mod, int *ret_rc)
{
  unsigned long int i;
  /* If there is a code object (with a main function) then do a fixup on it and
     it's data/stack object if it exists.*/
  for(i=1; i<=get_obj_num(lx_exe_mod); i++)
  {
    struct o32_obj * obj = get_obj(lx_exe_mod, i);
    if(obj != 0)
      if(!do_fixup_obj_lx(lx_exe_mod, obj, ret_rc))
        return 0; /*Some error happend, return false and forward error in ret_rc.*/
  }
 return 1;
}



/* Internal Fixup*/
void apply_internal_fixup(struct LX_module * lx_exe_mod, struct r32_rlc * min_rlc, unsigned long int vm_start_of_page)
{

  unsigned long int * ptr_source; // Address to fixup
  int srcoff_cnt1;
  int addit;
  int object1;
  int trgoffs;
  struct o32_obj * target_object;
  unsigned long int vm_start_target_obj;
  unsigned long int vm_source;
  unsigned long int vm_target;

  srcoff_cnt1 = get_srcoff_cnt1_rlc(min_rlc);
  object1 = get_mod_ord1_rlc(min_rlc); /* On the same offset as Object1. */
  trgoffs = get_trgoff_size(min_rlc);
  addit = get_additive_rlc(min_rlc);

  target_object = get_obj(lx_exe_mod, object1);
  vm_start_target_obj = target_object->o32_base;

  /* Get address of target offset and put in source offset. */
  vm_target = vm_start_target_obj + get_imp_ord1_rlc(min_rlc)/*trgoffs*/;
  vm_source = vm_start_of_page + srcoff_cnt1;

  if (options.debugixfmgr) io_printf("source=%x, target=%x, addit=%d\n", vm_source, vm_target, addit);

  ptr_source = (unsigned long int *)vm_source;
  *ptr_source = vm_target;
}




/* Applies fixups for an object. Returns true(1) or false(0) to show status.*/
int do_fixup_obj_lx(struct LX_module * lx_exe_mod,
                                struct o32_obj * lx_obj, int *ret_rc)
{
  int ord_found;
  char *pas_imp_proc_name;
  int import_name_offs;
  char buf_import_name[260];
  char cont_buf_mod_name[255];
  char * mod_name;
  char * cont_mod_name;
  int import_ord;
  int mod_nr;
  unsigned long rc;
  unsigned long int * ptr_source;
  unsigned long int vm_source;
  unsigned long int vm_target;
  unsigned long int vm_start_target_obj;
  struct o32_obj * target_object;
  char buf_mod_name[255];
  char * org_mod_name;
  struct LX_module *found_module;
  int trgoffs;
  int object1;
  int addit;
  int srcoff_cnt1;
  int fixup_source_flag;
  int fixup_source;
  int fixup_offset;
  char *import_name;
  unsigned long int vm_start_of_page;
  struct r32_rlc * min_rlc;
  int pg_offs_fix;
  int pg_end_offs_fix;
  int page_nr=0;
  int startpage = lx_obj->o32_pagemap;
  int lastpage  = lx_obj->o32_pagemap + lx_obj->o32_mapsize;
  UCHAR uchLoadError[CCHMAXPATH] = {0}; /* Error info from DosExecPgm */

  //io_printf("--------------------Listing fixup data ------------------------- %p\n", lx_obj);

  /* Goes through every page of the object.
     The fixups are variable size and a bit messy to traverse.*/

  for(page_nr=startpage; page_nr < lastpage; page_nr++)
  {
    if (options.debugixfmgr) io_printf("-----  Object %d of %d\n",startpage, lastpage);

    /* Go and get byte position for fixup from the page logisk_sida.
       Start offset for fixup in the page*/
    pg_offs_fix = get_fixup_pg_tbl_offs(lx_exe_mod, page_nr);

    /* Offset for next page.*/
    pg_end_offs_fix = get_fixup_pg_tbl_offs(lx_exe_mod, page_nr+1);

    /* Fetches a relocations structure from offset pg_offs_fix.*/
    min_rlc = get_fixup_rec_tbl_obj(lx_exe_mod, pg_offs_fix);

    fixup_offset = pg_offs_fix;

    /* Get the memory address for the page. The page number is
       from the beginning of all pages a need to be adapted to the beginning of this
       object. */
    vm_start_of_page = lx_obj->o32_base +
                                    get_e32_pagesize(lx_exe_mod) * (page_nr-lx_obj->o32_pagemap);

    /*
    This loop traverses the fixups and increases
    the pointer min_rlc with the size of previoues fixup.
    while(min_rlc is within the offset of current page) {
    */
    while(fixup_offset < pg_end_offs_fix)
    {
      min_rlc = get_fixup_rec_tbl_obj(lx_exe_mod, fixup_offset);
      print_struct_r32_rlc_info(min_rlc);

      fixup_source = min_rlc->nr_stype & 0xf;
      fixup_source_flag = min_rlc->nr_stype & 0xf0;

      switch(min_rlc->nr_flags & NRRTYP)
      {
        case NRRINT :
          apply_internal_fixup(lx_exe_mod, min_rlc, vm_start_of_page);
          fixup_offset += get_reloc_size_rlc(min_rlc);
          break;
        case NRRORD:
          {/* Import by ordinal */
            /* Indata: lx_exe_mod, min_rlc */
            mod_nr = get_mod_ord1_rlc(min_rlc); // Request module number
            import_ord = get_imp_ord1_rlc(min_rlc); // Request ordinal number
            srcoff_cnt1 = get_srcoff_cnt1_rlc(min_rlc);
            addit = get_additive_rlc(min_rlc);

            mod_name = (char*)&buf_mod_name;

            /* Get name of imported module. */
            org_mod_name = get_imp_mod_name(lx_exe_mod,mod_nr);
            copy_pas_str(mod_name, org_mod_name);

#if 0
            /* Look for module if it's already loaded, if it's not try to load it.*/
            rc=ModLoadModule(uchLoadError, sizeof(uchLoadError),
                              mod_name, (unsigned long *)&found_module);
            if (found_module)
               found_module=(struct LX_module *)(((IXFModule *)found_module)->FormatStruct);
            if(!found_module) { /* Unable to find and load module. */
                   io_printf("Can't find module: '%s' \n", mod_name);
                   *ret_rc = rc;
                   return 0;
            }
            cont_mod_name = (char*)&cont_buf_mod_name;
            copy_pas_str(cont_mod_name, get_module_name_res_name_tbl_entry(lx_exe_mod));


            if(!apply_import_fixup(lx_exe_mod, found_module, lx_obj, mod_nr, import_ord,
                                                    addit, srcoff_cnt1, min_rlc, ret_rc)) {
               char tmp_buf[255];
               char *s_buf=(char*) &tmp_buf[0];
               copy_pas_str(s_buf, get_imp_proc_name(found_module,import_ord));
               io_printf("Import error in '%s', can't find '%s'(%d)\n",
                            mod_name, s_buf, import_ord);
               *ret_rc = 182; /* ERROR_ORDINAL_NOT_FOUND 182, ERROR_FILE_NOT_FOUND 2*/
               return 0;
            }
#endif

          }
          fixup_offset += get_reloc_size_rlc(min_rlc);
          break;

        case NRRNAM:
          {/* Import by name */
            //io_printf("Import by name \n");
            mod_nr = get_mod_ord1_rlc(min_rlc);
            import_name_offs = get_imp_ord1_rlc(min_rlc);
            srcoff_cnt1 = get_srcoff_cnt1_rlc(min_rlc);
            addit = get_additive_rlc(min_rlc);

            pas_imp_proc_name = get_imp_proc_name(lx_exe_mod, import_name_offs);
            copy_pas_str(buf_import_name, pas_imp_proc_name);
            import_name = (char*)&buf_import_name;
            mod_name = (char*)&buf_mod_name;
                                            /* Get name of imported module. */
            org_mod_name = get_imp_mod_name(lx_exe_mod,mod_nr);
            copy_pas_str(mod_name, org_mod_name);
#if 0
            /* Look for module if it's already loaded, if it's not try to load it.*/
            rc=ModLoadModule(uchLoadError, sizeof(uchLoadError),
                             mod_name, (unsigned long *)&found_module);
            if (found_module)
               found_module=(struct LX_module *)(((IXFModule *)found_module)->FormatStruct);

            if(!found_module) { /* Unable to find and load module. */
              //io_printf("Can't find module: '%s' \n", mod_name);
              *ret_rc = rc;
              return 0;
            }

            ord_found = get_res_name_tbl_entry(found_module, buf_import_name);
            if(ord_found == 0)
                    ord_found = get_non_res_name_tbl_entry(found_module, buf_import_name);


            if(!apply_import_fixup(lx_exe_mod, found_module, lx_obj, mod_nr,
                                   ord_found, addit, srcoff_cnt1, min_rlc, ret_rc)) {
               char tmp_buf[255];
               char *s_buf=(char*) &tmp_buf[0];
               copy_pas_str(s_buf, get_imp_proc_name(found_module,import_ord));
               io_printf("Import error in '%s', can't find '%s'\n", mod_name, s_buf);
               *ret_rc = 182; /* ERROR_ORDINAL_NOT_FOUND 182, ERROR_FILE_NOT_FOUND 2*/
               return 0;
            }


            /* TODO Translate to English!
              Leta efter funktionen  buf_import_name i mod_name. Den �r en str�ng
              s� skapa funktioner att leta i Entry Table i mod_name.
              N�t med en pascalstr�ng och en ordinal, anv�nd ordinalen som
              vanligt och leta upp funktionen med den.
            */
#endif

          }
          fixup_offset += get_reloc_size_rlc(min_rlc);
          break;

        default: io_printf("Unsupported Fixup! SRC: 0x%x \n", fixup_source);
                 return 0; /* Is there any OS/2 error number for this? */
      } /* switch(fixup_source) */
    } /* while(fixup_offset < pg_end_offs_fix) { */
  }
  return 1;
}
