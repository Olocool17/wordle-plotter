#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wordlelist.h>

#include <game.h>
#include <ui.h>

const int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);

void wordle(bool random)
{
    srand(time(NULL));
    if (random)
    {
        char* wordle_word = malloc(sizeof(char)*6);
        int rand_i = rand_range(wordlist_count);
        strcpy(wordle_word, wordle_list[rand_i]);
    }
    else
    {
        char* wordle_word = manual_word_select();
    }
}

char* manual_word_select()
{
    char* word = word_select();
    if(!word_in_list(word, (char**) wordle_list))
    {
        game_error_menu("word not found");
        return manual_word_select();
    }
    return word;
}
bool word_in_list(char* word, char** list)
{
    for (size_t i = 0; i < wordlist_count; i++)
    {
        if (strcmp(word, list[i]) == 0)
        {
            return true;
        }
    }
    return false;
}


int rand_range(int limit)
{
    return rand() / RAND_MAX * limit;
}

void poging(int pogingnummer, char* attempt, char* secret_word) {
    //Create copies of attempt and secret word to preserve original ones.
    char* attempt_copy = malloc(sizeof(char)*6);
    strcpy(attempt_copy, attempt);
    char* secret_word_copy = malloc(sizeof(char)*6);
    strcpy(secret_word_copy, secret_word);
    //First iteration: directly check for matching letters and draw green tiles if the characters match.
    int positie = 0;
    while (positie < 5) {
        if (attempt_copy[positie] == secret_word_copy[positie]) {
            draw_green(positie, pogingnummer - 1);
            attempt_copy[positie] = '0';
            secret_word_copy[positie] = '0';
        }
    positie += 1;
    }
    //Second iteration: check for the remaining letters in the attempt if they appear anywhere in the remainder of the secret word.
    //If a match is found it will immediately set the first instances of the letter in both strings to 0 and draw a yellow tile on the place of the first
    //instance of this letter in the attempt. This correctly works out for duplicates from both sides :).
    int i = 0;
    while (i < 5) {
        if (strstr(secret_word_copy, attempt_copy[i])!=NULL && attempt_copy[i]!='0') {
            draw_yellow(i, pogingnummer - 1);
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
        i += 1;
    }
    //Third iteration: drawing black tiles for every tile that has not receieved a color in the previous 2 iterations.
    int l = 0;
    while (l<5) {
        if (attempt_copy[l] != '0') {
            draw_black(l, pogingnummer - 1);
            attempt_copy[l] = '0';
        l += 1;
        }
    }
    //Free copies to prevent memory leak.
    free(secret_word_copy);
    free(attempt_copy);
}