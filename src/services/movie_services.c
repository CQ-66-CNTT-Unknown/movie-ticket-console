#include "../../include/services/movie_services.h"
#include "../../include/constanst.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/utils/decision_utils.h"
#include "../../include/utils/input_utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Prints the details of a movie to the console
 * @param movie Pointer to the Movie struct whose details are to be printed
 */
static void print_movie_details(const Movie *movie) {
    printf("Thong tin phim:\n");
    printf("ID: %d\n", movie->movie_id);
    printf("Ten phim: %s\n", movie->title);
    printf("Thoi luong: %d phut\n", movie->duration);
}

void display_all_movies() {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);

    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    if (movie_array->count == 0) {
        printf("Khong co phim nao.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    printf("\n===== TAT CA PHIM =====\n");
    printf("%-8s | %-40s | %-10s\n", "ID", "Ten phim", "Thoi luong");
    printf("-");
    for (int i = 0; i < 60; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < movie_array->count; i++) {
        printf("%-8d | %-40s | %-10d phut\n", movie_array->movies[i].movie_id, movie_array->movies[i].title,
               movie_array->movies[i].duration);
    }

    free(movie_array->movies);
    free(movie_array);
}

/**
 * @brief Checks whether the searched movie name exists within the movie name loaded from file.
 * @param haystack Movie name loaded from file.
 * @param needle Movie name entered by the user.
 * @return 1 if found, 0 otherwise.
 */
static int compare_film(char *haystack, char *needle) {
    if (haystack == NULL || needle == NULL) {
        return 0;
    }

    // Convert both strings to lowercase for case-insensitive comparison
    char haystack_lower[MAX_MOVIE_NAME];
    char needle_lower[MAX_MOVIE_NAME];

    for (int i = 0; haystack[i] != '\0'; i++) {
        haystack_lower[i] = tolower(haystack[i]);
    }
    haystack_lower[strlen(haystack)] = '\0';

    for (int i = 0; needle[i] != '\0'; i++) {
        needle_lower[i] = tolower(needle[i]);
    }
    needle_lower[strlen(needle)] = '\0';

    // Check if needle is a substring of haystack
    if (strstr(haystack_lower, needle_lower) != NULL) {
        return 1; // Found
    }
    return 0; // Not found
}

void search_movie_by_name() {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);

    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    printf("\n===== SEARCH MOVIE BY NAME =====\n");
    char movie_input[MAX_MOVIE_NAME];
    printf("Nhap ten phim: ");
    fgets(movie_input, sizeof(movie_input), stdin);
    deleteEnter(movie_input);

    // Find matching movies
    Movie results[100];
    int result_count = 0;

    for (int i = 0; i < movie_array->count && result_count < 100; i++) {
        if (compare_film(movie_array->movies[i].title, movie_input)) {
            results[result_count] = movie_array->movies[i];
            result_count++;
        }
    }

    if (result_count == 0) {
        printf("\nKhong tim thay phim phu hop.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Display results
    printf("\n%-5s | %-40s | %-10s\n", "STT", "Ten phim", "Thoi luong");
    printf("-");
    for (int i = 0; i < 60; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < result_count; i++) {
        printf("%-5d | %-40s | %-10d phut\n", i + 1, results[i].title, results[i].duration);
    }
}

void add_movie() {
    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);

    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    printf("\n===== THEM PHIM MOI =====\n");

    // Get new movie ID (find max ID and add 1)
    int new_movie_id = 1;
    if (movie_array->count > 0) {
        for (int i = 0; i < movie_array->count; i++) {
            if (movie_array->movies[i].movie_id >= new_movie_id) {
                new_movie_id = movie_array->movies[i].movie_id + 1;
            }
        }
    }

    // Get movie title from user
    char title[100];
    printf("Nhap ten phim: ");
    fgets(title, sizeof(title), stdin);
    deleteEnter(title);

    // Validate title is not empty
    if (strlen(title) == 0) {
        printf("Ten phim khong the de trong!\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Get movie duration from user
    int duration = input_id("Nhap thoi luong phim (phut): ", 50);
    if (duration <= 0) {
        printf("Thoi luong phim phai lon hon 0!\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Write to CSV file
    FILE *movie_file = fopen(MOVIE_SOURCE_PATH, "a");
    if (movie_file == NULL) {
        fprintf(stderr, "Failed to open movie file for writing.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    fprintf(movie_file, "%d,%s,%d\n", new_movie_id, title, duration);
    fclose(movie_file);

    printf("\nPhim da duoc them thanh cong!\n");
    printf("ID phim: %d\n", new_movie_id);
    printf("Ten phim: %s\n", title);
    printf("Thoi luong: %d phut\n", duration);

    free(movie_array->movies);
    free(movie_array);
}

void edit_movie() {
    int movie_id = input_id("Nhap ID cua phim ban muon chinh sua: ", 50);

    if (movie_id == -1) {
        printf("The ID is invalid!\n");
        return;
    }

    MovieArray *movie_array = get_all_movies(MOVIE_SOURCE_PATH);

    if (movie_array == NULL) {
        fprintf(stderr, "Failed to load movie data.\n");
        return;
    }

    // Find the movie
    Movie *found_movie = NULL;
    
    for (int i = 0; i < movie_array->count; i++) {
        if (movie_array->movies[i].movie_id == movie_id) {
            found_movie = &movie_array->movies[i];
            break;
        }
    }

    if (found_movie == NULL) {
        printf("Movie with ID %d not found!\n", movie_id);
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    printf("\n===== EDIT MOVIE =====\n");
    print_movie_details(found_movie);

    printf("\nBan muon chinh sua thong tin nao?\n");
    printf("1. Ten phim\n");
    printf("2. Thoi luong\n");
    printf("0. Huy bo\n");
    printf("Nhap lua chon cua ban: ");

    int choice = -1;
    inputNumber(&choice);

    if (choice == 0) {
        printf("Chinh sua da bi huy.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    if (choice == 1) {
        printf("Nhap ten phim moi: ");
        char new_title[100];
        fgets(new_title, sizeof(new_title), stdin);
        deleteEnter(new_title);

        if (strlen(new_title) == 0) {
            printf("Ten phim khong the de trong!\n");
            free(movie_array->movies);
            free(movie_array);
            return;
        }

        strncpy(found_movie->title, new_title, sizeof(found_movie->title) - 1);
        found_movie->title[sizeof(found_movie->title) - 1] = '\0';
        printf("Ten phim da duoc cap nhat!\n");
    } else if (choice == 2) {
        int new_duration = input_id("Nhap thoi luong moi (phut): ", 50);
        if (new_duration <= 0) {
            printf("Thoi luong phai lon hon 0!\n");
            free(movie_array->movies);
            free(movie_array);
            return;
        }
        found_movie->duration = new_duration;
        printf("Thoi luong da duoc cap nhat!\n");
    } else {
        printf("Lua chon khong hop le!\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Write all movies back to file
    FILE *movie_file = fopen(MOVIE_SOURCE_PATH, "w");
    if (movie_file == NULL) {
        fprintf(stderr, "Failed to open movie file for writing.\n");
        free(movie_array->movies);
        free(movie_array);
        return;
    }

    // Write header
    fprintf(movie_file, "movie_id,title,duration\n");

    // Write all movies
    for (int i = 0; i < movie_array->count; i++) {
        fprintf(movie_file, "%d,%s,%d\n", movie_array->movies[i].movie_id, movie_array->movies[i].title,
                movie_array->movies[i].duration);
    }

    fclose(movie_file);
    printf("\nMovie updated successfully!\n");
    print_movie_details(found_movie);

    free(movie_array->movies);
    free(movie_array);
}

void delete_movie() {
    int movie_id = input_id("Nhap ID cua phim ban muon xoa: ", MAX_MOVIE_NAME);

    if (movie_id == -1) {
        printf("ID phim khong hop le.\n");
        return;
    }

    bool is_scheduled = is_movie_scheduled(movie_id, SCREENING_SOURCE_PATH);

    if (is_scheduled) {
        printf("Phim da duoc dat lich chieu, vui long huy tat ca cac lich chieu lien quan truoc khi xoa phim!\n");
        return;
    }

    if (!is_decision_yes("Xoa phim voi ID")) {
        return;
    }

    Movie *deleted_movie = delete_movie_record(movie_id, MOVIE_SOURCE_PATH);
    if (deleted_movie != NULL) {
        printf("Phim da duoc xoa thanh cong!\n");
        print_movie_details(deleted_movie);
        free(deleted_movie);
    } else {
        printf("Khong the xoa phim.\n");
    }
}
