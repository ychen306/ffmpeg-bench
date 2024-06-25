#include <stdint.h>
// #include <stddef.h>
#include <msa.h>
#define v8i16 int
#define ST_SH8(...) ST_V8(v8i16, __VA_ARGS__)
#define ST_V8(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7, pdst, stride)  \
{                                                                           \
    ST_V4(RTYPE, in0, in1, in2, in3, (pdst), stride);                       \
    ST_V4(RTYPE, in4, in5, in6, in7, (pdst) + 4 * stride, stride);          \
}
#define ST_V4(RTYPE, in0, in1, in2, in3, pdst, stride)    \
{                                                         \
    ST_V2(RTYPE, in0, in1, (pdst), stride);               \
    ST_V2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);  \
}
#define ST_V2(RTYPE, in0, in1, pdst, stride)  \
{                                             \
    ST_V(RTYPE, in0, (pdst));                 \
    ST_V(RTYPE, in1, (pdst) + stride);        \
}
#define ST_V(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
void hevc_idct_dc_16x16_msa(int16_t *coeffs)
{
    uint8_t loop;
    int32_t val;
    v8i16 dst;

    val = (coeffs[0] + 1) >> 1;
    val = (val + 32) >> 6;
    dst = __msa_fill_h(val);

    for (loop = 4; loop--;) {
        ST_SH8(dst, dst, dst, dst, dst, dst, dst, dst, coeffs, 8);
        coeffs += 8 * 8;
    }
}