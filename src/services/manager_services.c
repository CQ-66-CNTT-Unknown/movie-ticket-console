#include "../../include/services/manager_services.h"
#include "../../include/constanst.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/repos/ticket_repo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief MovieStats structure representing the statistics of a movie
 */
typedef struct MovieStats {
    char title[100]; /**< The title of the movie */
    int tickets_sold; /**< The number of tickets sold for the movie */
    double revenue; /**< The total revenue generated from ticket sales for the movie */
} MovieStats;

static void swap_movie_stats(MovieStats *a, MovieStats *b) {
    MovieStats temp = *a;
    *a = *b;
    *b = temp;
}

void sort_movie_stats_by_revenue(MovieStats *stats, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (stats[j].revenue < stats[j + 1].revenue) {
                swap_movie_stats(&stats[j], &stats[j + 1]);
            }
        }
    }
}

void show_revenue_report() {
    FILE *movie_source = fopen(MOVIE_SOURCE_PATH, "r");
    FILE *screening_source = fopen(SCREENING_SOURCE_PATH, "r");
    FILE *ticket_source = fopen(TICKET_SOURCE_PATH, "r");
    if (movie_source == NULL || screening_source == NULL || ticket_source == NULL) {
        printf("Data is not available!\n");
        if (movie_source != NULL)
            fclose(movie_source);
        if (screening_source != NULL)
            fclose(screening_source);
        if (ticket_source != NULL)
            fclose(ticket_source);
        return;
    }

    MovieArray movies_array = get_all_movies(movie_source);
    ScreeningArray screenings_array = get_all_screenings(screening_source);
    TicketArray tickets_array = get_all_tickets(ticket_source);

    MovieStats *stats = (MovieStats *) malloc(movies_array.count * sizeof(MovieStats));
    if (stats == NULL) {
        fprintf(stderr, "Failed to allocate memory for movie statistics.\n");
        return;
    }

    double total_revenue = 0.0;
    int total_tickets_sold = 0;

    for (int i = 0; i < movies_array.count; i++) {
        strcpy(stats[i].title, movies_array.movies[i].title);
        stats[i].tickets_sold = 0;
        stats[i].revenue = 0.0;

        for (int j = 0; j < screenings_array.count; j++) {
            if (screenings_array.screenings[j].movie_id == movies_array.movies[i].movie_id) {
                for (int k = 0; k < tickets_array.count; k++) {
                    if (tickets_array.tickets[k].screening_id == screenings_array.screenings[j].screening_id) {
                        stats[i].tickets_sold++;
                        stats[i].revenue += screenings_array.screenings[j].price;
                        total_revenue += screenings_array.screenings[j].price;
                        total_tickets_sold++;
                    }
                }
            }
        }
    }

    sort_movie_stats_by_revenue(stats, movies_array.count);

    printf("Revenue Report:\n");
    printf("%-32s | %-16s | %-16s\n", "Movie Title", "Tickets Sold", "Revenue (VND)");

    int divider_length = 32 + 16 + 16 + (2 * 3); // Total width of the table
    for (int i = 0; i < divider_length; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < movies_array.count; i++)
        printf("%-32s | %-16d | %-16.0f\n", stats[i].title, stats[i].tickets_sold, stats[i].revenue);

    for (int i = 0; i < divider_length; i++)
        printf("-");
    printf("\n");

    printf("%-32s | %-16d | %-16.0f\n", "Total", total_tickets_sold, total_revenue);

    free(stats);
    fclose(movie_source);
    fclose(screening_source);
    fclose(ticket_source);
}
