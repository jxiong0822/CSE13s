#include "names.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    //Asking for number of players input
    int num_of_players = 2;
    printf("Number of players (2 to 10)? ");
    int input_check = scanf("%d", &num_of_players);

    //printf("num players : %d \n", num_of_players);
    //printf("input is int? : %d \n", input_check);

    if (input_check < 1 || num_of_players < 2 || num_of_players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        num_of_players = 2;
    }
    //Asking for random number generator seed
    int generation_seed = 2023;
    printf("Random-number seed? ");
    input_check = scanf("%d", &generation_seed);

    //printf("generation seed : %d \n", generation_seed);
    //printf("input is int? : %d \n", input_check);

    if (input_check < 1) {
        fprintf(stderr, "Invalid seed. Using 2023 instead.\n");
        generation_seed = 2023;
    }
    //setting srandom seed to input(or default)
    srandom(generation_seed);

    int score[num_of_players];
    for (int i = 0; i < num_of_players; i++) {
        score[i] = 0;
        //	printf("%d\n", score[i]);
    }

    const int points[7] = { 0, 0, 10, 10, 15, 5, 5 };

    int current_player = 0;
    int dice_roll = 0;

    printf("%s\n", player_name[0]);
    while (true) {
        dice_roll = random() % 7;
        score[current_player] += points[dice_roll];
        printf(" rolls %d, has %d\n", points[dice_roll], score[current_player]);
        //checks for and declares for winner
        if (score[current_player] >= 100) {
            printf("%s won!\n", player_name[current_player]);
            break;
        }
        //next player if roll = 0
        if (dice_roll == 0 || dice_roll == 1) {
            if (current_player == num_of_players - 1) {
                current_player = 0;
            } else {
                current_player += 1;
            }
            printf("%s\n", player_name[current_player]);
            //printf("current player index:%d\n",current_player);
        }
    }
    return 0;
}
