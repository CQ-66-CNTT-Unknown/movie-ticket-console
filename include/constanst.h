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

// --- Màu chữ tiêu chuẩn (Regular Text) ---
#define BLACK          "\033[30m"
#define RED            "\033[31m"
#define GREEN          "\033[32m"
#define YELLOW         "\033[33m"
#define BLUE           "\033[34m"
#define PURPLE         "\033[35m"
#define CYAN           "\033[36m"
#define WHITE          "\033[37m"
#define RESET          "\033[0m"