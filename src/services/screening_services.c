#include "../../include/services/screening_services.h"
#include "../../include/constanst.h"
#include "../../include/models/movie.h"
#include "../../include/models/screening.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @brief Get all movies from the
 * @param movie_source The file source for movies
 * @return `MovieArray` containing all movies
 */
static MovieArray get_all_movies(FILE *movie_source) {
    MovieArray movie_array;
    movie_array.movies = NULL;
    movie_array.count = 0;
    movie_array.capacity = 0;

    // TODO

    return movie_array;
}

/**
 * @brief Get all screenings from the system
 * @param screening_source The file source for screenings
 * @return `ScreeningArray` containing all screenings
 */
static ScreeningArray get_all_screening(FILE *screening_source) {
    ScreeningArray screening_array;
    screening_array.screenings = NULL;
    screening_array.count = 0;
    screening_array.capacity = 0;

    // TODO

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

    printf("%-10s | %-50s | %-40s | %-20s | %-10s | %-10s\n", "Screening ID", "Movie", "Start time", "Duration (m)",
           "Price ($)", "Room number");
    int table_width = 10 + 50 + 20 + 20 + 10 + 10 + (5 * 3);
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
                printf("%-10d | %-50s | %-40s | %-20d | %-10.2f | %-10d\n", screening_array.screenings[i].screening_id,
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
