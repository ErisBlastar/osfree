/*
 * Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.
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
#include "metadata.h"
#include "toolcontext.h"
#include "lv_alloc.h"
#include "pv_alloc.h"
#include "str_list.h"
#include "segtype.h"

/*
 * Attempt to merge two adjacent segments.
 * Currently only supports striped segments on AREA_PV.
 * Returns success if successful, in which case 'first'
 * gets adjusted to contain both areas.
 */
static int _merge(struct lv_segment *first, struct lv_segment *second)
{
        if (!first || !second || first->segtype != second->segtype ||
            !first->segtype->ops->merge_segments) return 0;

        return first->segtype->ops->merge_segments(first, second);
}

int lv_merge_segments(struct logical_volume *lv)
{
        struct list *segh, *t;
        struct lv_segment *current, *prev = NULL;

        if (lv->status & LOCKED || lv->status & PVMOVE)
                return 1;

        list_iterate_safe(segh, t, &lv->segments) {
                current = list_item(segh, struct lv_segment);

                if (_merge(prev, current))
                        list_del(&current->list);
                else
                        prev = current;
        }

        return 1;
}

/*
 * Verify that an LV's segments are consecutive, complete and don't overlap.
 */
int check_lv_segments(struct logical_volume *lv, int complete_vg)
{
        struct lv_segment *seg, *seg2;
        uint32_t le = 0;
        unsigned seg_count = 0;
        int r = 1;
        uint32_t area_multiplier, s;

        //list_iterate_items(seg, &lv->segments) {
        list_iterate_items(seg, struct lv_segment, &lv->segments) {
                seg_count++;
                if (seg->le != le) {
                        log_error("LV %s invalid: segment %u should begin at "
                                  "LE %" PRIu32 " (found %" PRIu32 ").",
                                  lv->name, seg_count, le, seg->le);
                        r = 0;
                }

                area_multiplier = segtype_is_striped(seg->segtype) ?
                                        seg->area_count : 1;

                if (seg->area_len * area_multiplier != seg->len) {
                        log_error("LV %s: segment %u has inconsistent "
                                  "area_len %u",
                                  lv->name, seg_count, seg->area_len);
                        r = 0;
                }

                if (complete_vg && seg->log_lv) {
                        if (!seg_is_mirrored(seg)) {
                                log_error("LV %s: segment %u has log LV but "
                                          "is not mirrored",
                                          lv->name, seg_count);
                                r = 0;
                        }

                        if (!(seg->log_lv->status & MIRROR_LOG)) {
                                log_error("LV %s: segment %u log LV %s is not "
                                          "a mirror log",
                                           lv->name, seg_count, seg->log_lv->name);
                                r = 0;
                        }

                        if (!(seg2 = first_seg(seg->log_lv)) ||
                            seg2->mirror_seg != seg) {
                                log_error("LV %s: segment %u log LV does not "
                                          "point back to mirror segment",
                                           lv->name, seg_count);
                                r = 0;
                        }
                }

                if (complete_vg && seg->status & MIRROR_IMAGE) {
                        if (!seg->mirror_seg ||
                            !seg_is_mirrored(seg->mirror_seg)) {
                                log_error("LV %s: segment %u mirror image "
                                          "is not mirrored",
                                          lv->name, seg_count);
                                r = 0;
                        }
                }

                for (s = 0; s < seg->area_count; s++) {
                        if (seg_type(seg, s) == AREA_UNASSIGNED) {
                                log_error("LV %s: segment %u has unassigned "
                                          "area %u.",
                                          lv->name, seg_count, s);
                                r = 0;
                        } else if (seg_type(seg, s) == AREA_PV) {
                                if (!seg_pvseg(seg, s) ||
                                    seg_pvseg(seg, s)->lvseg != seg ||
                                    seg_pvseg(seg, s)->lv_area != s) {
                                        log_error("LV %s: segment %u has "
                                                  "inconsistent PV area %u",
                                                  lv->name, seg_count, s);
                                        r = 0;
                                }
                        } else {
                                if (!seg_lv(seg, s) ||
                                    seg_lv(seg, s)->vg != lv->vg ||
                                    seg_lv(seg, s) == lv) {
                                        log_error("LV %s: segment %u has "
                                                  "inconsistent LV area %u",
                                                  lv->name, seg_count, s);
                                        r = 0;
                                }

                                if (complete_vg && seg_lv(seg, s) &&
                                    (seg_lv(seg, s)->status & MIRROR_IMAGE) &&
                                    (!(seg2 = find_seg_by_le(seg_lv(seg, s),
                                                            seg_le(seg, s))) ||
                                     seg2->mirror_seg != seg)) {
                                        log_error("LV %s: segment %u mirror "
                                                  "image %u missing mirror ptr",
                                                  lv->name, seg_count, s);
                                        r = 0;
                                }

/* FIXME I don't think this ever holds?
                                if (seg_le(seg, s) != le) {
                                        log_error("LV %s: segment %u has "
                                                  "inconsistent LV area %u "
                                                  "size",
                                                  lv->name, seg_count, s);
                                        r = 0;
                                }
 */
                        }
                }

                le += seg->len;
        }

        if (le != lv->le_count) {
                log_error("LV %s: inconsistent LE count %u != %u",
                          lv->name, le, lv->le_count);
                r = 0;
        }

        return r;
}

