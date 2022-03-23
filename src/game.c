#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <game.h>
#include <ui.h>

/*void worlde(bool random)
{
    srand(time(NULL));
    FILE* wordlist_file = fopen("worlde-word-list.txt", "r");
    char* wordbuffer = malloc(sizeof(char)*6);
    int wordlist_lines = 0;
    while (fread(wordbuffer, 6, 1, wordlist_file) == 6)
    {
        wordlist_lines += 1;
    }
    fseek(wordlist_file, 0, SEEK_SET);
    if (random)
    {
        int rand_i = rand_range(wordlist_lines);
        fseek(wordlist_file, rand_i*6, SEEK_CUR);
        fread(wordbuffer, 6, 1, wordlist_file);
    }
    else
    {
        wordbuffer = word_select();
        if(!word_in_list(wordbuffer, wordlist_file))

    }
    char* wordle_word = malloc(sizeof(char)*6);
    strcpy(wordle_word, wordbuffer);
}

bool word_in_list(char* word, FILE* list)
{
    
}


int rand_range(int limit)
{
    return rand() / RAND_MAX * limit;
}
*/