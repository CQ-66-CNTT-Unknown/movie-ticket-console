#pragma once

#include "../models/screening.h"

#include <stdio.h>

/**
 * @brief Get all screenings from the system
 * @param screening_source The file source for screenings
 * @return `ScreeningArray` containing all screenings
 */
ScreeningArray get_all_screenings(FILE *screening_source);
