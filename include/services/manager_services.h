#pragma once

/**
 * Add a new movie to data
 *
 */
void add_movie();

/**
 * Edit an existing movie in data
 *@param movie_id The ID of the movie to be edited
 */
void edit_movie(int movie_id);

/**
 * Delete a movie from data
 *@param movie_id The ID of the movie to be deleted
 */
void delete_movie(int movie_id);

/*
 * View all screenings
 */
void view_screenings();

/**
 * Create a new screening
 *@param movie_id The ID of the movie for which the screening is to be created
 */
void create_screening(int movie_id);

/**
 * Cancel an existing screening
 *@param screening_id The ID of the screening to be canceled
 */
void cancel_screening();

/**
 * Show revenue report
 *
 */
void show_revenue_report();
