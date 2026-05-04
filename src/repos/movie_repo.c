#include "../../include/repos/movie_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MovieArray *get_all_movies(const char *movie_source_path) {
    MovieArray *movie_array = (MovieArray *) malloc(sizeof(MovieArray));
    if (movie_array == NULL) {
        fprintf(stderr, "Memory allocation failed for MovieArray\n");
        return NULL;
    }

    FILE *movie_source = fopen(movie_source_path, "r");
    if (movie_source == NULL) {
        fprintf(stderr, "Failed to open movie source file: %s\n", movie_source_path);
        free(movie_array);
        return NULL;
    }

    movie_array->movies = NULL;
    movie_array->count = 0;
    movie_array->capacity = 0;

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), movie_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), movie_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_MOVIES_FIELDS];
        parse_csv_line(buffer, fields, N_MOVIES_FIELDS);

        if (movie_array->count >= movie_array->capacity) {
            int new_capacity = (movie_array->capacity == 0) ? 4 : movie_array->capacity * 2;
            Movie *new_movies = (Movie *) realloc(movie_array->movies, new_capacity * sizeof(Movie));

            if (new_movies == NULL) {
                fprintf(stderr, "Memory allocation failed for loading movies\n");
                free(movie_array->movies);
                free(movie_array);
                fclose(movie_source);
                return NULL;
            }

            movie_array->movies = new_movies;
            movie_array->capacity = new_capacity;
        }

        Movie *current_movie = &movie_array->movies[movie_array->count];

        current_movie->movie_id = atoi(fields[0]);

        strncpy(current_movie->title, fields[1], sizeof(current_movie->title) - 1);
        current_movie->title[sizeof(current_movie->title) - 1] = '\0';

        current_movie->duration = atoi(fields[2]);

        movie_array->count++;
    }

    return movie_array;
}
