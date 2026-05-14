#include "../../include/utils/get_temp_file_path.h"
#include "../../include/constanst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_temp_file_path(const char *original_path) {
    if (original_path == NULL)
        return NULL;

    const char *last_slash = strrchr(original_path, '/');

    size_t dir_len = (last_slash != NULL) ? (last_slash - original_path) + 1 : 0;

    char *temp_path = (char *) malloc(dir_len + 8 + 1);
    if (temp_path == NULL)
        return NULL;

    if (dir_len > 0) {
        strncpy(temp_path, original_path, dir_len);
        temp_path[dir_len] = '\0';
    } else
        temp_path[0] = '\0';

    strcat(temp_path, "temp.csv");

    return temp_path;
}
