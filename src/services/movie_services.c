#include "../../include/services/movie_services.h"
#include "../../include/constanst.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_all_movies() {
    // TODO
}

void search_movie_by_name(const char *name) {
    // TODO
}

void add_movie() {
    // TODO
}

void edit_movie(int movie_id) {
    // TODO
}

/**
 * @brief Find the position of a movie in the movie source file
 * @param target_movie_id The ID of the movie to find
 * @param movie_source The file pointer to the movie source file
 * @return The line number of the movie if found, or `-1` if not found
 */
static int find_movie_position(int target_movie_id, FILE *movie_source) {
    char buffer[LINE_DATA_BUFFER_SIZE];
    char *delimiter = ",";

    fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source); // Skip the row of field names which is the first row
    int current_line = 2;
    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source)) {
        char *movie_id = strtok(buffer, delimiter);
        if (atoi(movie_id) == target_movie_id)
            return current_line;

        current_line++;
    }

    return -1;
}

/**
 * @brief Check if a movie is scheduled for screening
 * @param target_movie_id The ID of the movie to check
 * @param screening_source The file pointer to the screening source file
 * @return `true` if the movie is scheduled, `false` otherwise
 */
static bool is_movie_scheduled(int target_movie_id, FILE *screening_source) {
    char buffer[LINE_DATA_BUFFER_SIZE];
    char *delimiter = ",";

    fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source); // Skip the first row which contains the field names

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, screening_source)) {
        strtok(buffer, delimiter); // Skip the first column which is the screening ID
        char *movie_id = strtok(NULL, delimiter);

        if (atoi(movie_id) == target_movie_id)
            return true;
    }

    return false;
}

void delete_movie(int movie_id) {
    if (movie_id == -1) {
        printf("The ID is invalid!\n");
        return;
    }

    FILE *movie_source = fopen("data/movie.csv", "r");

    if (movie_source == NULL) {
        printf("Cannot find the movie source\n");
        return;
    }

    int movie_row_position = find_movie_position(movie_id, movie_source);
    if (movie_row_position == -1) {
        printf("The movie ID is not found!\n");
        fclose(movie_source);
        return;
    }

    bool is_scheduled;

    FILE *screening_source = fopen("data/screening.csv", "r");
    if (screening_source == NULL)
        is_scheduled = true;
    else
        is_scheduled = is_movie_scheduled(movie_id, screening_source);

    if (screening_source != NULL)
        fclose(screening_source);

    if (is_scheduled) {
        printf("The movie is scheduled, please cancel related screenings before delete the movie!\n");
        fclose(movie_source);
        return;
    }

    printf("Delete movie with ID %d [y/N(Default)]: ", movie_id);
    char decision;
    scanf("%c", &decision);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clear buffer
    if (tolower(decision) != 'y') {
        fclose(movie_source);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    char buffer[LINE_DATA_BUFFER_SIZE];

    rewind(movie_source);

    // Write the row (1st row) of the field names
    fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source);
    fputs(buffer, temp);
    int current_line = 2;

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source) != NULL) {
        if (current_line != movie_row_position)
            fputs(buffer, temp);
        current_line++;
    }

    fclose(movie_source);
    fclose(temp);

    remove("data/movie.csv");
    rename("temp.txt", "data/movie.csv");
}
