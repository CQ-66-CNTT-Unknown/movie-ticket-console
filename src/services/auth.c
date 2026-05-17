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
        printf("Ten dang nhap: ");
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        printf("Mat khau: ");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0';

        User *found_user = get_user_by_credentials(username, password, USER_SOURCE_PATH);

        if (found_user != NULL) {
            printf("Dang nhap thanh cong! Chao mung, %s!\n", found_user->username);
            return found_user;
        } else {
            printf("Ten dang nhap hoac mat khau khong dung. Vui long thu lai.\n");
        }
    }
}
