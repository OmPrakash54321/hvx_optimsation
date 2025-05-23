#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define STRIDE 64
#define NUM 256
#define MAX 255
#define MIN 0

int main(void) {
    time_t t;
    int seed = (int)time(&t); // for having random values in the input pixel
    srand(seed);
    #if (DEBUG == 1)
        printf("seed - %d, time - %ld \n", seed, t);
    #endif

    // Declare and initialise input pixels
    pixel pix1[NUM];
    pixel pix2[NUM];
    for (int i = 0; i < NUM; i++) {
        pix1[i] = rand() % MAX; 
        pix2[i] = rand() % MAX; 
    }

    // Run scalar and vector functions
    int satd_c = satd_4x4(pix1, STRIDE, pix2, STRIDE);
    #if defined(__arm__) || defined(__aarch64__)
    int satd_neon = pixel_satd_4x4_neon(pix1, STRIDE, pix2, STRIDE);
    #if (DEBUG == 1)
    printf("satd_c - %d \n", satd_c);
    printf("satd_neon - %d \n", satd_neon);
    #endif
    if (satd_c != satd_neon) printf("Mismatch c - %d, neon - %d", satd_c, satd_neon);
    #endif
    return 0;
}