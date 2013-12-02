/*

gbmmap.c - Map RGBA to palette or BGRx bitmap data

*/

#include <stdlib.h>
#include <string.h>
#include "gbm.h"
#include "gbmmap.h"


#define	GetR(rgba)  ((rgba)         & 0xff)
#define	GetG(rgba)  (((rgba) >> 8)  & 0xff)
#define	GetB(rgba)  (((rgba) >> 16) & 0xff)
#define	GetA(rgba)  (((rgba) >> 24) & 0xff)

#define GetRGB(rgba)    ((rgba) & 0xffffff)
#define MakeRGB(r,g,b)  (((gbm_u32) r) | ((gbm_u32) g << 8) | ((gbm_u32) b << 16))

#define CVT(x) (((x) * 255) / ((1L << 16) - 1))

/* ----------------------------------------------------------- */

/* Convert bitmap data with palette of one row to another palette depth.
 * The destination buffer must be different to the source buffer.
 *
 * Supported are: 2 bpp_pal -> 4 bpp_pal
 */
gbm_boolean gbm_map_row_PAL_PAL(const gbm_u8 * data_src, const GBM * gbm_src,
                                      gbm_u8 * data_dst, const GBM * gbm_dst)
{
   int  colorIndex, x;

   if ((gbm_src->w != gbm_dst->w) || (data_src == data_dst))
   {
      return GBM_FALSE;
   }

   switch(gbm_src->bpp)
   {
      case 2: /* 2 bpp -> 4 bpp */
         if (gbm_dst->bpp != 4)
         {
            return GBM_FALSE;
         }

         for (x = 0; x < gbm_src->w - 4; x += 4)
         {
            /* get the color index */
            colorIndex = *data_src++;

            *data_dst++ = ((colorIndex >> 6) << 4)       | ((colorIndex >> 4) & 3);
            *data_dst++ = (((colorIndex >> 2) & 3) << 4) | (colorIndex & 3);
         }

         if (x < gbm_src->w)
         {
            /* get the color index */
            colorIndex = *data_src++;

            *data_dst  = (colorIndex >> 6) << 4;
            x++;

            if (x < gbm_src->w)
            {
               *data_dst++ = *data_dst | ((colorIndex >> 4) & 3);
               x++;

               if (x < gbm_src->w)
               {
                  *data_dst = ((colorIndex >> 2) & 3) << 4;
                  x++;

                  if (x < gbm_src->w)
                  {
                     *data_dst = *data_dst | (colorIndex & 3);
                  }
               }
            }
         }
         break;

      default:
         return GBM_FALSE;
   }

   return GBM_TRUE;
}

/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

/* Convert bitmap data with palette of one row to BGR.
 * The destination buffer must be different to the source buffer.
 *
 * Supported are: 1 bpp_pal -> 24 bpp
 *                2 bpp_pal -> 24 bpp
 *                4 bpp_pal -> 24 bpp
 *                8 bpp_pal -> 24 bpp
 *               16 bpp_pal -> 24 bpp
 *               16 bpp_pal -> 48 bpp
 */
