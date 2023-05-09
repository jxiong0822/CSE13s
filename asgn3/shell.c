#include "gaps.h"
#include "stats.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {

    for (uint32_t gap_index = 0; gap_index < GAPS; gap_index++) {

        uint32_t gap = gaps[gap_index];

        for (uint32_t i = gap; i < n; i++) {

            uint32_t j = i;

            uint32_t temp = move(stats, A[i]);

            while (j >= gap && (cmp(stats, A[j - gap], temp) == 1)) {

                A[j] = move(stats, A[j - gap]);

                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
}
