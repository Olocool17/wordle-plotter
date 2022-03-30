#ifndef GAME_LOADED
#define GAME_LOADED

//plays the wordle game, selects a random word if 'random' is true
void wordle(bool random);

char* manual_word_select();

bool word_in_list(char* word, char** list);

int rand_range(int limit);

void poging(int pogingnummer, char* attempt, char* secret_word);

#endif