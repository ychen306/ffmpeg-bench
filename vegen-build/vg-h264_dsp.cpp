#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#define SUINT int

__attribute__((always_inline))  
static inline uint8_t av_clip_pixel(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

#define pixel uint8_t
#define dctcoef int16_t

#define op_scale1(x)  block[x] = av_clip_pixel( (block[x]*weight + offset) >> log2_denom )
#define op_scale2(x)  dst[x] = av_clip_pixel( (src[x]*weights + dst[x]*weightd + offset) >> (log2_denom+1))

void weight_h264_pixels16_8_c(uint8_t * _block, ptrdiff_t stride, int height, 
                                int log2_denom, int weight, int offset){ 
    pixel *block = (pixel*)_block;
    stride >>= sizeof(pixel)-1;
    offset = (unsigned)offset << (log2_denom);
    if(log2_denom) offset += 1<<(log2_denom-1);
    with_threads(y, height, 1){
      int i = y*stride;
      with_threads(x, 16, 16){
        op_scale1(i + x);
      }
    } 
}


