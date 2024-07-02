#include <stdint.h>
#include <stddef.h>
#include <assert.h>
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


#define H264_CHROMA_MC(OPNAME, OP)\
void (OPNAME ## h264_chroma_mc1)(uint8_t *__restrict__ _dst /*align 8*/, const uint8_t *__restrict__ _src /*align 1*/, ptrdiff_t stride, int h, int x, int y){\
    pixel *dst = (pixel*)_dst;\
    const pixel *src = (const pixel*)_src;\
    const int A=(8-x)*(8-y);\
    const int B=(  x)*(8-y);\
    const int C=(8-x)*(  y);\
    const int D=(  x)*(  y);\
    int i;\
    stride >>= sizeof(pixel)-1;\
    \
    assert(x<8 && y<8 && x>=0 && y>=0);\
\
    if(D){\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + B*src[1] + C*src[stride+0] + D*src[stride+1]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else if (B + C) {\
        const int E= B+C;\
        const int step= C ? stride : 1;\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + E*src[step+0]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else {\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0]));\
            dst+= stride;\
            src+= stride;\
        }\
    }\
}\
void (OPNAME ## h264_chroma_mc2)(uint8_t *__restrict__ _dst /*align 8*/, const uint8_t *__restrict__ _src /*align 1*/, ptrdiff_t stride, int h, int x, int y)\
{\
    pixel *dst = (pixel*)_dst;\
    const pixel *src = (const pixel*)_src;\
    const int A=(8-x)*(8-y);\
    const int B=(  x)*(8-y);\
    const int C=(8-x)*(  y);\
    const int D=(  x)*(  y);\
    int i;\
    stride >>= sizeof(pixel)-1;\
    \
    assert(x<8 && y<8 && x>=0 && y>=0);\
\
    if(D){\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + B*src[1] + C*src[stride+0] + D*src[stride+1]));\
            OP(dst[1], (A*src[1] + B*src[2] + C*src[stride+1] + D*src[stride+2]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else if (B + C) {\
        const int E= B+C;\
        const ptrdiff_t step = C ? stride : 1;\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + E*src[step+0]));\
            OP(dst[1], (A*src[1] + E*src[step+1]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else {\
        for ( i = 0; i < h; i++){\
            OP(dst[0], A * src[0]);\
            OP(dst[1], A * src[1]);\
            dst += stride;\
            src += stride;\
        }\
    }\
}\
\
void (OPNAME ## h264_chroma_mc4)(uint8_t *__restrict__ _dst /*align 8*/, const uint8_t *__restrict__ _src /*align 1*/, ptrdiff_t stride, int h, int x, int y)\
{\
    pixel *dst = (pixel*)_dst;\
    const pixel *src = (const pixel*)_src;\
    const int A=(8-x)*(8-y);\
    const int B=(  x)*(8-y);\
    const int C=(8-x)*(  y);\
    const int D=(  x)*(  y);\
    int i;\
    stride >>= sizeof(pixel)-1;\
    \
    assert(x<8 && y<8 && x>=0 && y>=0);\
\
    if(D){\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + B*src[1] + C*src[stride+0] + D*src[stride+1]));\
            OP(dst[1], (A*src[1] + B*src[2] + C*src[stride+1] + D*src[stride+2]));\
            OP(dst[2], (A*src[2] + B*src[3] + C*src[stride+2] + D*src[stride+3]));\
            OP(dst[3], (A*src[3] + B*src[4] + C*src[stride+3] + D*src[stride+4]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else if (B + C) {\
        const int E= B+C;\
        const ptrdiff_t step = C ? stride : 1;\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + E*src[step+0]));\
            OP(dst[1], (A*src[1] + E*src[step+1]));\
            OP(dst[2], (A*src[2] + E*src[step+2]));\
            OP(dst[3], (A*src[3] + E*src[step+3]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else {\
        for ( i = 0; i < h; i++){\
            OP(dst[0], A * src[0]);\
            OP(dst[1], A * src[1]);\
            OP(dst[2], A * src[2]);\
            OP(dst[3], A * src[3]);\
            dst += stride;\
            src += stride;\
        }\
    }\
}\
\
void (OPNAME ## h264_chroma_mc8)(uint8_t *__restrict__ _dst /*align 8*/, const uint8_t *__restrict__ _src /*align 1*/, ptrdiff_t stride, int h, int x, int y)\
{\
    pixel *dst = (pixel*)_dst;\
    const pixel *src = (const pixel*)_src;\
    const int A=(8-x)*(8-y);\
    const int B=(  x)*(8-y);\
    const int C=(8-x)*(  y);\
    const int D=(  x)*(  y);\
    int i;\
    stride >>= sizeof(pixel)-1;\
    \
    assert(x<8 && y<8 && x>=0 && y>=0);\
\
    if(D){\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + B*src[1] + C*src[stride+0] + D*src[stride+1]));\
            OP(dst[1], (A*src[1] + B*src[2] + C*src[stride+1] + D*src[stride+2]));\
            OP(dst[2], (A*src[2] + B*src[3] + C*src[stride+2] + D*src[stride+3]));\
            OP(dst[3], (A*src[3] + B*src[4] + C*src[stride+3] + D*src[stride+4]));\
            OP(dst[4], (A*src[4] + B*src[5] + C*src[stride+4] + D*src[stride+5]));\
            OP(dst[5], (A*src[5] + B*src[6] + C*src[stride+5] + D*src[stride+6]));\
            OP(dst[6], (A*src[6] + B*src[7] + C*src[stride+6] + D*src[stride+7]));\
            OP(dst[7], (A*src[7] + B*src[8] + C*src[stride+7] + D*src[stride+8]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else if (B + C) {\
        const int E= B+C;\
        const ptrdiff_t step = C ? stride : 1;\
        for(i=0; i<h; i++){\
            OP(dst[0], (A*src[0] + E*src[step+0]));\
            OP(dst[1], (A*src[1] + E*src[step+1]));\
            OP(dst[2], (A*src[2] + E*src[step+2]));\
            OP(dst[3], (A*src[3] + E*src[step+3]));\
            OP(dst[4], (A*src[4] + E*src[step+4]));\
            OP(dst[5], (A*src[5] + E*src[step+5]));\
            OP(dst[6], (A*src[6] + E*src[step+6]));\
            OP(dst[7], (A*src[7] + E*src[step+7]));\
            dst+= stride;\
            src+= stride;\
        }\
    } else {\
        for ( i = 0; i < h; i++){\
            OP(dst[0], A * src[0]);\
            OP(dst[1], A * src[1]);\
            OP(dst[2], A * src[2]);\
            OP(dst[3], A * src[3]);\
            OP(dst[4], A * src[4]);\
            OP(dst[5], A * src[5]);\
            OP(dst[6], A * src[6]);\
            OP(dst[7], A * src[7]);\
            dst += stride;\
            src += stride;\
        }\
    }\
}

#define op_avg(a, b) a = (((a)+(((b) + 32)>>6)+1)>>1)
#define op_put(a, b) a = (((b) + 32)>>6)

H264_CHROMA_MC(put_       , op_put)
H264_CHROMA_MC(avg_       , op_avg)
// put_h264_chroma_mc8
// avg_h264_chroma_mc8

H264_WEIGHT(16)