gbm_boolean gbm_map_row_PAL_BGR(const gbm_u8       * data_src, const GBM * gbm_src,
                                      gbm_u8       * data_dst, const GBM * gbm_dst,
                                const GBMRGB_16BPP * gbmrgb_src)
{
   int  block_count = gbm_src->w;
   int  colorIndex, x, s;
   gbm_u8 c;

         gbm_u16 * data16_dst =       (gbm_u16 *) data_dst;
   const gbm_u16 * data16_src = (const gbm_u16 *) data_src;
   const GBMRGB_16BPP * pRGB;

   if ((gbm_src->w != gbm_dst->w) || (data_src == data_dst))
   {
      return GBM_FALSE;
   }

   switch(gbm_src->bpp)
   {
      case 1:
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }
         for (x = 0; x < gbm_src->w; x++)
         {
            /* get the color index */
            if ( (x & 7) == 0 )
            {
               c = *data_src++;
            }
            else
            {
               c <<= 1;
            }

            pRGB = &gbmrgb_src[c >> 7];
            *data_dst++ = (gbm_u8) pRGB->b;
            *data_dst++ = (gbm_u8) pRGB->g;
            *data_dst++ = (gbm_u8) pRGB->r;
         }
         break;

      case 2:
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }

         while (block_count > 0)
         {
            /* get the color index */
            colorIndex = *data_src++;

            s = 6;
            while ((s >= 0) && (block_count > 0))
            {
               --block_count;

               pRGB = &gbmrgb_src[(colorIndex >> s) & 3];
               *data_dst++ = (gbm_u8) pRGB->b;
               *data_dst++ = (gbm_u8) pRGB->g;
               *data_dst++ = (gbm_u8) pRGB->r;
               s -= 2;
            }
         }
         break;

      case 4:
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }
         for (x = 0; x + 1 < gbm_src->w; x += 2)
         {
            /* get the color index */
            colorIndex = *data_src++;

            pRGB = &gbmrgb_src[colorIndex >> 4];
            *data_dst++ = (gbm_u8) pRGB->b;
            *data_dst++ = (gbm_u8) pRGB->g;
            *data_dst++ = (gbm_u8) pRGB->r;

            pRGB = &gbmrgb_src[colorIndex & 15];
            *data_dst++ = (gbm_u8) pRGB->b;
            *data_dst++ = (gbm_u8) pRGB->g;
            *data_dst++ = (gbm_u8) pRGB->r;
         }

         if (x < gbm_src->w)
         {
            /* get the color index */
            colorIndex = *data_src++;

            pRGB = &gbmrgb_src[colorIndex >> 4];
            *data_dst++ = (gbm_u8) pRGB->b;
            *data_dst++ = (gbm_u8) pRGB->g;
            *data_dst++ = (gbm_u8) pRGB->r;
         }
         break;

      case 8:
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }
         while (block_count > 0)
         {
            --block_count;

            /* get the color index */
            colorIndex = *data_src++;

            *data_dst++ = (gbm_u8) gbmrgb_src[colorIndex].b;
            *data_dst++ = (gbm_u8) gbmrgb_src[colorIndex].g;
            *data_dst++ = (gbm_u8) gbmrgb_src[colorIndex].r;
         }
         break;

      case 16:
         switch(gbm_dst->bpp)
         {
            case 24:
               while (block_count > 0)
               {
                  --block_count;

                  /* get the color index */
                  colorIndex = *data16_src++;

                  *data_dst++ = (gbm_u8) CVT(gbmrgb_src[colorIndex].b);
                  *data_dst++ = (gbm_u8) CVT(gbmrgb_src[colorIndex].g);
                  *data_dst++ = (gbm_u8) CVT(gbmrgb_src[colorIndex].r);
              }
              break;

            case 48:
               while (block_count > 0)
               {
                  --block_count;

                  /* get the color index */
                  colorIndex = *data16_src++;

                  *data16_dst++ = gbmrgb_src[colorIndex].b;
                  *data16_dst++ = gbmrgb_src[colorIndex].g;
                  *data16_dst++ = gbmrgb_src[colorIndex].r;
              }
              break;

            default:
               return GBM_FALSE;
         }
         break;

      default:
         return GBM_FALSE;
   }

   return GBM_TRUE;
}

/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

/* Convert bitmap data of one row from RGBx to BGRx.
 *
 * Supported are: 24->24
 *                32->24
 *                32->32
 *                48->24
 *                48->48
 *                64->24
 *                64->48
 *                64->64
 */
