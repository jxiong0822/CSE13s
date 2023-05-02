#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int ch;

    int saw_e;
    int saw_b;
    int saw_m;
    int saw_r;
    int saw_v;
    int saw_w;
    int saw_n;
    int saw_s;
    int saw_h;

    while ((ch = getopt(argc, argv, "aebmrvwnsh")) != -1) {
        switch (ch) {
        case 'a':
            saw_e = 1;
            saw_b = 1;
            saw_m = 1;
            saw_r = 1;
            saw_v = 1;
            saw_w = 1;
            saw_n = 1;
        case 'e': saw_e = 1; break;
        case 'b': saw_b = 1; break;
        case 'm': saw_m = 1; break;
        case 'r': saw_r = 1; break;
        case 'v': saw_v = 1; break;
        case 'w': saw_w = 1; break;
        case 'n': saw_n = 1; break;
        case 's': saw_s = 1; break;
        case 'h': saw_h = 1; break;
        default: saw_h = 1;
        }
    }
    if ((saw_e + saw_b + saw_m + saw_r + saw_v + saw_w + saw_n) == 0) {
        printf(
            "SYNOPSIS:\n\tA number of sequential tests for e and pi.\nUSAGE\n\t./mathlib-test-x86 "
            "-[aebmrvnsh]\nOPTIONS\n\t-a   Runs all tests.\n\t-e   Runs e test.\n\t-b   Runs BBP "
            "pi test.\n\t-m   Runs Madhava pi test.\n\t-r   Runs Euler pi test.\n\t-v   Runs Viete "
            "pi test.\n\t-w   Runs Wallis pi test.\n\t-n   Runs Newton square root tests.\n\t-s   "
            "Print verbose statistics.\n\t-h   Display program synopsis and usage.\n");
    }

    if (saw_h) {
        printf(
            "SYNOPSIS:\n\tA number of sequential tests for e and pi.\nUSAGE\n\t./mathlib-test-x86 "
            "-[aebmrvnsh]\nOPTIONS\n\t-a   Runs all tests.\n\t-e   Runs e test.\n\t-b   Runs BBP "
            "pi test.\n\t-m   Runs Madhava pi test.\n\t-r   Runs Euler pi test.\n\t-v   Runs Viete "
            "pi test.\n\t-w   Runs Wallis pi test.\n\t-n   Runs Newton square root tests.\n\t-s   "
            "Print verbose statistics.\n\t-h   Display program synopsis and usage.\n");
    }

    if (saw_e) {
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, (M_E - e()));
        if (saw_s) {
            printf("e() terms = %i\n", e_terms());
        }
    }

    if (saw_b) {
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI,
            (M_PI - pi_bbp()));
        if (saw_s) {
            printf("pi_bbp() terms = %i\n", pi_bbp_terms());
        }
    }

    if (saw_m) {
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
            (M_PI - pi_madhava()));
        if (saw_s) {
            printf("pi_madhava() terms = %i\n", pi_madhava_terms());
        }
    }

    if (saw_r) {
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI,
            M_PI - pi_euler());
        if (saw_s) {
            printf("pi_euler() terms = %i\n", pi_euler_terms());
        }
    }

    if (saw_v) {
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI,
            M_PI - pi_viete());
        if (saw_s) {
            printf("pi_viete() terms = %i\n", pi_viete_factors());
        }
    }

    if (saw_w) {
        printf("pi_wallis() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_wallis(), M_PI,
           M_PI - pi_wallis());
        if (saw_s) {
            printf("pi_wallis() terms = %i\n", pi_wallis_factors());
        }
    }

    if (saw_n) {
        for (float i = 0; i < 9.95; i += 0.1) {
            double temp_newton = sqrt_newton(i);
            printf("sqrt_newton(%.2f) = %16.15lf, sqrt(%.2f) = %16.15lf, diff = %16.15lf\n", i,
                temp_newton, i, sqrt(i), sqrt(i) - temp_newton);
            if (saw_s) {
                printf("sqrt_newton() terms = %i\n", sqrt_newton_iters());
            }
        }
    }

    return 0;
}
