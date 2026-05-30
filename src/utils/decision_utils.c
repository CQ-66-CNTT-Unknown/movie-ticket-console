#include "../../include/utils/decision_utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool is_decision_yes(const char *prompt) {
    printf("%s [y/N(Mac dinh)]: ", prompt);
    char decision;
    scanf("%c", &decision);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clear buffer
    return tolower(decision) == 'y';
}
