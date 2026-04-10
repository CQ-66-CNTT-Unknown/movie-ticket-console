#pragma once

/**
 *@brief Movie structure representing a movie in the system
 */
typedef struct Movie {
    int movie_id; /**< The unique identifier for the movie */
    char title[100]; /**< The title of the movie */
    int duration; /**< The duration of the movie in minutes */
} Movie;
