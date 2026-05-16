#include "../../include/constanst.h"
#include "../../include/models/user.h"
#include "../../include/repos/user_repo.h"
#include "../../include/utils/input_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

User* login() {
    char username[50];
    char password[50];

    while (1) {
        printf(RED"\n===== LOGIN =====\n" RESET);
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        User *found_user = get_user_by_credentials(username, password, USER_SOURCE_PATH);

        if (found_user != NULL) {
            printf("Login successful! Welcome, %s!\n", found_user->username);
            return found_user;
        } else {
            printf("Invalid username or password. Please try again.\n");
        }
    }
}