gbm_boolean gbm_map_row_RGBx_BGRx(const gbm_u8       * data_src, const GBM * gbm_src,
                                        gbm_u8       * data_dst, const GBM * gbm_dst,
                                  const GBMRGB_16BPP * back_rgb, const gbm_boolean unassociatedAlpha)
{
   int block_count = gbm_src->w;

   const gbm_u32 * data32_src = (const gbm_u32 *) data_src;
   const gbm_u16  * data16_src = (const gbm_u16 *)  data_src;
         gbm_u16  * data16_dst =       (gbm_u16 *)  data_dst;

   gbm_u16  data16, data16_1, data16_2, alpha16;
   gbm_u32 data32;

   gbm_u8 r,g,b, alpha;

   if (gbm_src->w != gbm_dst->w)
   {
      return GBM_FALSE;
   }

   switch(gbm_src->bpp)
   {
      case 24: /* 24->24 */
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }

         while (block_count > 0)
         {
            --block_count;

            r = *data_src++;
            g = *data_src++;
            b = *data_src++;

            *data_dst++ = b; /* blue */
            *data_dst++ = g;
            *data_dst++ = r;
         }
         break;

      /* ------------- */

      case 32:
         switch(gbm_dst->bpp)
         {
            case 24: /* 32->24 */
               if (unassociatedAlpha)
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     data32 = *data32_src++;

                     /* calc alpha channel against background color */
                     alpha       = GetA(data32);
                     alpha_mult  = (gbm_u16) alpha + 1;
                     alpha_diff  = 255 - alpha;

                     *data_dst++ = ((gbm_u16) GetB(data32) * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) GetG(data32) * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) GetR(data32) * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                  }
               }
               else
               {
                  /* strip alpha channel */
                  while (block_count > 0)
                  {
                     --block_count;

                     data32 = *data32_src++;

                     *data_dst++ = GetB(data32);
                     *data_dst++ = GetG(data32);
                     *data_dst++ = GetR(data32);
                  }
               }
               break;

            /* ------------- */

            case 32: /* 32->32 */
               if (unassociatedAlpha)
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     data32 = *data32_src++;

                     /* calc alpha channel against background color */
                     alpha       = GetA(data32);
                     alpha_mult  = (gbm_u16) alpha + 1;
                     alpha_diff  = 255 - alpha;

                     *data_dst++ = ((gbm_u16) GetB(data32) * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) GetG(data32) * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) GetR(data32) * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                     *data_dst++ = alpha;
                  }
               }
               else
               {
                  while (block_count > 0)
                  {
                     --block_count;

                     data32 = *data32_src++;

                     *data_dst++ = GetB(data32);
                     *data_dst++ = GetG(data32);
                     *data_dst++ = GetR(data32);
                     *data_dst++ = GetA(data32);
                  }
               }
               break;

            /* ------------- */

            default:
               return GBM_FALSE;
         }
         break;

      /* ------------- */

      case 48:
         switch(gbm_dst->bpp)
         {
            case 24: /* 48->24 */
               while (block_count > 0)
               {
                  --block_count;

                  data16    = *data16_src++; /* red   */
                  data16_1  = *data16_src++; /* green */
                  data16_2  = *data16_src++; /* blue  */

                  *data_dst++ = CVT(data16_2); /* blue  */
                  *data_dst++ = CVT(data16_1); /* green */
                  *data_dst++ = CVT(data16);   /* red   */
               }
               break;

            /* ------------- */

            case 48: /* 48->48 */
               while (block_count > 0)
               {
                  --block_count;

                  data16    = *data16_src++; /* red   */
                  data16_1  = *data16_src++; /* green */
                  data16_2  = *data16_src++; /* blue  */

                  *data16_dst++ = data16_2; /* blue  */
                  *data16_dst++ = data16_1; /* green */
                  *data16_dst++ = data16;   /* red   */
               }
               break;

            /* ------------- */

            default:
               return GBM_FALSE;
         }
         break;

      /* ------------- */

      case 64:
         switch(gbm_dst->bpp)
         {
            case 24: /* 64->24 */
               if (unassociatedAlpha)
               {
                  gbm_u32 alpha_mult;
                  gbm_u16 alpha_diff;
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     alpha16  = *data16_src++; /* alpha */

                     alpha_mult = (gbm_u32) alpha16 + 1;
                     alpha_diff = 65535 - alpha16;

                     /* calc alpha channel against background color */
                     *data_dst++ = CVT(((gbm_u32) data16_2 * alpha_mult + back_rgb->b * alpha_diff) >> 16);
                     *data_dst++ = CVT(((gbm_u32) data16_1 * alpha_mult + back_rgb->g * alpha_diff) >> 16);
                     *data_dst++ = CVT(((gbm_u32) data16   * alpha_mult + back_rgb->r * alpha_diff) >> 16);
                  }
               }
               else
               {
                  /* strip alpha channel */
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     data16_src++;             /* skip alpha */

                     *data_dst++ = CVT(data16_2); /* blue  */
                     *data_dst++ = CVT(data16_1); /* green */
                     *data_dst++ = CVT(data16);   /* red   */
                  }
               }
               break;

            /* ------------- */

            case 48: /* 64->48 */
               if (unassociatedAlpha)
               {
                  gbm_u32 alpha_mult;
                  gbm_u16  alpha_diff;
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     alpha16  = *data16_src++; /* alpha */

                     alpha_mult = (gbm_u32) alpha16 + 1;
                     alpha_diff = 65535 - alpha16;

                     /* calc alpha channel against background color */
                     *data16_dst++ = ((gbm_u32) data16_2 * alpha_mult + back_rgb->b * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) data16_1 * alpha_mult + back_rgb->g * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) data16   * alpha_mult + back_rgb->r * alpha_diff) >> 16;
                  }
               }
               else
               {
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     data16_src++;             /* skip alpha */

                     *data16_dst++ = data16_2; /* blue  */
                     *data16_dst++ = data16_1; /* green */
                     *data16_dst++ = data16;   /* red   */
                  }
               }
               break;

            /* ------------- */

            case 64: /* 64->64 */
               if (unassociatedAlpha)
               {
                  gbm_u32 alpha_mult;
                  gbm_u16  alpha_diff;
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     alpha16  = *data16_src++; /* alpha */

                     alpha_mult = (gbm_u32) alpha16 + 1;
                     alpha_diff = 65535 - alpha16;

                     /* calc alpha channel against background color */
                     *data16_dst++ = ((gbm_u32) data16_2 * alpha_mult + back_rgb->b * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) data16_1 * alpha_mult + back_rgb->g * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) data16   * alpha_mult + back_rgb->r * alpha_diff) >> 16;
                     *data16_dst++ = alpha16;
                  }
               }
               else
               {
                  while (block_count > 0)
                  {
                     --block_count;

                     data16   = *data16_src++; /* red   */
                     data16_1 = *data16_src++; /* green */
                     data16_2 = *data16_src++; /* blue  */
                     alpha16  = *data16_src++; /* alpha */

                     *data16_dst++ = data16_2; /* blue  */
                     *data16_dst++ = data16_1; /* green */
                     *data16_dst++ = data16;   /* red   */
                     *data16_dst++ = alpha16;  /* alpha */
                  }
               }
               break;

            /* ------------- */

            default:
               return GBM_FALSE;
         }
         break;

      /* ------------- */

      default:
         return GBM_FALSE;
   }

   return GBM_TRUE;
}


/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

