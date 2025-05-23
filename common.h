#include <stdint.h>

#define DEBUG 1

typedef uint32_t sum2_t;
typedef uint16_t sum_t;
typedef uint8_t pixel;
typedef long int intptr_t;

#define BITS_PER_SUM (8 * sizeof(sum_t))

#define HADAMARD4(d0, d1, d2, d3, s0, s1, s2, s3) { \
        sum2_t t0 = s0 + s1; \
        sum2_t t1 = s0 - s1; \
        sum2_t t2 = s2 + s3; \
        sum2_t t3 = s2 - s3; \
        d0 = t0 + t2; \
        d2 = t0 - t2; \
        d1 = t1 + t3; \
        d3 = t1 - t3; \
}

inline sum2_t abs2(sum2_t a)
{
    sum2_t s = ((a >> (BITS_PER_SUM - 1)) & (((sum2_t)1 << BITS_PER_SUM) + 1)) * ((sum_t)-1);

    return (a + s) ^ s;
}

int pixel_satd_4x4_neon(const uint8_t *pix1, intptr_t stride_pix1, const uint8_t *pix2, intptr_t stride_pix2);
int satd_4x4(const pixel* pix1, intptr_t stride_pix1, const pixel* pix2, intptr_t stride_pix2);
int satd_4x4_hvx(const pixel* pix1, intptr_t stride_pix1, const pixel* pix2, intptr_t stride_pix2);
