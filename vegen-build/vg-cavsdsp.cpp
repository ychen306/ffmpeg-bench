#include <stddef.h>
#include <stdint.h>

static inline uint8_t av_clip_uint8(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

void cavs_idct8_add_c(uint8_t * dst, int16_t * block, ptrdiff_t stride)
{
    int i;
    int16_t (*src)[8] = (int16_t(*)[8])block;

    src[0][0] += 8;

    with_threads(i, 8, 8){
        vegen::block<int, 8, 8> temp;
        const int a0 = 3 * src[i][1] - 2 * src[i][7];
        const int a1 = 3 * src[i][3] + 2 * src[i][5];
        const int a2 = 2 * src[i][3] - 3 * src[i][5];
        const int a3 = 2 * src[i][1] + 3 * src[i][7];

        const int b4 = 2 * (a0 + a1 + a3) + a1;
        const int b5 = 2 * (a0 - a1 + a2) + a0;
        const int b6 = 2 * (a3 - a2 - a1) + a3;
        const int b7 = 2 * (a0 - a2 - a3) - a2;

        const int a7 = 4 * src[i][2] - 10 * src[i][6];
        const int a6 = 4 * src[i][6] + 10 * src[i][2];
        const int a5 = 8 * (src[i][0] - src[i][4]) + 4;
        const int a4 = 8 * (src[i][0] + src[i][4]) + 4;

        const int b0 = a4 + a6;
        const int b1 = a5 + a7;
        const int b2 = a5 - a7;
        const int b3 = a4 - a6;

        temp(i, 0) = (b0 + b4) >> 3;
        temp(i, 1) = (b1 + b5) >> 3;
        temp(i, 2) = (b2 + b6) >> 3;
        temp(i, 3) = (b3 + b7) >> 3;
        temp(i, 4) = (b3 - b7) >> 3;
        temp(i, 5) = (b2 - b6) >> 3;
        temp(i, 6) = (b1 - b5) >> 3;
        temp(i, 7) = (b0 - b4) >> 3;

        vegen::sync();

        const int _a0 = 3 * temp(1, i) - 2 * temp(7, i);
        const int _a1 = 3 * temp(3, i) + 2 * temp(5, i);
        const int _a2 = 2 * temp(3, i) - 3 * temp(5, i);
        const int _a3 = 2 * temp(1, i) + 3 * temp(7, i);

        const int _b4 = 2 * (_a0 + _a1 + _a3) + _a1;
        const int _b5 = 2 * (_a0 - _a1 + _a2) + _a0;
        const int _b6 = 2 * (_a3 - _a2 - _a1) + _a3;
        const int _b7 = 2 * (_a0 - _a2 - _a3) - _a2;

        const int _a7 = 4 * temp(2, i) - 10 * temp(6, i);
        const int _a6 = 4 * temp(6, i) + 10 * temp(2, i);
        const int _a5 = 8 * (temp(0, i) - temp(4, i));
        const int _a4 = 8 * (temp(0, i) + temp(4, i));

        const int _b0 = _a4 + _a6;
        const int _b1 = _a5 + _a7;
        const int _b2 = _a5 - _a7;
        const int _b3 = _a4 - _a6;

        dst[i + 0*stride] = av_clip_uint8( dst[i + 0*stride] + ((_b0 + _b4) >> 7));
        dst[i + 1*stride] = av_clip_uint8( dst[i + 1*stride] + ((_b1 + _b5) >> 7));
        dst[i + 2*stride] = av_clip_uint8( dst[i + 2*stride] + ((_b2 + _b6) >> 7));
        dst[i + 3*stride] = av_clip_uint8( dst[i + 3*stride] + ((_b3 + _b7) >> 7));
        dst[i + 4*stride] = av_clip_uint8( dst[i + 4*stride] + ((_b3 - _b7) >> 7));
        dst[i + 5*stride] = av_clip_uint8( dst[i + 5*stride] + ((_b2 - _b6) >> 7));
        dst[i + 6*stride] = av_clip_uint8( dst[i + 6*stride] + ((_b1 - _b5) >> 7));
        dst[i + 7*stride] = av_clip_uint8( dst[i + 7*stride] + ((_b0 - _b4) >> 7));

    }

}

