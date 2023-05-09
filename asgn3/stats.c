/*
* This file provides the C code for a sorting-algorithm statistics library.
*
* By calling these functions in your sorting routine, you will
* automatically get statistics on the number of comparisons, the number of
* swaps, and the number of moves.
*
* How to Use the Functions:
*
*   1. In the test harness, before calling the sorting functions, declare
*      a Stats variable.
*
*       Stats stats;
*
*   2. Before calling each sorting function, reset the statistics.  You'll
*      use the & prefix operator to get a pointer to the stats variable.
*      By doing this, the reset() function can clear the statistics.
*
*       reset(&stats);
*
*   2. Within your sorting function, use these functions for the indicated
*      purposes.
*
*       Purpose   Instead of this C code...   Use this C code...
*       -------   -------------------------   --------------------------
*       Compare   if (A < B)                  if (cmp(pstats, A, B) < 0)
*       Swap      tmp = A; A = B; B = tmp;    swap(pstats, &A, &B);
*       Move      A = B;                      A = move(pstats, B);
*
*   3. In the test harness, after calling your sorting function, print the
*      collected statistics by calling print_stats().  The algorithm_name
*      parameter is a string with the name of the algorithm.
*
*       print_stats(&stats, algorithm_name, num_elements);
*/

#include "stats.h"

#include <stdio.h>

int cmp(Stats *stats, uint32_t x, uint32_t y) {
    stats->compares += 1;
    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t move(Stats *stats, uint32_t x) {
    stats->moves += 1;
    return x;
}

void swap(Stats *stats, uint32_t *x, uint32_t *y) {
    stats->moves += 3;
    uint32_t t = *x;
    *x = *y;
    *y = t;
}

void reset(Stats *stats) {
    stats->moves = stats->compares = 0;
}

void print_stats(Stats *stats, const char *algorithm_name, uint32_t num_elements) {
    printf("%s, %" PRIu32 " elements, "
           "%" PRIu64 " moves, "
           "%" PRIu64 " compares\n",
        algorithm_name, num_elements, stats->moves, stats->compares);
}