/* Convert the CMYK data in one plane  into one RGB plane.
 *
 * Supports:  8, 8, 8, 8 -> 24 bpp
 *           16,16,16,16 -> 24 bpp
 *           16,16,16,16 -> 48 bpp
 *
 * Set component_inverse to GBM_TRUE to calc with (255-component) rather than component directly.
 */
gbm_boolean gbm_map_row_CMYK_to_BGR(const gbm_u8 * data_src, const GBM * gbm_src,
                                          gbm_u8 * data_dst, const GBM * gbm_dst,
                                    const gbm_boolean component_inverse)
{
   int block_count = gbm_src->w;

   const gbm_u16  * data16_src = (const gbm_u16 *)  data_src;
         gbm_u16  * data16_dst =       (gbm_u16 *)  data_dst;

   gbm_u16 r,g,b, c,m,y,k;
   gbm_u32 r32,g32,b32;

   if (gbm_src->w != gbm_dst->w)
   {
      return GBM_FALSE;
   }

   switch(gbm_src->bpp)
   {
      case 32: /* 8,8,8,8 -> 24 bpp */
         if (gbm_dst->bpp != 24)
         {
            return GBM_FALSE;
         }

         while (block_count > 0)
         {
            --block_count;

            c = *data_src++;
            m = *data_src++;
            y = *data_src++;
            k = *data_src++;

            if (component_inverse)
            {
               k = 255 - k;
               r = (k * (255 - c)) / 255;
               g = (k * (255 - m)) / 255;
               b = (k * (255 - y)) / 255;
            }
            else
            {
               r = (k * c) / 255;
               g = (k * m) / 255;
               b = (k * y) / 255;
            }

            *data_dst++ = (gbm_u8) b;
            *data_dst++ = (gbm_u8) g;
            *data_dst++ = (gbm_u8) r;
         }
         break;

      /* ------------- */

      case 64:
         switch(gbm_dst->bpp)
         {
            case 24: /* 16,16,16,16 -> 24 bpp */
               while (block_count > 0)
               {
                  --block_count;

                  c = *data16_src++;
                  m = *data16_src++;
                  y = *data16_src++;
                  k = *data16_src++;

                  c = CVT(c);
                  m = CVT(m);
                  y = CVT(y);
                  k = CVT(k);

                  if (component_inverse)
                  {
                     k = 255 - k;
                     r = (k * (255 - c)) / 255;
                     g = (k * (255 - m)) / 255;
                     b = (k * (255 - y)) / 255;
                  }
                  else
                  {
                     r = (k * c) / 255;
                     g = (k * m) / 255;
                     b = (k * y) / 255;
                  }

                  *data_dst++ = (gbm_u8) b;
                  *data_dst++ = (gbm_u8) g;
                  *data_dst++ = (gbm_u8) r;
               }
               break;

            /* ------------- */

            case 48: /* 16,16,16,16 -> 48 bpp */
               while (block_count > 0)
               {
                  --block_count;

                  c = *data16_src++;
                  m = *data16_src++;
                  y = *data16_src++;
                  k = *data16_src++;

                  if (component_inverse)
                  {
                     k   = 65535 - k;
                     r32 = (k * (65535 - c)) / 65535;
                     g32 = (k * (65535 - m)) / 65535;
                     b32 = (k * (65535 - y)) / 65535;
                  }
                  else
                  {
                     r32 = (k * c) / 65535;
                     g32 = (k * m) / 65535;
                     b32 = (k * y) / 65535;
                  }

                  *data16_dst++ = (gbm_u16) b32;
                  *data16_dst++ = (gbm_u16) g32;
                  *data16_dst++ = (gbm_u16) r32;
               }
               break;

            /* ------------- */

            default:
               return GBM_FALSE;
         }
         break;

      /* ------------- */

      default:
         return GBM_FALSE;
   }

   return GBM_TRUE;
}


/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

/* Convert the current plane into one RGB plane.
 *
 * The following order of calls must be preserved:
 * sample = 0: C
 * sample = 1: M
 * sample = 2: Y
 * sample = 3: K
 *
 * Supports:  8, 8, 8, 8 -> 24 bpp
 *           16,16,16,16 -> 24 bpp
 *           16,16,16,16 -> 48 bpp
 *
 * Set component_inverse to GBM_TRUE to calc with (255-component) rather than component directly.
 */
