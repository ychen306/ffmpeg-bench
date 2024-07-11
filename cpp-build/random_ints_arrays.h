#ifndef RANDOM_INTS_ARRAYS_H
#define RANDOM_INTS_ARRAYS_H

#include <stdlib.h>
#include <stdint.h>

#define RAND_INT(B) static int ## B ## _t rand ## B() { return rand() % ((B == 32) ? INT32_MAX : ((B == 64) ? INT64_MAX : (1 << B))); }
#define RAND_UINT(B) static uint ## B ## _t randu ## B() { return rand() % ((B == 32) ? UINT32_MAX : ((B == 64) ? UINT64_MAX : (1 << B))); }

#define RAND_ARRAY(B) static int ## B ## _t *rand_array_ ## B(int size) { \
        int ## B ## _t *arr = (int ## B ## _t*) malloc(sizeof(int ## B ## _t) * size); \
        if (arr == NULL) { \
            perror("Unable to allocate memory"); \
            exit(EXIT_FAILURE); \
        } \
        for (int i = 0; i < size; i++) \
            arr[i] = rand ## B(); \
        return arr; \
}

#define RAND_ARRAY_U(B) static uint ## B ## _t *rand_array_u ## B(int size) { \
        uint ## B ## _t *arr = (uint ## B ## _t*) malloc(sizeof(uint ## B ## _t) * size); \
        if (arr == NULL) { \
            perror("Unable to allocate memory"); \
            exit(EXIT_FAILURE); \
        } \
        for (int i = 0; i < size; i++) \
            arr[i] = randu ## B(); \
        return arr; \
}

static inline int *rand_array(int size) {
    int* arr = (int*) malloc(size * sizeof(int));
    if (arr == NULL) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
        arr[i] = ((unsigned)rand()) >> 24;
    return arr;
}

#endif // RANDOM_INTS_ARRAYS_H

