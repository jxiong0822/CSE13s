#include "insert.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void insertion_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    for (uint32_t k = 1; k < length; k++) {
        u_int32_t j = k;
        uint32_t temp = move(stats, arr[k]);
        while ((j > 0) && (cmp(stats, temp, arr[j - 1]) < 0)) {
            arr[j] = move(stats, arr[j - 1]);
            j -= 1;
        }
        arr[j] = move(stats, temp);
    }
}

/*
int insertion_sort(u_int32_t *A, u_int32_t len) {
    for (u_int32_t k = 1; k < length; k++) {
        u_int32_t j = k;
        u_int32_t temp = arr[k];
        while ((j > 0) && (temp < A[j - 1])) {
            arr[j] = arr[j - 1];
            j -= 1;
        }
        arr[j] = temp;
    }
}

*   2. Within your sorting function, use these functions for the indicated
*      purposes.
*
*       Purpose   Instead of this C code...   Use this C code...
*       -------   -------------------------   --------------------------
*       Compare   if (A < B)                  if (cmp(pstats, A, B) < 0)
*       Swap      tmp = A; A = B; B = tmp;    swap(pstats, &A, &B);
*       Move      A = B;                      A = move(pstats, B);
*
*/
