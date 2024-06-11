
// https://github.com/FFmpeg/FFmpeg/blob/94f2274a8b61438572f0873ccf430e55ce0e0e2b/libavcodec/aacencdsp.h#L46

#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
void quantize_bands(int *out, const float *in, const float *scaled,
    int size, int is_signed, int maxval, const float Q34,
    const float rounding)
{
    for (int i = 0; i < size; i++) {
        float qc = scaled[i] * Q34;
        int tmp = (int)FFMIN(qc + rounding, (float)maxval);
        if (is_signed && in[i] < 0.0f) {
            tmp = -tmp;
        }
        out[i] = tmp;
    }
}