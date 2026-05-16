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

#define COLOR_RED     0xFF0000
#define COLOR_GREEN   0x00FF00
#define COLOR_BLUE    0x0000FF
#define COLOR_WHITE   0xFFFFFF
#define COLOR_BLACK   0x000000
