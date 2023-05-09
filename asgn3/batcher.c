#include "stats.h"

#include <stdint.h>
#include <stdlib.h>

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    //if (A[x] > A[y]) {
    if (cmp(stats, A[x], A[y]) > 0) {
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0) {
        return;
    }

    uint32_t t = 32 - __builtin_clz(n);
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0) {
            for (uint32_t i = 0; i < n - d; i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
/*
Purpose   Instead of this C code...   Use this C code...
*       -------   -------------------------   --------------------------
*       Compare   if (A < B)                  if (cmp(pstats, A, B) < 0)
*       Swap      tmp = A; A = B; B = tmp;    swap(pstats, &A, &B);
*       Move      A = B;                      A = move(pstats, B);
*/
