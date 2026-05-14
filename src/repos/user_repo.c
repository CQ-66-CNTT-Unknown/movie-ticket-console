#include "../../include/repos/user_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER_SOURCE_PATH "data/user.csv"
#define N_USER_FIELDS 4

User *get_user_by_credentials(const char *username, const char *password, const char *user_source_path) {
    FILE *user_source = fopen(user_source_path, "r");
    if (user_source == NULL) {
        fprintf(stderr, "Error opening user source file: %s\n", user_source_path);
        return NULL;
    }

    User *found_user = NULL;
    char buffer[LINE_DATA_BUFFER_SIZE];
    
    fgets(buffer, sizeof(buffer), user_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), user_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_USER_FIELDS];
        parse_csv_line(buffer, fields, N_USER_FIELDS);

        // Check if username matches
        if (strcmp(fields[1], username) == 0) {
            // Check if password matches
            if (strcmp(fields[2], password) == 0) {
                found_user = (User *) malloc(sizeof(User));
                if (found_user == NULL) {
                    fprintf(stderr, "Memory allocation failed for user\n");
                    fclose(user_source);
                    return NULL;
                }

                found_user->user_id = atoi(fields[0]);
                strncpy(found_user->username, fields[1], sizeof(found_user->username) - 1);
                found_user->username[sizeof(found_user->username) - 1] = '\0';
                found_user->role = (UserRole) atoi(fields[3]);

                fclose(user_source);
                return found_user;
            }
        }
    }

    fclose(user_source);
    return NULL; // User not found or password incorrect
}

User *get_user_by_id(int user_id, const char *user_source_path) {
    FILE *user_source = fopen(user_source_path, "r");
    if (user_source == NULL) {
        fprintf(stderr, "Error opening user source file: %s\n", user_source_path);
        return NULL;
    }

    User *found_user = NULL;
    char buffer[LINE_DATA_BUFFER_SIZE];
    
    fgets(buffer, sizeof(buffer), user_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), user_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_USER_FIELDS];
        parse_csv_line(buffer, fields, N_USER_FIELDS);

        if (atoi(fields[0]) == user_id) {
            found_user = (User *) malloc(sizeof(User));
            if (found_user == NULL) {
                fprintf(stderr, "Memory allocation failed for user\n");
                fclose(user_source);
                return NULL;
            }

            found_user->user_id = atoi(fields[0]);
            strncpy(found_user->username, fields[1], sizeof(found_user->username) - 1);
            found_user->username[sizeof(found_user->username) - 1] = '\0';
            found_user->role = (UserRole) atoi(fields[3]);

            fclose(user_source);
            return found_user;
        }
    }

    fclose(user_source);
    return NULL;
}
