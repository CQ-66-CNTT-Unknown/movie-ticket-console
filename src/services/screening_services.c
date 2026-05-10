#include "../../include/services/screening_services.h"
#include "../../include/constanst.h"
#include "../../include/models/movie.h"
#include "../../include/models/screening.h"
#include "../../include/models/ticket.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/repos/ticket_repo.h"
#include "../../include/utils/csv_utils.h"
#include "../../include/utils/input_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Print the header of the screening table
 */
static void print_screening_table_header() {
    printf("%-12s | %-28s | %-28s | %-16s | %-12s | %-8s\n", "Screening ID", "Movie", "Start time", "Duration (m)",
           "Price (VND)", "Room");
    int table_width = 12 + 28 + 28 + 16 + 12 + 8 + (5 * 3);
    for (int i = 0; i < table_width; i++)
        printf("-");
    printf("\n");
}

/**
 * @brief Find a movie by its ID
 * @param movie_array The array of movies
 * @param movie_id The ID of the movie to find
 * @return A pointer to the found movie, or `NULL` if not found
 */
static Movie *find_movie_by_id(const MovieArray *movie_array, int movie_id) {
    for (int i = 0; i < movie_array->count; i++) {
        if (movie_array->movies[i].movie_id == movie_id)
            return &movie_array->movies[i];
    }
    return NULL;
}

/**
 * @brief Print information about a screening
 * @param screening The screening to print information for
 * @param movie The movie associated with the screening
 */
static void print_screening_info(Screening *screening, Movie *movie) {
    char time_str[26];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&screening->start_time));

    printf("%-12d | %-28s | %-28s | %-16d | %-12.0f | %-8d\n", screening->screening_id, movie->title, time_str,
           movie->duration, screening->price, screening->room_number);
}

void view_screenings() {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);
    ScreeningArray *screening_array = get_all_screenings(SCREENING_SOURCE_PATH);

    if (screening_array == NULL) {
        fprintf(stderr, "Failed to load screening data.\n");
        return;
    }

    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        free(screening_array);
        return;
    }

    if (screening_array->count == 0) {
        fprintf(stderr, "No screenings available.\n");
        free(screening_array);
        free(movie_array);
        return;
    }

    print_screening_table_header();

    for (int i = 0; i < screening_array->count; i++) {
        Screening *current_screening = &screening_array->screenings[i];
        Movie *associated_movie = find_movie_by_id(movie_array, current_screening->movie_id);

        if (associated_movie != NULL) {
            print_screening_info(current_screening, associated_movie);
        } else {
            fprintf(stderr, "Warning: Movie with ID %d not found for screening ID %d\n", current_screening->movie_id,
                    current_screening->screening_id);
        }
    }

    free(screening_array);
    free(movie_array);
}

void show_seat_map(int screening_id) {
    TicketArray *ticket_array = get_all_tickets(TICKET_SOURCE_PATH);
    
    if (ticket_array == NULL) {
        fprintf(stderr, "Failed to load ticket data.\n");
        return;
    }

    printf("\n===== SEAT MAP FOR SCREENING %d =====\n", screening_id);
    printf("Legend: [.] = Available  [X] = Booked\n\n");

    // Define room layout: 10 rows (A-J), 10 seats per row (1-10)
    int rows = 10;
    int seats_per_row = 10;
    
    // Create a seat matrix to track booked seats
    bool booked[rows][seats_per_row];
    memset(booked, false, sizeof(booked));

    // Mark booked seats
    for (int i = 0; i < ticket_array->count; i++) {
        if (ticket_array->tickets[i].screening_id == screening_id) {
            char *seat_code = ticket_array->tickets[i].seat_code;
            int row = seat_code[0] - 'A'; // Convert A-J to 0-9
            int seat = atoi(seat_code + 1) - 1; // Convert 1-10 to 0-9

            if (row >= 0 && row < rows && seat >= 0 && seat < seats_per_row) {
                booked[row][seat] = true;
            }
        }
    }

    // Display the seat map
    printf("    ");
    for (int j = 1; j <= seats_per_row; j++) {
        printf("%2d  ", j);
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%c : ", 'A' + i);
        for (int j = 0; j < seats_per_row; j++) {
            if (booked[i][j]) {
                printf("[X]  ");
            } else {
                printf("[.]  ");
            }
        }
        printf("\n");
    }

    free(ticket_array->tickets);
    free(ticket_array);
}

void create_screening(int movie_id) {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);
    
    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    // Find the movie
    Movie *found_movie = NULL;
    for (int i = 0; i < movie_array->count; i++) {
        if (movie_array->movies[i].movie_id == movie_id) {
            found_movie = &movie_array->movies[i];
            break;
        }
    }

    if (found_movie == NULL) {
        fprintf(stderr, "Movie with ID %d not found.\n", movie_id);
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    printf("\n===== CREATE SCREENING FOR MOVIE: %s =====\n", found_movie->title);

    // Get new screening ID (find max ID and add 1)
    ScreeningArray *screening_array = get_all_screenings(SCREENING_SOURCE_PATH);
    int new_screening_id = 1;
    
    if (screening_array != NULL && screening_array->count > 0) {
        for (int i = 0; i < screening_array->count; i++) {
            if (screening_array->screenings[i].screening_id >= new_screening_id) {
                new_screening_id = screening_array->screenings[i].screening_id + 1;
            }
        }
        free(screening_array->screenings);
        free(screening_array);
    }

    // Input screening details
    printf("Enter screening date and time (YYYY-MM-DD HH:MM format): ");
    char datetime_str[50];
    fgets(datetime_str, sizeof(datetime_str), stdin);
    datetime_str[strcspn(datetime_str, "\n")] = '\0';

    struct tm time_struct = {0};
    sscanf(datetime_str, "%d-%d-%d %d:%d", 
           &time_struct.tm_year, &time_struct.tm_mon, &time_struct.tm_mday,
           &time_struct.tm_hour, &time_struct.tm_min);
    
    time_struct.tm_year -= 1900; // tm_year is years since 1900
    time_struct.tm_mon -= 1;     // tm_mon is 0-11

    time_t start_time = mktime(&time_struct);
    if (start_time == (time_t)-1) {
        fprintf(stderr, "Invalid date/time format.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    double price = input_id("Enter ticket price (VND): ", 50);
    if (price < 0) {
        fprintf(stderr, "Invalid price.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    int room_number = input_id("Enter room number: ", 50);
    if (room_number < 0) {
        fprintf(stderr, "Invalid room number.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Write to CSV file
    FILE *screening_file = fopen(SCREENING_SOURCE_PATH, "a");
    if (screening_file == NULL) {
        fprintf(stderr, "Failed to open screening file for writing.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    fprintf(screening_file, "%d,%d,%ld,%.0f,%d\n", new_screening_id, movie_id, start_time, price, room_number);
    fclose(screening_file);

    printf("\nScreening created successfully!\n");
    printf("Screening ID: %d\n", new_screening_id);
    printf("Movie: %s\n", found_movie->title);
    printf("Start time: %s\n", ctime(&start_time));
    printf("Price: %.0f VND\n", price);
    printf("Room: %d\n", room_number);

    free(movie_array->movies);
    free(movie_array);
}

void cancel_screening() {
    // TODO
}
