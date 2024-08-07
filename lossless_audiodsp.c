//https://github.com/FFmpeg/FFmpeg/blob/9dac8495b0020f7326bbe79efa070a0150109afc/libavcodec/lossless_audiodsp.c#L27

#include <stdint.h>

int32_t scalarproduct_and_madd_int16(int16_t *__restrict__ v1, const int16_t *__restrict__ v2,
                                              const int16_t *__restrict__ v3,
                                              int order, int mul)
{
    unsigned res = 0;

    do {
        res   += *v1 * *v2++;
        *v1++ += mul * *v3++;
        res   += *v1 * *v2++;
        *v1++ += mul * *v3++;
    } while (order-=2);
    /*
    for (int i = 0; i < order; i++) {
      res += v1[i] * v2[i];
      v1[i] += mul * v3[i];
    }
    */
    return res;
}

int32_t scalarproduct_and_madd_int32(int16_t *__restrict__ v1, const int32_t *__restrict__ v2,
                                              const int16_t *__restrict__ v3,
                                              int order, int mul)
{
    int res = 0;

    do {
        res   += *v1 * (uint32_t)*v2++;
        *v1++ += mul * *v3++;
        res   += *v1 * (uint32_t)*v2++;
        *v1++ += mul * *v3++;
    } while (order-=2);
    return res;
}
