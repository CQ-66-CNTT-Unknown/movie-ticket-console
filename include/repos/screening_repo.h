#pragma once

#include "../models/screening.h"

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Get all screenings from the system
 * @param screening_source_path The file path for screenings
 * @return pointer to ScreeningArray containing all screenings or `NULL` if an error occurs
 */
ScreeningArray *get_all_screenings(const char *screening_source_path);

/**
 * @brief Check if a movie is scheduled for screening
 * @param target_movie_id The ID of the movie to check
 * @param screening_source_path The file path for screenings
 * @return `true` if the movie is scheduled, `false` otherwise
 */
bool is_movie_scheduled(int target_movie_id, const char *screening_source_path);
