#pragma once

#include "../models/movie.h"

#include <stdio.h>

/**
 * @brief Get all movies from the system
 * @param movie_source_path The file path for movies
 * @return pointer to MovieArray containing all movies or `NULL` if an error occurs
 */
MovieArray *get_all_movies(const char *movie_source_path);
