#include "../../include/services/movie_services.h"
#include "../../include/constanst.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/utils/decision_utils.h"
#include "../../include/utils/input_utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_all_movies() {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);
    
    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    if (movie_array->count == 0) {
        printf("No movies available.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    printf("\n===== ALL MOVIES =====\n");
    printf("%-8s | %-40s | %-10s\n", "ID", "Title", "Duration");
    printf("-");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    for (int i = 0; i < movie_array->count; i++) {
        printf("%-8d | %-40s | %-10d minutes\n", 
               movie_array->movies[i].movie_id,
               movie_array->movies[i].title,
               movie_array->movies[i].duration);
    }

    free(movie_array->movies);
    free(movie_array);
}

int compareFilm(char *haystack, char *needle) {
    if (haystack == NULL || needle == NULL) {
        return 0;
    }

    // Convert both strings to lowercase for case-insensitive comparison
    char haystack_lower[MAX_MOVIE_NAME];
    char needle_lower[MAX_MOVIE_NAME];
    
    for (int i = 0; haystack[i] != '\0'; i++) {
        haystack_lower[i] = tolower(haystack[i]);
    }
    haystack_lower[strlen(haystack)] = '\0';
    
    for (int i = 0; needle[i] != '\0'; i++) {
        needle_lower[i] = tolower(needle[i]);
    }
    needle_lower[strlen(needle)] = '\0';

    // Check if needle is a substring of haystack
    if (strstr(haystack_lower, needle_lower) != NULL) {
        return 1; // Found
    }
    return 0; // Not found
}

Movie search_movie_by_name(Movie *a) {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);
    
    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return (Movie){-1, "", 0};
    }

    printf("\n===== SEARCH MOVIE BY NAME =====\n");
    char movie_input[MAX_MOVIE_NAME];
    printf("Enter the movie name you want to find: ");
    fgets(movie_input, sizeof(movie_input), stdin);
    deleteEnter(movie_input);

    // Find matching movies
    Movie results[100];
    int result_count = 0;

    for (int i = 0; i < movie_array->count && result_count < 100; i++) {
        if (compareFilm(movie_array->movies[i].title, movie_input)) {
            results[result_count] = movie_array->movies[i];
            result_count++;
        }
    }

    if (result_count == 0) {
        printf("\nNo matching movies found.\n");
        free(movie_array->movies);
        free(movie_array);
        return (Movie){-1, "", 0};
    }

    // Display results
    printf("\n%-5s | %-40s | %-10s\n", "No.", "Title", "Duration");
    printf("-");
    for (int i = 0; i < 60; i++) printf("-");
    printf("\n");

    for (int i = 0; i < result_count; i++) {
        printf("%-5d | %-40s | %-10d minutes\n", 
               i + 1, 
               results[i].title,
               results[i].duration);
    }

    // Get user choice
    int choice = -1;
    printf("\nEnter the corresponding number to select a movie (or 0 to cancel): ");
    inputNumber(&choice);

    if (choice >= 1 && choice <= result_count) {
        *a = results[choice - 1];
        printf("\nSelected: %s\n", a->title);
        free(movie_array->movies);
        free(movie_array);
        return *a;
    } else {
        printf("\nOperation cancelled.\n");
        free(movie_array->movies);
        free(movie_array);
        return (Movie){-1, "", 0};
    }
}

void add_movie() {
    // TODO
}

void edit_movie(int movie_id) {
    // TODO
}

/**
 * @brief Prints the details of a movie to the console
 * @param movie Pointer to the Movie struct whose details are to be printed
 */
static void print_movie_details(const Movie *movie) {
    printf("Movie Details:\n");
    printf("ID: %d\n", movie->movie_id);
    printf("Title: %s\n", movie->title);
    printf("Duration: %d minutes\n", movie->duration);
}

void delete_movie(int movie_id) {
    if (movie_id == -1) {
        printf("The ID is invalid!\n");
        return;
    }

    bool is_scheduled = is_movie_scheduled(movie_id, SCREENING_SOURCE_PATH);

    if (is_scheduled) {
        printf("The movie is scheduled, please cancel all related screenings before deleting the movie!\n");
        return;
    }

    if (!is_decision_yes("Delete movie with ID"))
        return;

    Movie *deleted_movie = delete_movie_record(movie_id, MOVIE_SOURCE_PATH);
    if (deleted_movie != NULL) {
        printf("Movie deleted successfully!\n");
        print_movie_details(deleted_movie);
        free(deleted_movie);
    } else {
        printf("Failed to delete the movie.\n");
    }
}