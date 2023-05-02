#include "mathlib.h"

static int iters;

double sqrt_newton(double x) {
    double next_y = 1.0;
    double y = 0.0;
    while ((absolute(next_y - y)) > EPSILON) {
        y = next_y;
        next_y = 0.5 * (y + x / y);
        iters += 1;
    }
    return y;
}

int sqrt_newton_iters(void) {
    return iters;
}
