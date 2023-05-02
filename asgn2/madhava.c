#include "mathlib.h"

static int iters;

static double k = 0.0;
static double running_additive;
static double sum = 0;

double pi_madhava(void) {
    do {
        running_additive = 1 / (2 * k + 1);
        //printf("running additive:%.15f\n",running_additive);
        for (double i = 1; i <= k; i++) {
            running_additive /= -3;
        }
        //printf("running additive:%.15f\n",running_additive);
        //printf("abs running additive:%.15f\n",absolute(running_additive));
        sum += running_additive;
        k += 1;
        iters += 1;
        //printf("current sum:%.15f\n",sum);
    } while (absolute(running_additive) > EPSILON);
    return sqrt_newton(12) * sum;
}

int pi_madhava_terms(void) {
    //printf("tf is going on %i\n", iters);
    return iters;
}
