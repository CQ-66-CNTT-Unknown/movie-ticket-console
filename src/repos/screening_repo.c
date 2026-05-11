#include "../../include/repos/screening_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"
#include "../../include/utils/get_temp_file_path.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ScreeningArray *get_all_screenings(const char *screening_source_path) {
    FILE *screening_source = fopen(screening_source_path, "r");
    if (screening_source == NULL) {
        fprintf(stderr, "Error opening screening source file: %s\n", screening_source_path);
        return NULL;
    }

    ScreeningArray *screening_array = (ScreeningArray *) malloc(sizeof(ScreeningArray));
    if (screening_array == NULL) {
        fprintf(stderr, "Memory allocation failed for loading screenings\n");
        fclose(screening_source);
        return NULL;
    }

    screening_array->screenings = NULL;
    screening_array->count = 0;
    screening_array->capacity = 0;

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), screening_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), screening_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_SCREENING_FIELDS];
        parse_csv_line(buffer, fields, N_SCREENING_FIELDS);

        if (screening_array->count >= screening_array->capacity) {
            int new_capacity = (screening_array->capacity == 0) ? 4 : screening_array->capacity * 2;
            Screening *new_screenings =
                    (Screening *) realloc(screening_array->screenings, new_capacity * sizeof(Screening));

            if (new_screenings == NULL) {
                fprintf(stderr, "Memory allocation failed for loading screenings\n");
                free(screening_array->screenings);
                fclose(screening_source);
                free(screening_array);
                return NULL;
            }

            screening_array->screenings = new_screenings;
            screening_array->capacity = new_capacity;
        }

        Screening *current_screening = &screening_array->screenings[screening_array->count];

        current_screening->screening_id = atoi(fields[0]);
        current_screening->movie_id = atoi(fields[1]);
        current_screening->start_time = (time_t) atol(fields[2]);
        current_screening->price = atof(fields[3]);
        current_screening->room_number = atoi(fields[4]);

        screening_array->count++;
    }

    fclose(screening_source);
    return screening_array;
}

bool is_movie_scheduled(int target_movie_id, const char *screening_source_path) {
    FILE *screening_source = fopen(screening_source_path, "r");
    if (screening_source == NULL) {
        fprintf(stderr, "Error opening screening source file: %s\n", screening_source_path);
        return false;
    }

    char buffer[LINE_DATA_BUFFER_SIZE];
    char *delimiter = ",";

    fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source); // Skip the first row which contains the field names

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source)) {
        strtok(buffer, delimiter); // Skip the first column which is the screening ID
        char *movie_id = strtok(NULL, delimiter);

        if (atoi(movie_id) == target_movie_id) {
            fclose(screening_source);
            return true;
        }
    }

    fclose(screening_source);
    return false;
}

Screening *delete_screening_record(int screening_id, const char *screening_source_path) {
    FILE *screening_source = fopen(screening_source_path, "r");
    if (screening_source == NULL) {
        fprintf(stderr, "Failed to open screening source file: %s\n", screening_source_path);
        return NULL;
    }

    char *temp_path = get_temp_file_path(screening_source_path);

    FILE *temp_file = fopen(temp_path, "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file for screening deletion\n");
        fclose(screening_source);
        free(temp_path);
        return NULL;
    }

    Screening *deleted_screening = NULL;

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source); // Read and write header line
    fprintf(temp_file, "%s", buffer);

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source)) {
        char temp_buffer[LINE_DATA_BUFFER_SIZE];
        strcpy(temp_buffer, buffer);

        char *fields[N_SCREENING_FIELDS];
        parse_csv_line(buffer, fields, N_SCREENING_FIELDS);

        if (atoi(fields[0]) != screening_id) {
            fprintf(temp_file, "%s", temp_buffer);
        } else {
            deleted_screening = (Screening *) malloc(sizeof(Screening));
            if (deleted_screening == NULL) {
                fprintf(stderr, "Memory allocation failed for deleted screening record\n");
                fclose(screening_source);
                fclose(temp_file);
                return NULL;
            }
            deleted_screening->screening_id = atoi(fields[0]);
            deleted_screening->movie_id = atoi(fields[1]);
            deleted_screening->start_time = (time_t) atol(fields[2]);
            deleted_screening->price = atof(fields[3]);
            deleted_screening->room_number = atoi(fields[4]);
        }
    }

    fclose(screening_source);
    fclose(temp_file);

    if (remove(screening_source_path) != 0) {
        fprintf(stderr, "Failed to delete original screening source file: %s\n", screening_source_path);
        return NULL;
    }

    if (rename(temp_path, screening_source_path) != 0) {
        fprintf(stderr, "Failed to rename temporary file to original screening source file: %s\n",
                screening_source_path);
        return NULL;
    }

    free(temp_path);

    return deleted_screening;
}
