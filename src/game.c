#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wordlelist.h>
#include <functions.h>
#include <game.h>
#include <ui.h>

void wordle(bool random) 
{
    char* wordle_word = malloc(sizeof(char)*6);
    //if the player chose to randomly select a word
    if (random) 
    {
        int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);
        int rand_i = rand_range(wordlist_count);
        strcpy_P(wordle_word, (char*)pgm_read_ptr(&(wordle_list[rand_i])));
    }
    //if the player chose to manually select a word
    else 
    {
        free(wordle_word);
        char* wordle_word = manual_word_select(-1);
        if ((strcmp(wordle_word, "00000") == 0))
        {
            display_menu(game_info_menu("exiting game...", ""), NULL);
        }
    }
    for(size_t attempt_count = 0; attempt_count < 6; attempt_count++)
    {
        char* attempt_word = manual_word_select(attempt_count);
        if (strcmp(attempt_word, "00000") == 0)
        {
            display_menu(game_info_menu("exiting game...", ""), NULL);
            free(wordle_word);
        }
        if(strcmp(attempt_word, wordle_word) == 0)
        {
            //victory termination procedure
            attempt(attempt_count, attempt_word, wordle_word);
            free(attempt_word);
            display_menu(game_info_menu("victory", wordle_word), NULL);
            free(wordle_word);
            return;
        }
        attempt(attempt_count, attempt_word, wordle_word);
        free(attempt_word);
    }
    //defeat termination procedure
    display_menu(game_info_menu("defeat", wordle_word), NULL);      
    free(wordle_word);
}

char* manual_word_select(int attempt_count) 
{
    char* word = word_select(attempt_count);
    //returning the user to word_select if they entered an invalid word
    if((strcmp(word, "00000") == 0) || (!word_in_list(word, (char**) wordle_list))) 
    {
        display_menu(game_error_menu("word not found"), NULL);
        return manual_word_select(attempt_count);
    }
    return word;
}

bool word_in_list(char* word, char** list) 
{
    int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);
    for (size_t i = 0; i < wordlist_count; i++) 
    {
        if (strcmp_P(word, (char*)pgm_read_ptr(&(list[i]))) == 0 ) return true;
    }
    return false;
}

int rand_range(int limit) 
{
    srand(time(NULL)); //Set a seed for the RNG
    return (int)((rand() / (float)RAND_MAX )* limit);
}

void attempt(int attempt_number, char* attempt, char* secret_word) 
{
    char* secret_word_copy = malloc(sizeof(char)*6);
    strcpy(secret_word_copy, secret_word);
    //Iteration for every letter in the secret word
    for (size_t i = 0; i < 5; i++) 
    {
        draw_letter_on_grid(attempt[i], i, attempt_number);
        //Check for every letter in the secret word if that letter is the same in the attempt
        if (attempt[i] == secret_word[i]) 
        {
            draw_letter_on_grid('g', i, attempt_number);
            attempt[i] = '0';
            secret_word_copy[i] = '0';
        }
    }    
    //if not, check if the letter appears anywhere else in the attempt, from left to right
    for (size_t i = 0; i < 5; i++) {
        if (secret_word_copy[i] != '0') {
            for (size_t p = 0; p < 5; p++) {
                if (secret_word[i] == attempt[p]) {
                    draw_letter_on_grid('y', p, attempt_number);
                    attempt[p] = '0';
                    secret_word_copy[i] = '0';
                    break; 
                }
            }
        }
    //any letter in the attempt that hasn't been assigned a color after all 5 letters in the secret word have been checked gets assigned a black value now
    for (size_t i=0; i < 5; i++) {
        if (attempt[i] != '0') draw_letter_on_grid('b', i, attempt_number);
    }
}