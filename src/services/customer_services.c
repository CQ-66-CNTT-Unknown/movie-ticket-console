#include "../../include/services/customer_services.h"
#include "../../include/constanst.h"
#include "../../include/repos/ticket_repo.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/repos/screening_repo.h"
#include "../../include/utils/input_utils.h"
#include "../../include/utils/decision_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// Search for movie title by movie_id
static void get_movie_title(int movie_id, char *out_title, int out_size) {
    MovieArray *movies = get_all_movies(MOVIE_SOURCE_PATH);
    strncpy(out_title, "Unknown", out_size - 1);
    if (movies == NULL) return;
    for (int i = 0; i < movies->count; i++) {
        if (movies->movies[i].movie_id == movie_id) {
            strncpy(out_title, movies->movies[i].title, out_size - 1);
            out_title[out_size - 1] = '\0';
            break;
        }
    }
    if (movies->movies) free(movies->movies);
    free(movies);
}

// Display the list of showtimes
static void display_screenings(ScreeningArray *screenings) {
    printf("\n+------+----------------------------+---------------------+----------+------+\n");
    printf("| %-4s | %-26s | %-19s | %-8s | %-4s |\n",
           "ID", "Phim", "Gio chieu", "Gia ve", "Phong");
    printf("+------+----------------------------+---------------------+----------+------+\n");

    for (int i = 0; i < screenings->count; i++) {
        Screening *s = &screenings->screenings[i];

        // Format time using Unix timestamp
        time_t t = (time_t)s->start_time;
        struct tm *tm_info = localtime(&t);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%d/%m/%Y %H:%M", tm_info);

        // Get the movie name
        char title[100];
        get_movie_title(s->movie_id, title, sizeof(title));
        printf("| %-4d | %-26s | %-19s | %8.0f | %-4d |\n",
               s->screening_id, title, time_str, s->price, s->room_number);
    }
    printf("+------+----------------------------+---------------------+----------+------+\n");
}

// Display seating chart
static void display_seat_map(int screening_id) {
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    char seats[5][5];

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            seats[i][j] = 'O';

    // Mark X as reserved seat
    if (tickets != NULL) {
        for (int i = 0; i < tickets->count; i++) {
            if (tickets->tickets[i].screening_id == screening_id) {
                char row = tickets->tickets[i].seat_code[0];
                char col = tickets->tickets[i].seat_code[1];
                if (row >= 'A' && row <= 'E' && col >= '1' && col <= '5') {
                    seats[row - 'A'][col - '1'] = 'X';
                }
            }
        }
        if (tickets->tickets) free(tickets->tickets);
        free(tickets);
    }

    printf("\n========= SO DO GHE NGOI (Suat chieu %d) =========\n", screening_id);
    printf("                  [ MAN HINH ]\n\n");

    printf("      ");
    for (int j = 1; j <= 5; j++) printf("  %d   ", j);
    printf("\n");

    // I
    for (int i = 0; i < 5; i++) {
        printf("      ");
        for (int j = 0; j < 5; j++) printf("+-----");
        printf("+\n");

        printf("  %c   ", 'A' + i);
        for (int j = 0; j < 5; j++) {
            if (seats[i][j] == 'X')
                printf("|[XXX]");  // reserved seats
            else
                printf("|[   ]");  // empty seats
        }
        printf("|\n");
    }

    printf("      ");
    for (int j = 0; j < 5; j++) printf("+-----");
    printf("+\n");

    printf("\n  [   ] = Trong      [XXX] = Da dat\n");
    printf("===================================================\n");
}

// create a new ticket_id
static int generate_new_ticket_id() {
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    int max_id = 0;
    if (tickets != NULL) {
        for (int i = 0; i < tickets->count; i++) {
            if (tickets->tickets[i].ticket_id > max_id)
                max_id = tickets->tickets[i].ticket_id;
        }
        if (tickets->tickets) free(tickets->tickets);
        free(tickets);
    }
    return max_id + 1;
}

