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
    if (random) 
    {
        int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);
        int rand_i = rand_range(wordlist_count);
        strcpy(wordle_word, wordle_list[rand_i]);
    }
    else 
    {
        free(wordle_word);
        char* wordle_word = manual_word_select();
        if ((strcmp(wordle_word, "00000") == 0))
        {
            display_menu(game_info_menu("exiting game...", ""), NULL);
            return;
        }
    }
    bool game_won = false;
    for(size_t attempt_count = 0; (attempt_count < 5) && (!game_won); attempt_count++)
    {
        char* attempt_word = manual_word_select();
        if (strcmp(attempt_word, "00000") == 0)
        {
            display_menu(game_info_menu("exiting game...", ""), NULL);
            free(wordle_word);
            return;
        }
        attempt(attempt_count, attempt_word, wordle_word);
        if(attempt_word == wordle_word)
        {
            game_won = true;
        }
        free(attempt_word);
    }
    if (game_won)
    {
        display_menu(game_info_menu("victory", wordle_word), NULL);
    }
    else
    {
        display_menu(game_info_menu("defeat", wordle_word), NULL);      
    }
    free(wordle_word);
}

char* manual_word_select() 
{
    char* word = word_select();
    if((strcmp(word, "00000") == 0)  && (!word_in_list(word, (char**) wordle_list))  ) 
    {
        game_error_menu("word not found");
        return manual_word_select();
    }
    return word;
}

bool word_in_list(char* word, char** list) 
{
    int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);
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
    srand(time(NULL)); //Set a seed for the RNG
    return rand() / RAND_MAX * limit;
}

void attempt(int attempt_number, char* attempt, char* secret_word) 
{
    //Create copies of attempt and secret word to preserve original ones.

    char* attempt_copy = malloc(sizeof(char)*6);
    strcpy(attempt_copy, attempt);
    char* secret_word_copy = malloc(sizeof(char)*6);
    strcpy(secret_word_copy, secret_word);
    //First iteration: draws the letters on the grid and directly checks for matching letters and draws "green tiles" if the characters match.
    for (size_t position = 0; position < 5; position++) {
        draw_letter_on_grid(attempt_copy[position], position, attempt_number);
        if (attempt_copy[position] == secret_word_copy[position]) 
        {
            draw_letter_on_grid('g', position, attempt_number);
            attempt_copy[position] = '0';
            secret_word_copy[position] = '0';
        }
    
    for (size_t position = 0; position < 5; position++)
    {
        draw_letter_on_grid(attempt_copy[position], position, attempt_number);
        if (attempt_copy[position] == secret_word_copy[position]) 
        {
            draw_letter_on_grid('g', position, attempt_number);
            attempt_copy[position] = '0';
            secret_word_copy[position] = '0';
        }
    }
    
    //Second iteration: check for the remaining letters in the attempt if they appear anywhere in the remainder of the secret word.
    //If a match is found it will immediately set the first instances of the letter in both strings to 0 and draw a "yellow tile" on the place of the first
    //instance of this letter in the attempt. This correctly works out for duplicates from both sides :).

    for (size_t i = 0; i < 5; i++) 
    {
        if ((strchr(secret_word_copy, attempt_copy[i]) != NULL) && (attempt_copy[i] != '0')) 
        {
            draw_letter_on_grid('y', i, attempt_number);
            //Iteration to find the first instance of the letter appearing in the secret word without already being marked as a "green tile".
            for (size_t p = 0; p < 5; p++)
            {
                if (secret_word_copy[p] == attempt_copy[i]) 
                {
                    secret_word_copy[p] = '0';
                    attempt_copy[i] = '0';
                    break; //is this break not misplaced?
                }
            }
        }
    }
    //Third iteration: drawing "black tiles" for every tile that has not received a color in the previous 2 iterations.
    for (size_t l=0; l < 5; l++) 
    {
        if (attempt_copy[l] != '0') 
        {
            draw_letter_on_grid('b', l, attempt_number);
            attempt_copy[l] = '0';
        }
    }
    free(secret_word_copy);
    free(attempt_copy);
    }
}