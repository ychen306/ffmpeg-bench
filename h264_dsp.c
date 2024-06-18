#include <stdint.h>
#include <stddef.h>
// making assumption that BIT_DEPTH = 8

// how to use this function definition in main bench file?



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

#define BIT_DEPTH 8
#define FUNC3(a, b, c)  a ## _ ## b ##  c
#define FUNC2(a, b, c)  FUNC3(a, b, c)
#define FUNCC(a) FUNC2(a, BIT_DEPTH, _c)
#define op_scale1(x)  block[x] = av_clip_pixel( (block[x]*weight + offset) >> log2_denom )
#define op_scale2(x)  dst[x] = av_clip_pixel( (src[x]*weights + dst[x]*weightd + offset) >> (log2_denom+1))

// https://github.com/FFmpeg/FFmpeg/blob/94f2274a8b61438572f0873ccf430e55ce0e0e2b/libavcodec/h264dsp_template.c#L32

#define H264_WEIGHT(W) \
void FUNCC(weight_h264_pixels ## W)(uint8_t *__restrict__ _block, ptrdiff_t stride, int height, \
                                           int log2_denom, int weight, int offset) \
{ \
    int y; \
    pixel *block = (pixel*)_block; \
    stride >>= sizeof(pixel)-1; \
    offset = (unsigned)offset << (log2_denom + (BIT_DEPTH-8)); \
    if(log2_denom) offset += 1<<(log2_denom-1); \
    for (y = 0; y < height; y++, block += stride) { \
        op_scale1(0); \
        op_scale1(1); \
        if(W==2) continue; \
        op_scale1(2); \
        op_scale1(3); \
        if(W==4) continue; \
        op_scale1(4); \
        op_scale1(5); \
        op_scale1(6); \
        op_scale1(7); \
        if(W==8) continue; \
        op_scale1(8); \
        op_scale1(9); \
        op_scale1(10); \
        op_scale1(11); \
        op_scale1(12); \
        op_scale1(13); \
        op_scale1(14); \
        op_scale1(15); \
    } \
} \
void FUNCC(biweight_h264_pixels ## W)(uint8_t *__restrict__ _dst, uint8_t *__restrict__ _src, ptrdiff_t stride, int height, \
                                             int log2_denom, int weightd, int weights, int offset) \
{ \
    int y; \
    pixel *dst = (pixel*)_dst; \
    pixel *src = (pixel*)_src; \
    stride >>= sizeof(pixel)-1; \
    offset = (unsigned)offset << (BIT_DEPTH-8); \
    offset = (unsigned)((offset + 1) | 1) << log2_denom; \
    for (y = 0; y < height; y++, dst += stride, src += stride) { \
        op_scale2(0); \
        op_scale2(1); \
        if(W==2) continue; \
        op_scale2(2); \
        op_scale2(3); \
        if(W==4) continue; \
        op_scale2(4); \
        op_scale2(5); \
        op_scale2(6); \
        op_scale2(7); \
        if(W==8) continue; \
        op_scale2(8); \
        op_scale2(9); \
        op_scale2(10); \
        op_scale2(11); \
        op_scale2(12); \
        op_scale2(13); \
        op_scale2(14); \
        op_scale2(15); \
    } \
}

H264_WEIGHT(16)
H264_WEIGHT(8)
H264_WEIGHT(4)
H264_WEIGHT(2)