// the book ticket
void book_ticket(int screening_id, char seat_code) {
    int actual_screening_id;
    char actual_seat_code[10];
    int current_user_id = 1;

    Screening *selected_screening = NULL;
    ScreeningArray *screenings = get_all_screenings(SCREENING_SOURCE_PATH);

    if (screenings == NULL || screenings->count == 0) {
        printf("Hien tai khong co suat chieu nao.\n");
        if (screenings) free(screenings);
        return;
    }

    // Display the list of showtimes before asking for the ID
    display_screenings(screenings);

    while (true) {
        printf("\nNhap ID suat chieu muon mua (0 de thoat): ");
        if (scanf("%d", &actual_screening_id) != 1) {
            while (getchar() != '\n');
            printf("Vui long nhap so hop le!\n");
            continue;
        }
        while (getchar() != '\n');

        if (actual_screening_id == 0) {
            free(screenings->screenings);
            free(screenings);
            return;
        }

        bool found = false;
        for (int i = 0; i < screenings->count; i++) {
            if (screenings->screenings[i].screening_id == actual_screening_id) {
                selected_screening = &screenings->screenings[i];
                found = true;
                break;
            }
        }

        if (!found)
            printf("Loi: ID suat chieu khong ton tai. Vui long thu lai.\n");
        else
            break;
    }

    // Display the seating chart in a grid format
    display_seat_map(actual_screening_id);

    while (true) {
        printf("Nhap ma ghe (vi du: A1, B3) hoac Q de thoat: ");
        scanf("%s", actual_seat_code);
        while (getchar() != '\n');

        if (strcmp(actual_seat_code, "Q") == 0 || strcmp(actual_seat_code, "q") == 0) {
            free(screenings->screenings);
            free(screenings);
            return;
        }

        actual_seat_code[0] = toupper(actual_seat_code[0]);

        if (strlen(actual_seat_code) != 2 ||
            actual_seat_code[0] < 'A' || actual_seat_code[0] > 'E' ||
            actual_seat_code[1] < '1' || actual_seat_code[1] > '5') {
            printf("Loi: Ma ghe khong hop le (phai tu A1 den E5). Nhap lai.\n");
            continue;
        }

        // Check if the seats have been reserved
        bool seat_taken = false;
        TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
        if (tickets != NULL) {
            for (int i = 0; i < tickets->count; i++) {
                if (tickets->tickets[i].screening_id == actual_screening_id &&
                    strcmp(tickets->tickets[i].seat_code, actual_seat_code) == 0) {
                    seat_taken = true;
                    break;
                }
            }
            if (tickets->tickets) free(tickets->tickets);
            free(tickets);
        }

        if (seat_taken) {
            printf("Loi: Ghe %s da co nguoi dat. Vui long chon ghe khac.\n", actual_seat_code);
            continue;
        }

        break;
    }

    // Confirm information
    char title[100];
    get_movie_title(selected_screening->movie_id, title, sizeof(title));

    printf("\n--- XAC NHAN THONG TIN DAT VE ---\n");
    printf("Phim        : %s\n", title);
    printf("ID Suat chieu: %d\n", actual_screening_id);
    printf("Ma ghe      : %s\n", actual_seat_code);
    printf("Gia ve      : %.0f VND\n", selected_screening->price);
    printf("---------------------------------\n");

    if (!is_decision_yes("Xac nhan thanh toan va mua ve")) {
        printf("Da huy qua trinh mua ve.\n");
        free(screenings->screenings);
        free(screenings);
        return;
    }

    // Save tickets
    int new_ticket_id = generate_new_ticket_id();
    FILE *ticket_file = fopen(TICKET_SOURCE_PATH, "a");
    if (ticket_file == NULL) {
        printf("Loi: Khong the ghi ve vao tep du lieu!\n");
        free(screenings->screenings);
        free(screenings);
        return;
    }
    fprintf(ticket_file, "%d,%d,%d,%s\n",
            new_ticket_id, actual_screening_id, current_user_id, actual_seat_code);
    fclose(ticket_file);

    printf("\nMua ve thanh cong! ID ve cua ban la: %d\n", new_ticket_id);

    free(screenings->screenings);
    free(screenings);
}

void cancel_ticket(int ticket_id) {
    printf("Chuc nang huy ve dang phat trien.\n");
}

void view_purchase_history() {
    printf("Chuc nang xem lich su dang phat trien.\n");
}