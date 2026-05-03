#include "../../include/services/screening_services.h"
#include "../../include/constanst.h"
#include "../../include/models/movie.h"
#include "../../include/models/screening.h"
#include "../../include/utils/csv_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Get all movies from the system
 * @param movie_source The file source for movies
 * @return `MovieArray` containing all movies
 */
static MovieArray get_all_movies(FILE *movie_source) {
    MovieArray movie_array = {NULL, 0, 0};

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), movie_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), movie_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_MOVIES_FIELDS];
        parse_csv_line(buffer, fields, N_MOVIES_FIELDS);

        if (movie_array.count >= movie_array.capacity) {
            int new_capacity = (movie_array.capacity == 0) ? 4 : movie_array.capacity * 2;
            Movie *new_movies = (Movie *) realloc(movie_array.movies, new_capacity * sizeof(Movie));

            if (new_movies == NULL) {
                fprintf(stderr, "Memory allocation failed for movies array\n");
                free(movie_array.movies);
                exit(EXIT_FAILURE);
            }

            movie_array.movies = new_movies;
            movie_array.capacity = new_capacity;
        }

        Movie *current_movie = &movie_array.movies[movie_array.count];

        current_movie->movie_id = atoi(fields[0]);

        strncpy(current_movie->title, fields[1], sizeof(current_movie->title) - 1);
        current_movie->title[sizeof(current_movie->title) - 1] = '\0';

        current_movie->duration = atoi(fields[2]);

        movie_array.count++;
    }


    return movie_array;
}

/**
 * @brief Get all screenings from the system
 * @param screening_source The file source for screenings
 * @return `ScreeningArray` containing all screenings
 */
static ScreeningArray get_all_screening(FILE *screening_source) {
    ScreeningArray screening_array = {NULL, 0, 0};

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), screening_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), screening_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_SCREENING_FIELDS];
        parse_csv_line(buffer, fields, N_SCREENING_FIELDS);

        if (screening_array.count >= screening_array.capacity) {
            int new_capacity = (screening_array.capacity == 0) ? 4 : screening_array.capacity * 2;
            Screening *new_screenings =
                    (Screening *) realloc(screening_array.screenings, new_capacity * sizeof(Screening));

            if (new_screenings == NULL) {
                fprintf(stderr, "Memory allocation failed for screenings array\n");
                free(screening_array.screenings);
                exit(EXIT_FAILURE);
            }

            screening_array.screenings = new_screenings;
            screening_array.capacity = new_capacity;
        }

        Screening *current_screening = &screening_array.screenings[screening_array.count];

        current_screening->screening_id = atoi(fields[0]);
        current_screening->movie_id = atoi(fields[1]);
        current_screening->start_time = (time_t) atol(fields[2]);
        current_screening->price = atof(fields[3]);
        current_screening->room_number = atoi(fields[4]);

        screening_array.count++;
    }

    return screening_array;
}

void view_screenings() {
    FILE *movie_source = fopen(MOVIE_SOURCE_PATH, "r");
    FILE *screening_source = fopen(SCREENING_SOURCE_PATH, "r");
    if (movie_source == NULL || screening_source == NULL) {
        printf("Data is not available!\n");
        if (movie_source != NULL)
            fclose(movie_source);
        if (screening_source != NULL)
            fclose(screening_source);
        return;
    }

    MovieArray movie_array = get_all_movies(movie_source);
    ScreeningArray screening_array = get_all_screening(screening_source);

    printf("%-12s | %-28s | %-28s | %-16s | %-12s | %-8s\n", "Screening ID", "Movie", "Start time", "Duration (m)",
           "Price (VND)", "Room");
    int table_width = 12 + 28 + 28 + 16 + 12 + 8 + (5 * 3);
    for (int i = 0; i < table_width; i++) {
        printf("-");
    }
    printf("\n");

    bool has_screening = false;

    for (int i = 0; i < screening_array.count; i++) {
        for (int j = 0; j < movie_array.count; j++) {

            char *time_str = ctime(&screening_array.screenings[i].start_time);
            time_str[strcspn(time_str, "\n")] = '\0';

            if (screening_array.screenings[i].movie_id == movie_array.movies[j].movie_id) {
                printf("%-12d | %-28s | %-28s | %-16d | %-12.0f | %-8d\n", screening_array.screenings[i].screening_id,
                       movie_array.movies[j].title, time_str, movie_array.movies[j].duration,
                       screening_array.screenings[i].price, screening_array.screenings[i].room_number);
                has_screening = true;
                break;
            }
        }
    }

    if (!has_screening)
        printf("No screenings available.\n");

    fclose(movie_source);
    fclose(screening_source);
}

void show_seat_map(int screening_id) {
    // TODO
}

void create_screening(int movie_id) {
    // TODO
}

void cancel_screening() {
    // TODO
}
