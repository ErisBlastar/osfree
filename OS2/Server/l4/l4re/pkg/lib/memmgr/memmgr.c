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

#include <l4/os3/memmgr.h>
#include <l4/os3/io.h>
#include <stdio.h>

/* struct t_mem_area root_area; */

unsigned long int round_up_to_4KiB(unsigned long int i);

void init_memmgr(struct t_mem_area *root_area) {
        root_area->start = 0;
        root_area->size = 0;
        root_area->next = 0;
}

const int page_sz = 4096;

unsigned long int round_up_to_4KiB(unsigned long int i) {
        unsigned long int align_i = i % page_sz;
        /* io_printf("Aligned i at %d bytes boundry: %lu, rest: %lu \n", page_sz, i, align_i); */
        if(align_i)
                i += page_sz - align_i;
        /*io_printf(" i: %lu \n", i); */
        return i;
}

/* seek_free_mem - S�ker efter fritt utrymme f�r ett block med storleken i_size.
     Startar s�kningen efter ett fritt utrymme vid start_pos och letar vid varje sida om den �r fri
         eller inte. Om den st�ter p� ett allokerar utrymme, �ka p� start_pos det utrymmets storlek
         till start_pos. */
void * seek_free_mem(struct t_mem_area *root_area, unsigned int i_size) {

        unsigned long int start_pos = 0;
        struct t_mem_area *el = root_area;
        unsigned int i_start = (unsigned int) start_pos;
        unsigned int el_start = (unsigned int) el->start;

        i_size = round_up_to_4KiB(i_size);

        while(el->next) {
                el = (struct t_mem_area*)el->next;
                i_start = start_pos;
                el_start = (unsigned int) el->start;

                if(((i_start >= el_start) && (i_start <= (el->size+el_start)))
                        || (((i_size+i_start) >= el_start) && ((i_size+i_start) <= (el->size+el_start))) ) {
                        start_pos += el->size+el_start; /* Omr�det anv�nds. */

                }
                if((i_start <= el_start) && ((i_size+i_start) >= (el->size+el_start)))
                        start_pos += el->size+el_start;
        }

        /*el = (struct t_mem_area*)el->next;*/

        i_start = start_pos;
        el_start = (unsigned int) el->start;

        if(((i_start >= el_start) && (i_start <= (el->size+el_start)))
                || (((i_size+i_start) >= el_start) && ((i_size+i_start) <= (el->size+el_start))) ) {
                start_pos += el->size+el_start; /* Omr�det anv�nds. */

        }
        if((i_start <= el_start) && ((i_size+i_start) >= (el->size+el_start)))
                start_pos += el->size+el_start;


        start_pos = round_up_to_4KiB(start_pos);
        /* io_printf(" start_pos: %lu \n", start_pos); */
        return (void *)start_pos;
}

/* is_mem_used   - Tar reda p� om ett minnesomr�de redan �r upptaget, returnerar i
                 s� fall SANT annars FALSKT. */
int   is_mem_used(struct t_mem_area *root_area, void * p_start, unsigned int i_size) {

        struct t_mem_area *el = root_area;
        unsigned int i_start = (unsigned int) p_start;
        unsigned int el_start = (unsigned int) el->start;
        /* io_printf("Is allocated?: %p - 0x%lx \n", p_start, (unsigned long int)i_start+i_size); */
        /* print_used_mem(root_area); */
        while(el->next) {
                el = (struct t_mem_area*)el->next;
                i_start = (unsigned int) p_start;
                el_start = (unsigned int) el->start;

                /*io_printf("if((%d >= %d) && (%d <= %d)\n", i_start, el_start, i_start, el->size+el_start);
                io_printf("        && (%d >= %d) && (%d <= %d) )\n",
                                i_size+i_start, el_start, i_size+i_start, el->size+el_start );*/


                /* Om b�rjan p� blocket �r lika med eller st�rre �n el->start
                   och b�rjan p� blocket �r mindre �n eller lika med el->size. */
                if(((i_start >= el_start) && (i_start <= (el->size+el_start)))
                        || (((i_size+i_start) >= el_start) && ((i_size+i_start) <= (el->size+el_start))) ) {
                        /* io_printf("Yes1.\n"); */
                        return 1; /* Omr�det anv�nds. */
                }
                if((i_start <= el_start) && ((i_size+i_start) >= (el->size+el_start))) {
                        /* io_printf("Yes2.\n"); */
                        return 1;
                }
                /* io_printf("No, continue.\n"); */

        }
        i_start = (unsigned int) p_start;
        el_start = (unsigned int) el->start;

        /*io_printf("if((%d >= %d) && (%d <= %d)\n", i_start, el_start, i_start, el->size+el_start);
        io_printf("        && (%d >= %d) && (%d <= %d) )\n",
                                i_size+i_start, el_start, i_size+i_start, el->size+el_start );*/

        if(((i_start >= el_start) && (i_start <= (el->size+el_start)))
                        || (((i_size+i_start) >= el_start) && ((i_size+i_start) <= (el->size+el_start))) ) {
                /* io_printf("Yes3.\n"); */
                return 1; /* Omr�det anv�nds. */
        }
        if((i_start <= el_start) && ((i_size+i_start) >= (el->size+el_start))) {
                /* io_printf("Yes4.\n"); */
                return 1;
        }
        /* io_printf("No, do exit.\n"); */
        return 0;
}


/* alloc_mem_area- Allokerar ett omr�de, returnerar sant om det gick annars
                 falskt. */
int   alloc_mem_area(struct t_mem_area *root_area, void * p_start, unsigned int i_size)
{
  struct t_mem_area *rot;
  struct t_mem_area * prev;
  struct t_mem_area* p_tmp;

        if(is_mem_used(root_area, p_start, i_size))
                return 0;
        rot = root_area;
        prev = rot;
        while(rot->next) {
                prev = rot;
                rot = (struct t_mem_area*)rot->next;
        }

        p_tmp = (struct t_mem_area*) malloc(sizeof(struct t_mem_area));
        if(!p_tmp) /* Inget minne allokerat f�r noden.*/
                return 0;
        rot->next = (void*) p_tmp;
        p_tmp->start = p_start;
        p_tmp->size = i_size;
        p_tmp->next = 0; /* Initialize next. */
        return 1;
}

/* dealloc_mem_area - Avallokerar minnesomr�det pekaren pekar p�. */
int   dealloc_mem_area(struct t_mem_area *root_area, void * p_start) {
        io_printf("FIXME: memmgr.c:dealloc_mem_area()\n");
        return 0;
}

void  print_used_mem(struct t_mem_area *root_area) {


        unsigned long int start_pos = 0;
        struct t_mem_area *el = root_area;
        unsigned int i_start = (unsigned int) start_pos;
        unsigned int el_start = (unsigned int) el->start;
        io_printf("--- Used Mem in Memmgr ---\n");
        while(el || el->next) {

                i_start = start_pos;
                el_start = (unsigned int) el->start;

                io_printf("Allocated: %p - 0x%lx \n", el->start, (unsigned long int)el->size+el_start);
                if(!el->next)
                        break;
                if(el->next)
                        el = (struct t_mem_area*)el->next;
        }
        io_printf("---    End Mem dump    ---\n");
} 