gbm_boolean gbm_map_sep_row_CMYK_to_BGR(const gbm_u8 * data_src, const GBM * gbm_src,
                                              gbm_u8 * data_dst, const GBM * gbm_dst,
                                        const int      sample  , const gbm_boolean component_inverse)
{
   int block_count = gbm_src->w;

   const gbm_u16  * data16_src = (const gbm_u16 *) data_src;
         gbm_u16  * data16_dst =       (gbm_u16 *) data_dst;

   if ((gbm_src->w != gbm_dst->w) || (sample < 0) || (sample > 3))
   {
      return GBM_FALSE;
   }

   /* for samples 0-2: copy into single plane target buffer */
   if (sample < 3)
   {
      switch(gbm_src->bpp)
      {
         case 32: /* 8,8,8,8 -> 24 bpp */
            if (gbm_dst->bpp != 24)
            {
               return GBM_FALSE;
            }
            data_dst += sample;

            while (block_count > 0)
            {
               --block_count;

               *data_dst  = *data_src++;
               data_dst  += 3;
            }
            break;

         case 64:
            switch(gbm_dst->bpp)
            {
               case 24: /* 16,16,16,16 -> 24 bpp */
                  data_dst += sample;

                  while (block_count > 0)
                  {
                     --block_count;

                     *data_dst  = CVT(*data16_src);
                     data_dst  += 3;
                     data16_src++;
                  }
                  break;

               case 48: /* 16,16,16,16 -> 48 bpp */
                  data16_dst += sample;

                  while (block_count > 0)
                  {
                     --block_count;

                     *data16_dst  = *data16_src++;
                     data16_dst  += 3;
                  }
                  break;

               default:
                  return GBM_FALSE;
            }
            break;

         default:
            return GBM_FALSE;
      }
   }
   else if (sample == 3)
   {
      /* now calculate the RGB values based on the data of the
       * stored CMY + the scanline K value
       */
       gbm_u16 r  ,g  ,b  , c,m,y,k;
      gbm_u32 r32,g32,b32;

      switch(gbm_src->bpp)
      {
         case 32: /* 8,8,8,8 -> 24 bpp */
            if (gbm_dst->bpp != 24)
            {
               return GBM_FALSE;
            }
            while (block_count > 0)
            {
               --block_count;

               c = *data_dst;
               m = *(data_dst + 1);
               y = *(data_dst + 2);
               k = *data_src++;

               if (component_inverse)
               {
                  k = 255 - k;
                  r = (k * (255 - c)) / 255;
                  g = (k * (255 - m)) / 255;
                  b = (k * (255 - y)) / 255;
               }
               else
               {
                  r = (k * c) / 255;
                  g = (k * m) / 255;
                  b = (k * y) / 255;
               }

               *data_dst++ = (gbm_u8) b;
               *data_dst++ = (gbm_u8) g;
               *data_dst++ = (gbm_u8) r;
            }
            break;

         case 64:
            switch(gbm_dst->bpp)
            {
               case 24: /* 16,16,16,16 -> 24 bpp */
                  while (block_count > 0)
                  {
                     --block_count;

                     c =     *data_dst;
                     m =     *(data_dst + 1);
                     y =     *(data_dst + 2);
                     k = CVT(*data16_src);
                     data16_src++;

                     if (component_inverse)
                     {
                        k = 255 - k;
                        r = (k * (255 - c)) / 255;
                        g = (k * (255 - m)) / 255;
                        b = (k * (255 - y)) / 255;
                     }
                     else
                     {
                        r = (k * c) / 255;
                        g = (k * m) / 255;
                        b = (k * y) / 255;
                     }

                     *data_dst++ = (gbm_u8) b;
                     *data_dst++ = (gbm_u8) g;
                     *data_dst++ = (gbm_u8) r;
                  }
                  break;

               case 48: /* 16,16,16,16 -> 48 bpp */
                  while (block_count > 0)
                  {
                     --block_count;

                     c = *data16_dst;
                     m = *(data16_dst + 1);
                     y = *(data16_dst + 2);
                     k = *data16_src++;

                     if (component_inverse)
                     {
                        k   = 65535 - k;
                        r32 = (k * (65535 - c)) / 65535;
                        g32 = (k * (65535 - m)) / 65535;
                        b32 = (k * (65535 - y)) / 65535;
                     }
                     else
                     {
                        r32 = (k * c) / 65535;
                        g32 = (k * m) / 65535;
                        b32 = (k * y) / 65535;
                     }

                     *data16_dst++ = (gbm_u16) b32;
                     *data16_dst++ = (gbm_u16) g32;
                     *data16_dst++ = (gbm_u16) r32;
                  }
                  break;

               default:
                  return GBM_FALSE;
            }
            break;

         default:
            return GBM_FALSE;
      }
   }

   return GBM_TRUE;
}

/* ----------------------------------------------------------- */

/* Convert the current plane into one RGB plane.
 *
 * The following order of calls must be preserved:
 * sample = 0: R
 * sample = 1: G
 * sample = 2: B
 * sample = 3: A (this is optional when an alpha channel exists)
 *
 * Supports:  8, 8, 8    -> 24 bpp
 *            8, 8, 8, 8 -> 24 bpp
 *           16,16,16    -> 24 bpp
 *           16,16,16,16 -> 24 bpp
 *           16,16,16    -> 48 bpp
 *           16,16,16,16 -> 48 bpp
 *           16,16,16,16 -> 64 bpp
 */
