#ifndef RANDOM_INTS_ARRAYS_H
#define RANDOM_INTS_ARRAYS_H


#define RAND_INT(B) static int ## B ## _t  (rand ## B)(){                       \
                return rand() % (1<<B);}

#define RAND_ARRAY(B) static int ## B ## _t *(rand_array_ ## B)(int size){      \
        int ## B ## _t *arr = malloc(sizeof(int ## B ## _t) * size);            \
        for (int i = 0; i < size; i++)                                          \
            arr[i] = (rand ## B)();                                             \
        return arr;                                                             \
}                                                                               \


inline int *rand_array(int size) {
    int* arr;
    arr = (int*) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
        arr[i] = rand();
    return arr;
}

#endif RANDOM_INTS_ARRAYS_H
