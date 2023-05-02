#include "mathlib.h"

#include <stdio.h>

static int iters;
static double k = 1;
static double running_additive = 1;
static double sum = 1;

double pi_wallis(void) {
    do {
        running_additive = (4 * k * k) / ((4 * k * k) - 1);
        //printf("running additive:%.15f\n",running_additive);
        sum *= running_additive;
        k += 1;
        iters += 1;
        //printf("current sum:%.15f\n",sum);
        //printf("margin of error::%.15f\n",absolute(1.0 - running_additive));
    } while (absolute(1.0 - running_additive) > EPSILON);
    //printf("margin of error::%.15f\n", absolute(1.0 - running_additive));
    return 2 * sum;
}

int pi_wallis_factors(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