gbm_boolean gbm_map_sep_row_RGBx_BGRx(const gbm_u8       * data_src, const GBM * gbm_src,
                                            gbm_u8       * data_dst, const GBM * gbm_dst,
                                      const GBMRGB_16BPP * back_rgb, const gbm_boolean unassociatedAlpha,
                                      const int            sample)
{
   int block_count = gbm_src->w;

   gbm_u16 r,g,b,a;

   const gbm_u16  * data16_src = (const gbm_u16 *) data_src;
         gbm_u16  * data16_dst =       (gbm_u16 *) data_dst;

   if ((gbm_src->w != gbm_dst->w) || (sample < 0) || (sample > 3))
   {
      return GBM_FALSE;
   }

   /* for samples 0-2: copy into single plane target buffer */
   if (sample < 3)
   {
      switch(gbm_src->bpp)
      {
         case 24:
         case 32:
            switch(gbm_dst->bpp)
            {
               case 24: /* 8,8,8   -> 24 bpp */
               case 32: /* 8,8,8,8 -> 32 bpp */
                  data_dst += sample;

                  while (block_count > 0)
                  {
                     --block_count;

                     *data_dst  = *data_src++;
                     data_dst  += 3;
                  }
                  break;

               default:
                  return GBM_FALSE;
            }
            break;

         case 48:
         case 64:
            switch(gbm_dst->bpp)
            {
               case 24: /* 16,16,16    -> 24 bpp */
               case 32: /* 16,16,16,16 -> 32 bpp */
                  data_dst += sample;

                  while (block_count > 0)
                  {
                     --block_count;

                     *data_dst  = CVT(*data16_src);
                     data_dst  += 3;
                     data16_src++;
                  }
                  break;

               case 48: /* 16,16,16    -> 48 bpp */
               case 64: /* 16,16,16,16 -> 64 bpp */
                  data16_dst += sample;

                  while (block_count > 0)
                  {
                     --block_count;

                     *data16_dst  = *data16_src++;
                     data16_dst  += 3;
                  }
                  break;

               default:
                  return GBM_FALSE;
            }
            break;

         default:
            return GBM_FALSE;
      }
   }
   else if ((sample == 3) && unassociatedAlpha)
   {
      /* now calculate the RGB values based on the data of the
       * stored RGB + the Alpha value
       */
      switch(gbm_src->bpp)
      {
         case 32:
            switch(gbm_dst->bpp)
            {
               case 24: /* 8,8,8,8 -> 24 bpp */
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     r = *data_dst;
                     g = *(data_dst + 1);
                     b = *(data_dst + 2);
                     a = *data_src++;

                     alpha_mult = (gbm_u16) a + 1;
                     alpha_diff = 255 - a;

                     *data_dst++ = ((gbm_u16) b * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) g * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) r * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                  }
               }
               break;

               case 32: /* 8,8,8,8 -> 32 bpp */
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     r = *data_dst;
                     g = *(data_dst + 1);
                     b = *(data_dst + 2);
                     a = *data_src++;

                     alpha_mult = (gbm_u16) a + 1;
                     alpha_diff = 255 - a;

                     *data_dst++ = ((gbm_u16) b * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) g * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) r * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                     *data_dst++ = (gbm_u8) a;
                  }
               }
               break;

               default:
                  return GBM_FALSE;
            }
            break;

         case 64:
            switch(gbm_dst->bpp)
            {
               case 24: /* 16,16,16,16 -> 24 bpp */
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     r =     *data_dst;
                     g =     *(data_dst + 1);
                     b =     *(data_dst + 2);
                     a = CVT(*data16_src);
                     data16_src++;

                     alpha_mult = (gbm_u16) a + 1;
                     alpha_diff = 255 - a;

                     /* calc alpha channel against background color */
                     *data_dst++ = ((gbm_u16) b * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) g * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) r * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                  }
               }
               break;

               case 32: /* 16,16,16,16 -> 32 bpp */
               {
                  gbm_u16 alpha_mult;
                  gbm_u8 alpha_diff;
                  GBMRGB back_rgb8;
                  back_rgb8.r = CVT(back_rgb->r);
                  back_rgb8.g = CVT(back_rgb->g);
                  back_rgb8.b = CVT(back_rgb->b);

                  while (block_count > 0)
                  {
                     --block_count;

                     r =     *data_dst;
                     g =     *(data_dst + 1);
                     b =     *(data_dst + 2);
                     a = CVT(*data16_src);
                     data16_src++;

                     alpha_mult = (gbm_u16) a + 1;
                     alpha_diff = 255 - a;

                     /* calc alpha channel against background color */
                     *data_dst++ = ((gbm_u16) b * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) g * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
                     *data_dst++ = ((gbm_u16) r * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
                     *data_dst++ = (gbm_u8) a;
                  }
               }
               break;

               case 48: /* 16,16,16,16 -> 48 bpp */
               {
                  gbm_u32 alpha_mult;
                  gbm_u16  alpha_diff;
                  while (block_count > 0)
                  {
                     --block_count;

                     r = *data16_dst;
                     g = *(data16_dst + 1);
                     b = *(data16_dst + 2);
                     a = *data16_src++;

                     alpha_mult = (gbm_u32) a + 1;
                     alpha_diff = 65535 - a;

                     /* calc alpha channel against background color */
                     *data16_dst++ = ((gbm_u32) b * alpha_mult + back_rgb->b * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) g * alpha_mult + back_rgb->g * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) r * alpha_mult + back_rgb->r * alpha_diff) >> 16;
                  }
               }
               break;

               case 64: /* 16,16,16,16 -> 64 bpp */
               {
                  gbm_u32 alpha_mult;
                  gbm_u16  alpha_diff;
                  while (block_count > 0)
                  {
                     --block_count;

                     r = *data16_dst;
                     g = *(data16_dst + 1);
                     b = *(data16_dst + 2);
                     a = *data16_src++;

                     alpha_mult = (gbm_u32) a + 1;
                     alpha_diff = 65535 - a;

                     /* calc alpha channel against background color */
                     *data16_dst++ = ((gbm_u32) b * alpha_mult + back_rgb->b * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) g * alpha_mult + back_rgb->g * alpha_diff) >> 16;
                     *data16_dst++ = ((gbm_u32) r * alpha_mult + back_rgb->r * alpha_diff) >> 16;
                     *data16_dst++ = a;
                  }
               }
               break;

               default:
                  return GBM_FALSE;
            }
            break;

         default:
            return GBM_FALSE;
      }
   }

   return GBM_TRUE;
}

