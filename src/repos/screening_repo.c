#include "../../include/repos/screening_repo.h"

#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"

#include <ctype.h>
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
