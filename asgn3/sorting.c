#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BITMASK 0x3FFFFFFF
//a bitmask of 1s 30 bits long

void print_set(Set set) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (set >> i) & 1);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {

    int ch;

    char *req_seed = NULL;
    char *req_arr_size = NULL;
    char *req_print_size = NULL;

    Set commands = set_empty();
    Set commands_2 = set_empty();
    srandom(13371453);
    int arr_size = 100;
    int elems_to_print = 100;

    while ((ch = getopt(argc, argv, "aisqbr:n:p:Hh")) != -1) {
        switch (ch) {
        case 'a':
            commands = set_insert(commands, 7); //adding insertion sort
            commands = set_insert(commands, 6); //adding shell sort
            commands = set_insert(commands, 5); //adding quick sort
            commands = set_insert(commands, 4); //adding batcher sort
            commands_2 = set_insert(commands, 0); // adding heapsort
            break;
        case 'i':
            commands = set_insert(commands, 7); //adding insertion sort
            break;
        case 's':
            commands = set_insert(commands, 6); //adding shell sort
            break;
        case 'q':
            commands = set_insert(commands, 5); //adding quick sort
            break;
        case 'b':
            commands = set_insert(commands, 4); //adding batcher sort
            break;
        case 'r':
            //int req_seed = atoi(optarg);
            req_seed = optarg;
            srandom(atoi(req_seed));
            //printf("Custom random seed requested; seed is %s\n", req_seed);
            commands = set_insert(commands, 3); //setting nonrandom seed asked
            break;
        case 'n':
            //int req_arr_size = atoi(optarg);
            req_arr_size = optarg;
            arr_size = atoi(req_arr_size);
            //printf("Custom array size requested; size is %i\n", arr_size);
            commands = set_insert(commands, 2); //setting nonrandom array size asked
            break;
        case 'p':
            //int req_print_size = atoi(optarg);
            req_print_size = optarg;
            if (atoi(req_print_size) > arr_size) {
                //printf("Print size requested exceeds array size; size is set to %i (array size)\n",
                //    arr_size);
                elems_to_print = arr_size;
                commands = set_insert(commands, 1); //setting print size asked
                break;
            } else {
                //printf("Requested custom print size; size is set to %s\n", req_print_size);
                elems_to_print = atoi(req_print_size);
                commands = set_insert(commands, 1); //setting print size asked
                break;
            }
        case 'H':
            commands = set_insert(commands, 0); //setting help asked
            break;
        case 'h': commands_2 = set_insert(commands, 0); // adding heapsort
        }
    }

    if (elems_to_print > arr_size) {
        elems_to_print = arr_size;
    }

    //generating the random array
    uint32_t *unsorted = malloc(arr_size * sizeof(uint32_t));

    for (int i = 0; i < arr_size; i++) {
        uint32_t r = random() & BITMASK;
        unsorted[i] = r;
    }

    //moving inital unsorted array into a temporary array so the initial is not lost
    uint32_t *unsorted_temp = malloc(arr_size * sizeof(uint32_t));

    for (int i = 0; i < arr_size; i++) {
        unsorted_temp[i] = unsorted[i];
    }

    //running sorting algorithms

    /*
    print_set(commands);
    bool i = set_member(commands, 2);
    printf("\n%d\n",i);
    commands = set_remove(commands, 2);
    print_set(commands);
    i = set_member(commands, 2);
    printf("\n%d\n",i);
    */

    if ((set_member(commands, 0)) || (commands == set_empty())) {
        fprintf(stderr,
            "Select at least one sort to perform.\nSYNOPSIS\n\tA collection of comparison-based "
            "sorting algorithms.\nUSAGE\n\t./sorting [-Hahbsqi] [-n length] [-p elements] [-r "
            "seed]\nOPTIONS\n\t-H              Display program help and usage.\n\t-a              "
            "Enable all sorts.\n\t-h              Enable Heap Sort.\n\t-b              Enable "
            "Batcher Sort.\n\t-s              Enable Shell Sort.\n\t-q              Enable Quick "
            "Sort.\n\t-i              Enable Insertion Sort.\n\t-n length       Specify number of "
            "array elements (default: 100).\n\t-p elements     Specify number of elements to print "
            "(default: 100).\n\t-r seed         Specify random seed (default: 13371453).\n");
    }

    Stats stats = { 0, 0 };

    //printf("\n\n%d\n\n", elems_to_print);

    //printf("\n\n%i\n\n",set_member(commands, 4));
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (set_member(commands, 7)) {
        reset(&stats);
        insertion_sort(&stats, unsorted_temp, arr_size); //run insertion sort
        print_stats(&stats, "Insertion Sort", arr_size);

        //printf("Generated sorted insertion array:\n");
        for (int i = 0; i < elems_to_print; i++) {
            if ((i % 5 == 0) && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, unsorted_temp[i]);
        }
        printf("\n");

        //reset temp array
        for (int i = 0; i < arr_size; i++) {
            unsorted_temp[i] = unsorted[i];
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (set_member(commands_2, 0)) {
        reset(&stats);
        heap_sort(&stats, unsorted_temp, arr_size); //run heap sort
        print_stats(&stats, "Heap Sort", arr_size);

        //printf("Generated sorted shell array:\n");
        for (int i = 0; i < elems_to_print; i++) {
            if ((i % 5 == 0) && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, unsorted_temp[i]);
        }
        printf("\n");

        //reset temp array
        for (int i = 0; i < arr_size; i++) {
            unsorted_temp[i] = unsorted[i];
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (set_member(commands, 6)) {
        reset(&stats);
        shell_sort(&stats, unsorted_temp, arr_size); //run shell sort
        print_stats(&stats, "Shell Sort", arr_size);

        //printf("Generated sorted shell array:\n");
        for (int i = 0; i < elems_to_print; i++) {
            if ((i % 5 == 0) && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, unsorted_temp[i]);
        }
        printf("\n");

        //reset temp array
        for (int i = 0; i < arr_size; i++) {
            unsorted_temp[i] = unsorted[i];
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (set_member(commands, 5)) {
        reset(&stats);
        quick_sort(&stats, unsorted_temp, arr_size); //run quick sort
        print_stats(&stats, "Quick Sort", arr_size);

        //printf("Generated sorted quick array:\n");
        for (int i = 0; i < elems_to_print; i++) {
            if ((i % 5 == 0) && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, unsorted_temp[i]);
        }
        printf("\n");

        //reset temp array
        for (int i = 0; i < arr_size; i++) {
            unsorted_temp[i] = unsorted[i];
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (set_member(commands, 4)) {
        reset(&stats);
        batcher_sort(&stats, unsorted_temp, arr_size); //run batcher sort
        print_stats(&stats, "Batcher Sort", arr_size);

        //printf("Generated sorted batcher array:\n");
        for (int i = 0; i < elems_to_print; i++) {
            if ((i % 5 == 0) && i != 0) {
                printf("\n");
            }
            printf("%13" PRIu32, unsorted_temp[i]);
        }
        printf("\n");
    }

    //freeing used memory
    free(unsorted);
    free(unsorted_temp);
    return 0;
}
