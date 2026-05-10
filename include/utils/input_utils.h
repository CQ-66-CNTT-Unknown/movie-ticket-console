#pragma once

/**
 *@brief Prompt the user for an ID input and validate it
 *@param prompt The message to display to the user when asking for input
 *@param buffer_size The size of the input buffer to read the user's input
 *@return The valid integer ID entered by the user, or -1 if the input is invalid
 */
int input_id(const char *prompt, int buffer_size);

/**
 * @brief delete the '\n' character of a string
 * @param s the pointer to string will be deleted Enter character.
 */
void deleteEnter(char *s);

/**
 * @brief Prompts the user to enter a number to select a function or exit.
 * @param choice Pointer to the variable that stores the input value.
 */
void inputNumber(int* choice);