/*
 * Split the supplied segment at the supplied logical extent
 * NB Use LE numbering that works across stripes PV1: 0,2,4 PV2: 1,3,5 etc.
 */
static int _lv_split_segment(struct logical_volume *lv, struct lv_segment *seg,
                             uint32_t le)
{
        struct lv_segment *split_seg;
        uint32_t s;
        uint32_t offset = le - seg->le;
        uint32_t area_offset;

        if (!seg_can_split(seg)) {
                log_error("Unable to split the %s segment at LE %" PRIu32
                          " in LV %s", seg->segtype->name, le, lv->name);
                return 0;
        }

        /* Clone the existing segment */
        if (!(split_seg = alloc_lv_segment(lv->vg->cmd->mem, seg->segtype,
                                           seg->lv, seg->le, seg->len,
                                           seg->status, seg->stripe_size,
                                           seg->log_lv,
                                           seg->area_count, seg->area_len,
                                           seg->chunk_size, seg->region_size,
                                           seg->extents_copied))) {
                log_error("Couldn't allocate cloned LV segment.");
                return 0;
        }

        if (!str_list_dup(lv->vg->cmd->mem, &split_seg->tags, &seg->tags)) {
                log_error("LV segment tags duplication failed");
                return 0;
        }

        /* In case of a striped segment, the offset has to be / stripes */
        area_offset = offset;
        if (seg_is_striped(seg))
                area_offset /= seg->area_count;

        split_seg->area_len -= area_offset;
        seg->area_len = area_offset;

        split_seg->len -= offset;
        seg->len = offset;

        split_seg->le = seg->le + seg->len;

        /* Adjust the PV mapping */
        for (s = 0; s < seg->area_count; s++) {
                seg_type(split_seg, s) = seg_type(seg, s);

                /* Split area at the offset */
                switch (seg_type(seg, s)) {
                case AREA_LV:
                        seg_lv(split_seg, s) = seg_lv(seg, s);
                        seg_le(split_seg, s) =
                            seg_le(seg, s) + seg->area_len;
                        log_debug("Split %s:%u[%u] at %u: %s LE %u", lv->name,
                                  seg->le, s, le, seg_lv(seg, s)->name,
                                  seg_le(split_seg, s));
                        break;

                case AREA_PV:
                        if (!(seg_pvseg(split_seg, s) =
                             assign_peg_to_lvseg(seg_pv(seg, s),
                                                 seg_pe(seg, s) +
                                                     seg->area_len,
                                                 seg_pvseg(seg, s)->len -
                                                     seg->area_len,
                                                 split_seg, s))) {
                                stack;
                                return 0;
                        }
                        log_debug("Split %s:%u[%u] at %u: %s PE %u", lv->name,
                                  seg->le, s, le,
                                  dev_name(seg_dev(seg, s)),
                                  seg_pe(split_seg, s));
                        break;

                case AREA_UNASSIGNED:
                        log_error("Unassigned area %u found in segment", s);
                        return 0;
                }
        }

        /* Add split off segment to the list _after_ the original one */
        list_add_h(&seg->list, &split_seg->list);

        return 1;
}

/*
 * Ensure there's a segment boundary at the given logical extent
 */
int lv_split_segment(struct logical_volume *lv, uint32_t le)
{
        struct lv_segment *seg;

        if (!(seg = find_seg_by_le(lv, le))) {
                log_error("Segment with extent %" PRIu32 " in LV %s not found",
                          le, lv->name);
                return 0;
        }

        /* This is a segment start already */
        if (le == seg->le)
                return 1;

        if (!_lv_split_segment(lv, seg, le)) {
                stack;
                return 0;
        }

        if (!vg_validate(lv->vg)) {
                stack;
                return 0;
        }

        return 1;
}
