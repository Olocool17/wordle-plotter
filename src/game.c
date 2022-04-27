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
        attempt(attempt_count, attempt_word, wordle_word);
        if(attempt_word == wordle_word)
        {
            //victory termination procedure
            free(attempt_word);
            display_menu(game_info_menu("victory", wordle_word), NULL);
            free(wordle_word);
            return;
        }
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
    if((strcmp(word, "00000") == 0) && (!word_in_list(word, (char**) wordle_list))) 
    {
        game_error_menu("word not found");
        return manual_word_select(attempt_count);
    }
    return word;
}

bool word_in_list(char* word, char** list) 
{
    int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);
    for (size_t i = 0; (i < wordlist_count) && (strcmp_P(word, (char*)pgm_read_ptr(&(list[i]))) == 0); i++) 
    {
        return true;
    }
    return false;
}

int rand_range(int limit) 
{
    srand(time(NULL)); //Set a seed for the RNG
    return rand() / RAND_MAX * limit;
}

//applying the wordle word comparison logic between the guessed word and secret word
void attempt(int attempt_number, char* attempt, char* secret_word) 
{
    char* attempt_copy = malloc(sizeof(char)*6);
    strcpy(attempt_copy, attempt);
    char* secret_word_copy = malloc(sizeof(char)*6);
    strcpy(secret_word_copy, secret_word);    
//first iteration: draws the letters on the grid and directly checks for matching letters and draws "green tiles" if the characters match.
    for (size_t i = 0; i < 5; i++) 
    {
        draw_letter_on_grid(attempt_copy[i], i, attempt_number);
        if (attempt_copy[i] == secret_word_copy[i]) 
        {
            draw_letter_on_grid('g', i, attempt_number);
            attempt_copy[i] = '0';
            secret_word_copy[i] = '0';
        }
    }
/* second iteration: check for the remaining letters in if they appear anywhere in the remainder of the secret word
   if a match is found it will set the first instances of the letter in both strings to 0 and draw a "yellow tile" on the place of the first instance of
   this letter in the attempt */  
    for (size_t i = 0; (i < 5) && ((strchr(secret_word_copy, attempt_copy[i]) != NULL) && (attempt_copy[i] != '0')); i++) 
    { 
        draw_letter_on_grid('y', i, attempt_number);
        //Finding the first instance of the letter appearing in the secret word that is not already marked as a "green tile".
        for (size_t p = 0; (p < 5) && (secret_word_copy[p] == attempt_copy[i]); p++)
        {
            secret_word_copy[p] = '0';
            attempt_copy[i] = '0';
            break; 
        }
    }
//third iteration: drawing "black tiles" for every tile in the guessed word that hasn't been assigned a color yet.
    for (size_t i=0; (i < 5) && (attempt_copy[i] != '0'); i++) 
    {
        draw_letter_on_grid('b', i, attempt_number);
    }
    free(secret_word_copy);
    free(attempt_copy);
}