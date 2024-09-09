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
    with_threads(y, height, 1){
        int i = y*dst_stride;
        int j = y*src_stride >> 1;
        with_threads(x, width, 1){
            dst[x + i] = av_clip_uint8(src[x + j] + 128);
        }
    }

}

void dirac_hpel_filter(uint8_t *dsth, uint8_t *dstv, uint8_t *dstc, uint8_t *src,
                              int stride, int width, int height)
{
    // int x, y;
    with_threads(y, height, 1){
        int i = y*stride;
        with_threads(x, height+5, 1){
            dstv[x+i-3] = av_clip_uint8(FILTER(src+x+i-3, stride));
        }
        with_threads(x, width, 1){
            dstc[x+i] = av_clip_uint8(FILTER(dstv+x+i, 1));
        }
        with_threads(x, width, 1){
            dsth[x+i] = av_clip_uint8(FILTER(src+x+i, 1));
        }
    }
}