/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

/* Convert bitmap data from RGBA to BGR (24 bit) */
gbm_boolean gbm_map_RGBA_BGR(const gbm_u32 * data_src,       gbm_u8       * data_dst,
                             const GBM     * gbm_dst , const GBMRGB_16BPP * back_rgb,
                             const gbm_boolean unassociatedAlpha)
{
   if (gbm_dst->bpp != 24)
   {
      return GBM_FALSE;
   }
   else
   {
      int h;

      const int stride_src = gbm_dst->w * 4;
      const int stride_dst = ((gbm_dst->w * 24 + 31) / 32) * 4;

      const gbm_u8 * data8_src = (const gbm_u8 *) data_src;

      GBM gbm_src = *gbm_dst;
      gbm_src.bpp = 32;

      for (h = 0; h < gbm_dst->h; h++)
      {
         if (! gbm_map_row_RGBx_BGRx(data8_src, &gbm_src,
                                     data_dst ,  gbm_dst,
                                     back_rgb ,  unassociatedAlpha))
         {
            return GBM_FALSE;
         }

         data8_src += stride_src;
         data_dst  += stride_dst;
      }
   }

   return GBM_TRUE;
}

/* ----------------------------------------------------------- */

/* Convert bitmap data from RGBA to RGB (24 bit) */
gbm_boolean gbm_map_RGBA_RGB(const gbm_u32 * data_src,       gbm_u8       * data_dst,
                             const GBM     * gbm_dst , const GBMRGB_16BPP * back_rgb,
                             const gbm_boolean unassociatedAlpha)
{
   if (gbm_dst->bpp != 24)
   {
      return GBM_FALSE;
   }
   else
   {
      int h;
      gbm_u32 data32;

      const int stride_src = gbm_dst->w * 4;
      const int stride_dst = ((gbm_dst->w * 24 + 31) / 32) * 4;

      const gbm_u8 * data8_src = (const gbm_u8 *) data_src;

      for (h = 0; h < gbm_dst->h; h++)
      {
         const gbm_u32 * data32_src = (const gbm_u32 *) data8_src;
         gbm_u8 * data8_dst = data_dst;

         int block_count = gbm_dst->w;

         if (unassociatedAlpha)
         {
            gbm_u8 alpha;
            gbm_u16 alpha_mult;
            gbm_u8 alpha_diff;
            GBMRGB back_rgb8;
            back_rgb8.r = CVT(back_rgb->r);
            back_rgb8.g = CVT(back_rgb->g);
            back_rgb8.b = CVT(back_rgb->b);

            while (block_count > 0)
            {
               --block_count;

               data32 = *data32_src++;

               /* calc alpha channel against background color */
               alpha        = GetA(data32);
               alpha_mult   = (gbm_u16) alpha + 1;
               alpha_diff   = 255 - alpha;

               *data8_dst++ = ((gbm_u16) GetR(data32) * alpha_mult + back_rgb8.r * alpha_diff) >> 8;
               *data8_dst++ = ((gbm_u16) GetG(data32) * alpha_mult + back_rgb8.g * alpha_diff) >> 8;
               *data8_dst++ = ((gbm_u16) GetB(data32) * alpha_mult + back_rgb8.b * alpha_diff) >> 8;
            }
         }
         else
         {
            /* strip alpha channel */
            while (block_count > 0)
            {
               --block_count;

               data32 = *data32_src++;

               *data8_dst++ = GetR(data32);
               *data8_dst++ = GetG(data32);
               *data8_dst++ = GetB(data32);
            }
         }

         data8_src += stride_src;
         data_dst  += stride_dst;
      }
   }

   return GBM_TRUE;
}


/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */
/* ----------------------------------------------------------- */

