#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wordlelist.h>

#include <game.h>
#include <ui.h>

const int wordlist_count = sizeof(wordle_list) / sizeof(wordle_list[0]);

void worlde(bool random)
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
