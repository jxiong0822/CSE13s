#include "mathlib.h"

static int iters;

static double k = 1;
static double running_additive = 1;
static double sum = 0;

double pi_euler(void) {
    do {
        //printf("running additive:%.15f\n",running_additive);
        running_additive = 1 / k;
        running_additive /= k;
        //printf("running additive:%.15f\n",running_additive);
        sum += running_additive;
        k += 1;
        iters += 1;
        //printf("current sum:%.15f\n",sum);
    } while (absolute(running_additive) > EPSILON);
    return sqrt_newton(6 * sum);
}

int pi_euler_terms(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
