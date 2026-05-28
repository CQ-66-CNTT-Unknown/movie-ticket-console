#include "../../include/repos/movie_repo.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"
#include "../../include/utils/get_temp_file_path.h"

MovieArray *get_all_movies(const char *movie_source_path) {
    MovieArray *movie_array = (MovieArray *) malloc(sizeof(MovieArray));
    if (movie_array == NULL) {
        fprintf(stderr, "Khong the cap phat bo nho cho MovieArray\n");
        return NULL;
    }

    FILE *movie_source = fopen(movie_source_path, "r");
    if (movie_source == NULL) {
        fprintf(stderr, "Khong the mo tep nguon phim: %s\n", movie_source_path);
        free(movie_array);
        fclose(movie_source);
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
                fprintf(stderr, "Khong the cap phat bo nho cho danh sach phim\n");
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

    fclose(movie_source);
    return movie_array;
}


Movie *delete_movie_record(int movie_id, const char *movie_source_path) {
    FILE *movie_source = fopen(movie_source_path, "r");
    if (movie_source == NULL) {
        fprintf(stderr, "Khong the mo tep nguon phim: %s\n", movie_source_path);
        return NULL;
    }

    char *temp_file_path = get_temp_file_path(movie_source_path);

    FILE *temp_file = fopen(temp_file_path, "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Khong the tao tep tam cho xoa phim\n");
        fclose(movie_source);
        free(temp_file_path);
        return NULL;
    }

    Movie *deleted_movie = NULL;

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source); // Read and write header line
    fprintf(temp_file, "%s", buffer);

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, movie_source)) {
        char temp_buffer[LINE_DATA_BUFFER_SIZE];
        strcpy(temp_buffer, buffer);

        char *fields[N_MOVIES_FIELDS];
        parse_csv_line(buffer, fields, N_MOVIES_FIELDS);

        if (atoi(fields[0]) != movie_id) {
            fprintf(temp_file, "%s", temp_buffer);
        } else {
            deleted_movie = (Movie *) malloc(sizeof(Movie));
            if (deleted_movie == NULL) {
                fprintf(stderr, "Khong the cap phat bo nho cho ban ghi phim da xoa\n");
                fclose(movie_source);
                fclose(temp_file);
                return NULL;
            }
            deleted_movie->movie_id = atoi(fields[0]);
            strncpy(deleted_movie->title, fields[1], sizeof(deleted_movie->title) - 1);
            deleted_movie->title[sizeof(deleted_movie->title) - 1] = '\0';
            deleted_movie->duration = atoi(fields[2]);
        }
    }

    fclose(movie_source);
    fclose(temp_file);

    if (remove(movie_source_path) != 0) {
        fprintf(stderr, "Khong the xoa tep nguon phim goc: %s\n", movie_source_path);
        free(temp_file_path);
        return NULL;
    }

    if (rename(temp_file_path, movie_source_path) != 0) {
        fprintf(stderr, "Khong the doi ten tep tam thanh tep nguon phim goc: %s\n", movie_source_path);
        free(temp_file_path);
        return NULL;
    }

    free(temp_file_path);
    return deleted_movie;
}
