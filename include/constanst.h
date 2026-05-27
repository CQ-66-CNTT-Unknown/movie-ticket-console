#pragma once

#define MAX_MOVIE_NAME 500
#define LINE_DATA_MAX_SIZE 1024
#define LINE_DATA_BUFFER_SIZE (LINE_DATA_MAX_SIZE + 2) // +2 for newline and null terminator

#define MOVIE_SOURCE_PATH "data/movie.csv"
#define SCREENING_SOURCE_PATH "data/screening.csv"
#define TICKET_SOURCE_PATH "data/ticket.csv"
#define USER_SOURCE_PATH "data/user.csv"

#define N_MOVIES_FIELDS 3
#define N_SCREENING_FIELDS 5
#define N_TICKETS_FIELDS 4
#define N_USER_FIELDS 4

#define TEMP_FILE_NAME "temp_file.csv"

#define DIR_LENGTH 256

/**
 * ANSI escape codes for text formatting and colors
 */
#define RESET "\033[0m"
#define BOLD_ON "\033[1m" // Enable bold text
#define INVERT "\033[7m" // Enable inverted colors (background becomes foreground and vice versa)

/**
 * ANSI color codes
 */
#define BLACK "\033[30m"
#define BOLD_RED "\033[31m"
#define BOLD_GREEN "\033[32m"
#define BOLD_YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD_MAGENTA "\033[35m"
#define BOLD_CYAN "\033[36m"
#define BOLD_WHITE "\033[37m"
#define DEFAULT "\033[39m" // Return to default text color of the terminal

/**
 * Bold / Bright colors
 */
#define BOLD_BLACK "\033[1;30m"
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_WHITE "\033[1;37m"

/**
 * Background colors
 */
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"
#define BG_DEFAULT "\033[49m" // Return to default background color of the terminal

/**
 * High intensity colors
 */
#define DARK_GRAY "\033[0;90m"
