#include "mathlib.h"

#include <stdio.h>

static int iters;
static double k = 1;
static double running_additive = 0;
static double sum = 1;

double pi_viete(void) {
    do {
        running_additive = 0;
        for (int i = 1; i <= k; i++) {
            running_additive = sqrt_newton(running_additive + 2);
            //printf("running additive:%.15f\n",running_additive);
        }
        running_additive /= 2;
        //printf("running additive:%.15f\n", running_additive);
        sum *= running_additive;
        k += 1;
        iters += 1;
        //printf("current sum:%.15f\n", sum);
        //printf("margin of error::%.15f\n", absolute(1.0 - running_additive));
    } while (absolute(1.0 - running_additive) > EPSILON);
    return 2 / sum;
}

int pi_viete_factors(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
