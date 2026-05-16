#pragma once
#include "../models/screening.h"
/**
 * @brief View all screenings
 */
void view_screenings();

/**
 * @brief choose a screening
 * @param a it will be returned after *a is asigned inside the function
 */
Screening choose_screening(Screening *a);

/**
 * @brief Show the seat map for a specific screening
 */
void show_seat_map();

/**
 * @brief Create a new screening
 */
void create_screening();

/**
 * @brief Cancel an existing screening
 */
void cancel_screening();
