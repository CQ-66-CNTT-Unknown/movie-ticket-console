#include "../../include/services/screening_services.h"
#include "../../include/constanst.h"
#include "../../include/models/movie.h"
#include "../../include/models/screening.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/utils/csv_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    ScreeningArray screening_array = get_all_screenings(screening_source);

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
