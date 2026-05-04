#pragma once

#include "../models/movie.h"

#include <stdio.h>

/**
 * @brief Get all movies from the system
 * @param movie_source The file source for movies
 * @return `MovieArray` containing all movies
 */
MovieArray get_all_movies(FILE *movie_source);
