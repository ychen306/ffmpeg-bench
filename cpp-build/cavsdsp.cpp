#include <stddef.h>
#include <stdint.h>

static inline uint8_t av_clip_uint8(int x) {
  if (x < 0)
    return 0;
  if (x > 255)
    return 255;
  return x;
}

void cavs_idct8_add_c(uint8_t *__restrict__ dst, int16_t *__restrict__ block, ptrdiff_t stride)
{
    int i;
    int16_t (*src)[8] = (int16_t(*)[8])block;

    src[0][0] += 8;

    for( i = 0; i < 8; i++ ) {
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

        src[i][0] = (b0 + b4) >> 3;
        src[i][1] = (b1 + b5) >> 3;
        src[i][2] = (b2 + b6) >> 3;
        src[i][3] = (b3 + b7) >> 3;
        src[i][4] = (b3 - b7) >> 3;
        src[i][5] = (b2 - b6) >> 3;
        src[i][6] = (b1 - b5) >> 3;
        src[i][7] = (b0 - b4) >> 3;
    }
    for( i = 0; i < 8; i++ ) {
        const int a0 = 3 * src[1][i] - 2 * src[7][i];
        const int a1 = 3 * src[3][i] + 2 * src[5][i];
        const int a2 = 2 * src[3][i] - 3 * src[5][i];
        const int a3 = 2 * src[1][i] + 3 * src[7][i];

        const int b4 = 2 * (a0 + a1 + a3) + a1;
        const int b5 = 2 * (a0 - a1 + a2) + a0;
        const int b6 = 2 * (a3 - a2 - a1) + a3;
        const int b7 = 2 * (a0 - a2 - a3) - a2;

        const int a7 = 4 * src[2][i] - 10 * src[6][i];
        const int a6 = 4 * src[6][i] + 10 * src[2][i];
        const int a5 = 8 * (src[0][i] - src[4][i]);
        const int a4 = 8 * (src[0][i] + src[4][i]);

        const int b0 = a4 + a6;
        const int b1 = a5 + a7;
        const int b2 = a5 - a7;
        const int b3 = a4 - a6;

        dst[i + 0*stride] = av_clip_uint8( dst[i + 0*stride] + ((b0 + b4) >> 7));
        dst[i + 1*stride] = av_clip_uint8( dst[i + 1*stride] + ((b1 + b5) >> 7));
        dst[i + 2*stride] = av_clip_uint8( dst[i + 2*stride] + ((b2 + b6) >> 7));
        dst[i + 3*stride] = av_clip_uint8( dst[i + 3*stride] + ((b3 + b7) >> 7));
        dst[i + 4*stride] = av_clip_uint8( dst[i + 4*stride] + ((b3 - b7) >> 7));
        dst[i + 5*stride] = av_clip_uint8( dst[i + 5*stride] + ((b2 - b6) >> 7));
        dst[i + 6*stride] = av_clip_uint8( dst[i + 6*stride] + ((b1 - b5) >> 7));
        dst[i + 7*stride] = av_clip_uint8( dst[i + 7*stride] + ((b0 - b4) >> 7));
    }
}
