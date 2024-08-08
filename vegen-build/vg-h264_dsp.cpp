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

void biweight_h264_pixels16_8_c(uint8_t *_dst, uint8_t *_src, ptrdiff_t stride, int height,
                                             int log2_denom, int weightd, int weights, int offset){
    int y;
    pixel *dst = (pixel*)_dst;
    pixel *src = (pixel*)_src;
    stride >>= sizeof(pixel)-1;
    offset = (unsigned)offset;
    offset = (unsigned)((offset + 1) | 1) << log2_denom;
    with_threads(y, height, 1){
      int i = y*stride;
      with_threads(x, 16, 16){
        op_scale2(i + x);
      }
    }
}

#define op_put(a, b) a = (((b) + 32)>>6)

void put_h264_chroma_mc8(uint8_t *_dst, const uint8_t *_src, ptrdiff_t stride, int h, int x, int y){

    pixel *dst = (pixel*)_dst;
    const pixel *src = (const pixel*)_src;
    const int A=(8-x)*(8-y);
    const int B=(  x)*(8-y);
    const int C=(8-x)*(  y);
    const int D=(  x)*(  y);
    int i;
    stride >>= sizeof(pixel)-1;
    
    // assert(x<8 && y<8 && x>=0 && y>=0);

    if(D){
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
              op_put(dst[x + i], (A*src[x + i + 1] + B*src[x + i + 1] + 
                      C*src[stride + x + i] + D*src[stride + x + i + 1]));
          }
        }
    }else if (B + C) {
        const int E= B+C;
        const ptrdiff_t step = C ? stride : 1;
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
            op_put(dst[x + i], (A*src[x + i] + E*src[step + x + i]));
          }
        }
    }else {
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
            op_put(dst[x + i], A * src[x + i]);
          }
        }
    }
}

#define op_avg(a, b) a = (((a)+(((b) + 32)>>6)+1)>>1)

void avg_h264_chroma_mc8(uint8_t *_dst, const uint8_t *_src, ptrdiff_t stride, int h, int x, int y){

    pixel *dst = (pixel*)_dst;
    const pixel *src = (const pixel*)_src;
    const int A=(8-x)*(8-y);
    const int B=(  x)*(8-y);
    const int C=(8-x)*(  y);
    const int D=(  x)*(  y);
    int i;
    stride >>= sizeof(pixel)-1;
    
    // assert(x<8 && y<8 && x>=0 && y>=0);

    if(D){
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
              op_avg(dst[x + i], (A*src[x + i + 1] + B*src[x + i + 1] + 
                      C*src[stride + x + i] + D*src[stride + x + i + 1]));
          }
        }
    }else if (B + C) {
        const int E= B+C;
        const ptrdiff_t step = C ? stride : 1;
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
            op_avg(dst[x + i], (A*src[x + i] + E*src[step + x + i]));
          }
        }
    }else {
        with_threads(y, h, 1){
          int i = y*stride;
          with_threads(x, 8, 8){
            op_avg(dst[x + i], A * src[x + i]);
          }
        }
    }
}


