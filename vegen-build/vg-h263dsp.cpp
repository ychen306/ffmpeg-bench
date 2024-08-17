#include <stdint.h>

#define FFABS(a) ((a) >= 0 ? (a) : (-(a)))

static inline int16_t av_clip(int16_t a, int16_t amin, int16_t amax){
    if (a < amin){
        return amin;
    } else if (a > amax) {
        return amax;
    } else {
        return a;
    }
}

const uint8_t ff_h263_loop_filter_strength[32] = {
    0, 1, 1, 2, 2, 3, 3,  4,  4,  4,  5,  5,  6,  6,  7, 7,
    7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12
};

void h263_h_loop_filter(uint8_t *src, int stride, int qscale){
    int y;
    const uint8_t strength = ff_h263_loop_filter_strength[qscale];
    with_threads(y, 8, 8){
        int16_t d1, d2, ad1;
        int16_t p0 = src[y * stride - 2];
        int16_t p1 = src[y * stride - 1];
        int16_t p2 = src[y * stride + 0];
        int16_t p3 = src[y * stride + 1];
        int16_t d  = (p0 - p3 + 4 * (p2 - p1)) / 8;

        if (d < -2 * strength)
            d1 = 0;
        else if (d < -strength)
            d1 = -2 * strength - d;
        else if (d < strength)
            d1 = d;
        else if (d < 2 * strength)
            d1 = 2 * strength - d;
        else
            d1 = 0;

        p1 += d1;
        p2 -= d1;
        if (p1 & 256)
            p1 = ~(p1 >> 31);
        if (p2 & 256)
            p2 = ~(p2 >> 31);

        src[y * stride - 1] = p1;
        src[y * stride + 0] = p2;

        ad1 = FFABS(d1) >> 1;

        d2 = av_clip((p0 - p3) / 4, -ad1, ad1);

        src[y * stride - 2] = p0 - d2;
        src[y * stride + 1] = p3 + d2;
    }
}

void h263_v_loop_filter(uint8_t *__restrict__ src, int stride, int qscale){
    int x;
    const uint8_t strength = ff_h263_loop_filter_strength[qscale];
    with_threads(x, 8, 8){
        int16_t d1, d2, ad1;
        int16_t p0 = src[x - 2 * stride];
        int16_t p1 = src[x - 1 * stride];
        int16_t p2 = src[x + 0 * stride];
        int16_t p3 = src[x + 1 * stride];
        int16_t d  = (p0 - p3 + 4 * (p2 - p1)) / 8;

        if (d < -2 * strength)
            d1 = 0;
        else if (d < -strength)
            d1 = -2 * strength - d;
        else if (d < strength)
            d1 = d;
        else if (d < 2 * strength)
            d1 = 2 * strength - d;
        else
            d1 = 0;

        p1 += d1;
        p2 -= d1;
        if (p1 & 256)
            p1 = ~(p1 >> 31);
        if (p2 & 256)
            p2 = ~(p2 >> 31);

        src[x - 1 * stride] = p1;
        src[x + 0 * stride] = p2;

        ad1 = FFABS(d1) >> 1;

        d2 = av_clip((p0 - p3) / 4, -ad1, ad1);

        src[x - 2 * stride] = p0 - d2;
        src[x + stride]     = p3 + d2;
    }

}
