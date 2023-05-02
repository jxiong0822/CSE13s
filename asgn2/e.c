#include "mathlib.h"

static int iters;

static double k = 0.0;
static double running_additive = 1.0;
static double sum = 0;

double e(void) {
    do {
        sum += running_additive;
        running_additive *= (1 / (k + 1));
        k += 1;
        iters += 1;
        //printf("tf is going on %i\n", iters);
        //printf("running additive:%.15f\n",running_additive);
    } while (running_additive > EPSILON);
    //printf("ending sum; %.10f\n",sum);
    return sum;
}

int e_terms(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
