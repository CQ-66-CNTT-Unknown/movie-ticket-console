#include "../../include/repos/movie_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Get all movies from the system
 * @param movie_source The file source for movies
 * @return `MovieArray` containing all movies
 */
MovieArray get_all_movies(FILE *movie_source) {
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
