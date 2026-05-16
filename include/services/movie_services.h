#pragma once
#include "../models/movie.h"
/**
 * @brief Display all available movies
 *
 */
void display_all_movies();

/**
 * @brief Checks whether the searched movie name exists within the movie name loaded from file.
 * @param haystack Movie name loaded from file.
 * @param needle Movie name entered by the user.
 * @return 1 if found, 0 otherwise.
 */
int compareFilm(char *haystack, char *needle);

/**
 * @brief Search for a movie by name
 * @param a it will be returned after *a was asigned inside the function.
 * @return A poiter to the movie is choosen after found or "NULL" if the search is failed
 */
Movie search_movie_by_name(Movie *a);

/**
 * @brief Add a new movie to data
 */
void add_movie();

/**
 * @brief Edit an existing movie in data
 * @param movie_id The ID of the movie to be edited
 */
void edit_movie(int movie_id);

/**
 * @brief Delete a movie from data
 * @param movie_id The ID of the movie to be deleted
 */
void delete_movie(int movie_id);
