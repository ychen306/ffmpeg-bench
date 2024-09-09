#include <stdint.h>

static inline uint8_t av_clip_uint8(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

#define FILTER(src, stride)                                     \
    ((21*((src)[ 0*stride] + (src)[1*stride])                   \
      -7*((src)[-1*stride] + (src)[2*stride])                   \
      +3*((src)[-2*stride] + (src)[3*stride])                   \
      -1*((src)[-3*stride] + (src)[4*stride]) + 16) >> 5)

void put_signed_rect_clamped_8bit_c(uint8_t *dst, int dst_stride, const uint8_t *_src, int src_stride, int width, int height)
{
    int x, y;
    const int16_t *src = (const int16_t *)_src;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x+=4) {
            dst[x  ] = av_clip_uint8(src[x  ] + 128);
            dst[x+1] = av_clip_uint8(src[x+1] + 128);
            dst[x+2] = av_clip_uint8(src[x+2] + 128);
            dst[x+3] = av_clip_uint8(src[x+3] + 128);
        }
        dst += dst_stride;
        src += src_stride >> 1;
    }
}

void dirac_hpel_filter(uint8_t *dsth, uint8_t *dstv, uint8_t *dstc, const uint8_t *src,
                              int stride, int width, int height)
{
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = -3; x < width+5; x++)
            dstv[x] = av_clip_uint8(FILTER(src+x, stride));

        for (x = 0; x < width; x++)
            dstc[x] = av_clip_uint8(FILTER(dstv+x, 1));

        for (x = 0; x < width; x++)
            dsth[x] = av_clip_uint8(FILTER(src+x, 1));

        src  += stride;
        dsth += stride;
        dstv += stride;
        dstc += stride;
    }
}

