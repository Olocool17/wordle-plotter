#ifndef GAME_LOADED
#define GAME_LOADED

//Handles the wordle game's logic and drawing

/*  
    @brief Starts the wordle game.
    @param random Determines if the secret word should be random or manually selected at the start of the game 
*/
void wordle(bool random);

/*
    @brief Prompts the user to enter a word and checks if this word is in the wordle list.
    @param attempt_count The attempt count that should be displayed. Use -1 to not display an attempt count.
    @return The manually selected word.
*/
char* manual_word_select(int attempt_count);

/*
    @brief Checks if a word is in a list of words.
    @param word Word to be checked
    @param list List to be sought through
    @return Whether the word is in the list or not
*/
bool word_in_list(char* word, char** list);

/*
    @brief Finds a random number in a range.
    @param limit Maximum value for the random number
    @return An integer between 0 and the limit
*/
int rand_range(int limit);

/*
    @brief Handles the visualization of the result of the attempts made by the player.
    @param attempt_number The number of attempts that have already occurred
    @param attempt The player's word attempt
    @param secret_word The word to be guessed
*/
void attempt(int attempt_number, char* attempt, char* secret_word);

#endif