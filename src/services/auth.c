#include "../../include/models/user.h"
#include "../../include/repos/user_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/input_utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
User login() {
    User invalid_user = {-1, "", 0};
    char username[50];
    char password[50];
    
    while (1) {
        printf("\n===== LOGIN =====\n");
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';
        
        printf("Enter password: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';
        
        User *found_user = get_user_by_credentials(username, password, USER_SOURCE_PATH);
        
        if (found_user != NULL) {
            User result = *found_user;
            free(found_user);
            printf("\nLogin successful! Welcome %s!\n", result.username);
            return result;
        } else {
            printf("\nInvalid username or password. Please try again.\n");
        }
    }
}
