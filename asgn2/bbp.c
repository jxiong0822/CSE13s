#include "mathlib.h"

static int iters;

static double k = 0.0;
static double running_additive;
static double sum = 0;

double pi_bbp(void) {
    do {
        running_additive = (4 / (8 * k + 1) - 2 / (8 * k + 4) - 1 / (8 * k + 5) - 1 / (8 * k + 6));
        //printf("running additive:%.15f\n",running_additive);
        for (double i = 1; i <= k; i++) {
            running_additive /= 16.0;
        }
        //printf("running additive:%.15f\n",running_additive);
        sum += running_additive;
        k += 1;
        iters += 1;
        //printf("current sum:%.15f\n",sum);
    } while (running_additive > EPSILON);
    return sum;
}

int pi_bbp_terms(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
