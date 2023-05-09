#include "stats.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j++) {

        if (cmp(stats, A[j - 1], A[hi - 1]) < 0) {
            i += 1;

            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }

    swap(stats, &A[i], &A[hi - 1]);
    return (i + 1);
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi);
        quick_sorter(stats, A, lo, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}

/*
*       Purpose   Instead of this C code...   Use this C code...
*       -------   -------------------------   --------------------------
*       Compare   if (A < B)                  if (cmp(pstats, A, B) < 0)
*       Swap      tmp = A; A = B; B = tmp;    swap(pstats, &A, &B);
*       Move      A = B;                      A = move(pstats, B);


uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
	uint32_t i = lo - 1;
	for (uint32_t j = lo; j < hi; j++) {
		if (A[j - 1] < A[hi - 1]) {
			i += 1;
			//swap A[i-1], A[j-1];
			swap(stats, &A[i-1], &A[j-1]);
		}
	}
	//swap A[i], A[hi - 1]
	swap(stats, &A[i], &A[hi - 1]);
	return (i + 1);
}

void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
	if (lo < hi) {
		uint32_t p = partition(stats, A, lo, hi);
		quick_sorter(stats, A, lo, p - 1);
		quick_sorter(stats, A, p + 1, hi);
	}
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n-1);
}
*/