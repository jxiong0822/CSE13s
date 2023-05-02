#include "mathlib.h"

static int iters = 0;

double sqrt_newton(double x) {
    double next_y = 1.0;
    double y = 0.0;
    while (absolute(next_y - y) > EPSILON) {
        y = next_y;
        next_y = 0.5 * (y + x / y);
        iters += 1;
    }
    return next_y;
}

int sqrt_newton_iters(void) {
    int temp = iters;
    iters = 0;
    return temp;
}
