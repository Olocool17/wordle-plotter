#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wordlelist.h>

#include <functions.h>
#include <game.h>
#include <ui.h>

const int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);

void wordle(bool random) {
    srand(time(NULL));
    if (random) {
        char* wordle_word = malloc(sizeof(char)*6);
        int rand_i = rand_range(wordlist_count);
        strcpy(wordle_word, wordle_list[rand_i]);
    }
    else {
        char* wordle_word = manual_word_select();
    }
}

char* manual_word_select() {
    char* word = word_select();
    if(!word_in_list(word, (char**) wordle_list)) {
        game_error_menu("word not found");
        return manual_word_select();
    }
    return word;
}

bool word_in_list(char* word, char** list) {
    for (size_t i = 0; i < wordlist_count; i++) {
        if (strcmp(word, list[i]) == 0) {
            return true;
        }
    }
    return false;
}

int rand_range(int limit) {
    return rand() / RAND_MAX * limit;
}

//Poging: handles the visualization of the result of the attempts made by the player.
void poging(int pogingnummer, char* attempt, char* secret_word) {
    //Create copies of attempt and secret word to preserve original ones.
    char* attempt_copy = malloc(sizeof(char)*6);
    strcpy(attempt_copy, attempt);
    char* secret_word_copy = malloc(sizeof(char)*6);
    strcpy(secret_word_copy, secret_word);
    //First iteration: draws the letters on the grid and directly checks for matching letters and draws "green tiles" if the characters match.
    for (size_t positie = 0; positie < 5; positie++) {
        switch(attempt_copy[positie]) {
            case 'A':
                draw_A(positie, pogingnummer - 1);
            case 'B':
                draw_B(positie, pogingnummer - 1);
            case 'C':
                draw_C(positie, pogingnummer - 1);
            case 'D':
                draw_D(positie, pogingnummer - 1);
            case 'E':
                draw_E(positie, pogingnummer - 1);
            case 'F':
                draw_F(positie, pogingnummer - 1);
            case 'G':
                draw_G(positie, pogingnummer - 1);
            case 'H':
                draw_H(positie, pogingnummer - 1);
            case 'I':
                draw_I(positie, pogingnummer - 1);
            case 'J':
                draw_J(positie, pogingnummer - 1);
            case 'K':
                draw_K(positie, pogingnummer - 1);
            case 'L':
                draw_L(positie, pogingnummer - 1);
            case 'M':
                draw_M(positie, pogingnummer - 1);
            case 'N':
                draw_N(positie, pogingnummer - 1);
            case 'O':
                draw_O(positie, pogingnummer - 1);
            case 'P':
                draw_P(positie, pogingnummer - 1);
            case 'Q':
                draw_Q(positie, pogingnummer - 1);
            case 'R':
                draw_R(positie, pogingnummer - 1);
            case 'S':
                draw_S(positie, pogingnummer - 1);
            case 'T':
                draw_T(positie, pogingnummer - 1);
            case 'U':
                draw_U(positie, pogingnummer - 1);
            case 'V':
                draw_V(positie, pogingnummer - 1);
            case 'W':
                draw_W(positie, pogingnummer - 1);
            case 'X':
                draw_X(positie, pogingnummer - 1);
            case 'Y':
                draw_Y(positie, pogingnummer - 1);
            case 'Z':
                draw_Z(positie, pogingnummer - 1);
        }
        if (attempt_copy[positie] == secret_word_copy[positie]) {
            draw_green(positie, pogingnummer - 1);
            attempt_copy[positie] = '0';
            secret_word_copy[positie] = '0';
        }
    }
    //Second iteration: check for the remaining letters in the attempt if they appear anywhere in the remainder of the secret word.
    //If a match is found it will immediately set the first instances of the letter in both strings to 0 and draw a "yellow tile" on the place of the first
    //instance of this letter in the attempt. This correctly works out for duplicates from both sides :).
    for (size_t i = 0; i < 5; i++) {
        if (strstr(secret_word_copy, attempt_copy[i])!=NULL && attempt_copy[i]!='0') {
            draw_yellow(i, pogingnummer - 1);
            //Iteration to find the first instance of the letter appearing in the secret word without already being marked as a "green tile".
            int p = 0;
            while (1) {
                if (secret_word_copy[p] == attempt_copy[i]) {
                    secret_word_copy[p] = '0';
                    p = 0;
                    attempt_copy[i] = '0';
                    break;
                }
                p += 1;
            }
        }
    }
    //Third iteration: drawing "black tiles" for every tile that has not receieved a color in the previous 2 iterations.
    for (size_t l=0; l < 5; l++) {
        if (attempt_copy[l] != '0') {
            draw_black(l, pogingnummer - 1);
            attempt_copy[l] = '0';
        }
    }
    free(secret_word_copy);
    free(attempt_copy);
}