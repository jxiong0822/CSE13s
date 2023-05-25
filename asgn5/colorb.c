#include "bmp.h"
#include "io.h"

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int ch;
    Buffer *inputFile = NULL;
    Buffer *outputFile = NULL;
    bool igiven = false;
    bool ogiven = false;
    bool helpasked = false;
    opterr = 0;

    while ((ch = getopt(argc, argv, "i:o:h")) != -1) {
        switch (ch) {
        case 'i':
            //printf("Input file: \"%s\"\n", optarg);
            inputFile = read_open(optarg);
            //printf("image opened with read\n");
            igiven = true;
            break;

        case 'o':
            //printf("Output file: \"%s\"\n", optarg);
            outputFile = write_open(optarg);
            //printf("image opened with write\n");
            ogiven = true;
            break;
        case 'h':
            fprintf(stdout, "Usage: colorb -i infile -o outfile\n       colorb -h\n");
            helpasked = true;
            break;
        case '?': fprintf(stderr, "colorb: unknown or poorly formatted option -%c\n", optopt);
        }
    }
    if (!helpasked) {
        if (!igiven) {
            fprintf(stdout, "colorb:  -i option is required\n");
        }
        if (!ogiven) {
            fprintf(stdout, "colorb:  -o option is required\n");
        }
        if (!igiven || !ogiven) {
            fprintf(stdout, "Usage: colorb -i infile -o outfile\n       colorb -h\n");
        }
    }
    if (igiven || ogiven) {
        //printf("trying to make image\n");
        BMP *inpimage = bmp_create(inputFile);
        //printf("inpimage made\n");
        bmp_reduce_palette(inpimage);
        //printf("inpimage reduced\n");
        bmp_write(inpimage, outputFile);
        //printf("inpimage written\n");
        bmp_free(&inpimage);
        //printf("inpimage freed\n");
        read_close(&inputFile);
        write_close(&outputFile);
    }
}
