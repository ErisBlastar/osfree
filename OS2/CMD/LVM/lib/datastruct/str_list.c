/*
 * Copyright (C) 2003-2004 Sistina Software, Inc. All rights reserved.
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * This file is part of LVM2.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "lib.h"
#include "str_list.h"

#include "libdevmapper.h"

struct list *str_list_create(struct dm_pool *mem)
{
        struct list *sl;

        if (!(sl = dm_pool_alloc(mem, sizeof(struct list)))) {
                stack;
                return NULL;
        }

        list_init(sl);

        return sl;
}

int str_list_add(struct dm_pool *mem, struct list *sll, const char *str)
{
        struct str_list *sln;

        if (!str) {
                stack;
                return 0;
        }

        /* Already in list? */
        if (str_list_match_item(sll, str))
                return 1;

        if (!(sln = dm_pool_alloc(mem, sizeof(*sln)))) {
                stack;
                return 0;
        }

        sln->str = str;
        list_add(sll, &sln->list);

        return 1;
}

int str_list_del(struct list *sll, const char *str)
{
        struct list *slh, *slht;

        list_iterate_safe(slh, slht, sll) {
                if (!strcmp(str, list_item(slh, struct str_list)->str))
                         list_del(slh);
        }

        return 1;
}

int str_list_dup(struct dm_pool *mem, struct list *sllnew, struct list *sllold)
{
        struct str_list *sl;

        list_init(sllnew);

        //list_iterate_items(sl, sllold) {
        list_iterate_items(sl, struct str_list, sllold) {
                if (!str_list_add(mem, sllnew, strdup(sl->str))) {
                        stack;
                        return 0;
                }
        }

        return 1;
}

/*
 * Is item on list?
 */
int str_list_match_item(struct list *sll, const char *str)
{
        struct str_list *sl;

        //list_iterate_items(sl, sll)
        list_iterate_items(sl, struct str_list, sll)
            if (!strcmp(str, sl->str))
                return 1;

        return 0;
}

/*
 * Is at least one item on both lists?
 */
int str_list_match_list(struct list *sll, struct list *sll2)
{
        struct str_list *sl;

        //list_iterate_items(sl, sll)
        list_iterate_items(sl, struct str_list, sll)
            if (str_list_match_item(sll2, sl->str))
                return 1;

        return 0;
}

/*
 * Do both lists contain the same set of items?
 */
int str_list_lists_equal(struct list *sll, struct list *sll2)
{
        struct str_list *sl;

        if (list_size(sll) != list_size(sll2))
                return 0;

        //list_iterate_items(sl, sll)
        list_iterate_items(sl, struct str_list, sll)
            if (!str_list_match_item(sll2, sl->str))
                return 0;

        return 1;
}