static gbm_u8 nearest_color(const gbm_u8 r, const gbm_u8 g, const gbm_u8 b,
                            const GBMRGB *gbmrgb, const int n_gbmrgb)
{
    int i_min = 0, dist_min = 0x30000;
    int dr, dg, db, dist;

    const GBMRGB * pGbmRgb = gbmrgb;

    int i = n_gbmrgb;
    while (i > 0)
    {
        --i;

        dr = (int) ( (unsigned)r - (unsigned)pGbmRgb->r );
        dg = (int) ( (unsigned)g - (unsigned)pGbmRgb->g );
        db = (int) ( (unsigned)b - (unsigned)pGbmRgb->b );

        dist = dr*dr + dg*dg + db*db;

        if ( dist < dist_min )
        { dist_min = dist; i_min = (n_gbmrgb - i - 1); }

        pGbmRgb++;
    }
    return (gbm_u8) i_min;
}

/* ----------------------------------------------------------- */

static gbm_u8 find_color_index(const gbm_u32 rgb, const GBMRGB *gbmrgb, const int n_gbmrgb)
{
    const GBMRGB * pGbmRgb = gbmrgb;

    gbm_u32 cRGB;

    int i = n_gbmrgb;
    while (i > 0)
    {
       --i;

       cRGB = MakeRGB(pGbmRgb->r, pGbmRgb->g, pGbmRgb->b);
       if (cRGB == rgb)
       {
          return (gbm_u8) (n_gbmrgb - i - 1);
       }

       pGbmRgb++;
    }

    /* calculate closest color if no exact match was found */
    return nearest_color(GetR(rgb), GetG(rgb), GetB(rgb), gbmrgb, n_gbmrgb);
}

/* ----------------------------------------------------------- */

static void trunc_1bpp(const gbm_u32 *data32_src,       gbm_u8 *data1_dst,
                       const GBM     *gbm_dst   , const GBMRGB *gbmrgb_dst)
{
    const int stride_dest = ((gbm_dst->w + 31) / 32) * 4;

    int    x, y;
    gbm_u32  d32;
    gbm_u8 i;

    memset(data1_dst, 0, gbm_dst->h * stride_dest);

    for ( y = 0; y < gbm_dst->h; y++ )
    {
        for ( x = 0; x < gbm_dst->w; x++ )
        {
            d32 = *data32_src++;
            i   = find_color_index(GetRGB(d32), gbmrgb_dst, 2);
            if ( i )
            {
                data1_dst[x>>3] |= ( 0x80 >> (x & 7) );
            }
        }
        data1_dst += stride_dest;
    }
}

/* ----------------------------------------------------------- */

static void trunc_4bpp(const gbm_u32 *data32_src,       gbm_u8 *data4_dst,
                       const GBM     *gbm_dst   , const GBMRGB *gbmrgb_dst)
{
    const int stride_dest = ((gbm_dst->w * 4 + 31) / 32) * 4;
    const int step_dest   = stride_dest - ((gbm_dst->w * 4)+7)/8;

    int    x, y;
    gbm_u32  d32;
    gbm_u8 i0, i1;

    for ( y = 0; y < gbm_dst->h; y++ )
    {
        for ( x = 0; x+1 < gbm_dst->w; x += 2 )
        {
            d32 = *data32_src++;
            i0  = find_color_index(GetRGB(d32), gbmrgb_dst, 16);

            d32 = *data32_src++;
            i1  = find_color_index(GetRGB(d32), gbmrgb_dst, 16);

            *data4_dst++ = ( ( i0 << 4 ) | i1 );
        }
        if ( gbm_dst->w & 1 )
        {
            d32 = *data32_src++;
            i0  = find_color_index(GetRGB(d32), gbmrgb_dst, 16);

            *data4_dst++ = ( i0 << 4 );
        }
        data4_dst += step_dest;
    }
}

/* ----------------------------------------------------------- */

static void trunc_8bpp(const gbm_u32 *data32_src,       gbm_u8 *data8_dst,
                       const GBM     *gbm_dst   , const GBMRGB *gbmrgb_dst)
{
    const int stride_dest = ((gbm_dst->w * 8 + 31) / 32) * 4;
    const int step_dest   = stride_dest - gbm_dst->w;

    int    x, y;
    gbm_u32  d32;

    for ( y = 0; y < gbm_dst->h; y++ )
    {
        for ( x = 0; x < gbm_dst->w; x++ )
        {
            d32          = *data32_src++;
            *data8_dst++ = find_color_index(GetRGB(d32), gbmrgb_dst, 256);
        }
        data8_dst += step_dest;
    }
}

/* ----------------------------------------------------------- */

/* Convert bitmap data from RGBA to palette */
gbm_boolean gbm_map_RGBA_PAL(const gbm_u32 * data_src,       gbm_u8 * data_dst,
                             const GBM     * gbm_dst , const GBMRGB * gbmrgb_dst)
{
   /* Map to palette format */
   switch(gbm_dst->bpp)
   {
      case 1:
         trunc_1bpp(data_src, data_dst, gbm_dst, gbmrgb_dst);
         break;

      case 4:
         trunc_4bpp(data_src, data_dst, gbm_dst, gbmrgb_dst);
         break;

      case 8:
         trunc_8bpp(data_src, data_dst, gbm_dst, gbmrgb_dst);
         break;

      default:
         return GBM_FALSE;
   }

   return GBM_TRUE;
}



