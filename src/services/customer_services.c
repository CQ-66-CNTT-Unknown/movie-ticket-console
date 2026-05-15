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

    for (int i = 0; i < 5; i++) {
        printf("      ");
        for (int j = 0; j < 5; j++) printf("+-----");
        printf("+\n");

        printf("  %c   ", 'A' + i);
        for (int j = 0; j < 5; j++) {
            if (seats[i][j] == 'X')
                printf("|[XXX]");  // reserved seats
            else {
                if (i == 4) // Row E - Double Seats
                    printf("|[< >]"); 
                else
                    printf("|[   ]");  // empty seats
            }
        }
        printf("|\n");
    }

    printf("      ");
    for (int j = 0; j < 5; j++) printf("+-----");
    printf("+\n");

    printf("\n  [   ] = Trong    [XXX] = Da dat    [< >] = Ghe doi (Hang E - Gia x2)\n");
    printf("===================================================\n");
}

// Customer selection table
static float get_customer_discount(char* customer_type_str) {
    int choice;
    while(true) {
        printf("\n+================================================+\n");
        printf("|             CHON DOI TUONG KHACH HANG          |\n");
        printf("+================================================+\n");
        printf("| 1. Hoc sinh - Sinh vien (Giam 10%% gia ve)      |\n");
        printf("| 2. Nguoi lon (Giu nguyen gia ve)               |\n");
        printf("| 3. Nguoi lon tuoi (Giam 8%% gia ve)             |\n");
        printf("+================================================+\n");
        printf("Nhap lua chon cua ban (1-3): ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Lua chon khong hop le! Vui long nhap so.\n");
            continue;
        }
        while(getchar() != '\n');

        if (choice == 1) {
            strcpy(customer_type_str, "Hoc sinh - Sinh vien (-10%)");
            return 0.90f;
        } else if (choice == 2) {
            strcpy(customer_type_str, "Nguoi lon (Gia goc)");
            return 1.0f;
        } else if (choice == 3) {
            strcpy(customer_type_str, "Nguoi lon tuoi (-8%)");
            return 0.92f;
        } else {
            printf("Vui long chon tu 1 den 3!\n");
        }
    }
}

// Optional services selection table
static float get_services_fee() {
    int choice;
    float total_service = 0;
    while(true) {
        printf("\n+================================================+\n");
        printf("|                 DICH VU DI KEM                 |\n");
        printf("+================================================+\n");
        printf("| 1. Nuoc ngot (+ 10,000 VND)                    |\n");
        printf("| 2. Mi tron   (+ 25,000 VND)                    |\n");
        printf("| 3. Bap rang  (+ 30,000 VND)                    |\n");
        printf("| 0. Khong chon them / Xong (Bo qua)             |\n");
        printf("+================================================+\n");
        printf("Tam tinh tien dich vu: %.0f VND\n", total_service);
        printf("Nhap lua chon cua ban (0-3): ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Lua chon khong hop le!\n");
            continue;
        }
        while(getchar() != '\n');

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            total_service += 10000;
            printf("-> Da them 1 Nuoc ngot vao gio hang.\n");
        } else if (choice == 2) {
            total_service += 25000;
            printf("-> Da them 1 Mi tron vao gio hang.\n");
        } else if (choice == 3) {
            total_service += 30000;
            printf("-> Da them 1 Bap rang vao gio hang.\n");
        } else {
            printf("Vui long chon tu 0 den 3!\n");
        }
    }
    return total_service;
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

    // 1. Select screening time
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

    // select target customers
    char customer_type_str[50];
    float discount_rate = get_customer_discount(customer_type_str);

    // choose a seat
    display_seat_map(actual_screening_id);

    while (true) {
        printf("Nhap ma ghe (vi du: A1, E3) hoac Q de thoat: ");
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

    // 4. CHỌN DỊCH VỤ ĐI KÈM
    float service_fee = get_services_fee();

    // 5. TÍNH TOÁN TỔNG TIỀN VÀ XÁC NHẬN
    int seat_multiplier = (actual_seat_code[0] == 'E') ? 2 : 1; 
    
    float base_price = selected_screening->price;
    float discounted_price = base_price * discount_rate;
    float seat_price = discounted_price * seat_multiplier;
    float total_bill = seat_price + service_fee;

    char title[100];
    get_movie_title(selected_screening->movie_id, title, sizeof(title));

    printf("\n+================================================+\n");
    printf("|             HOA DON THANH TOAN                 |\n");
    printf("+================================================+\n");
    printf("| Phim         : %-31s |\n", title);
    printf("| ID Suat chieu: %-31d |\n", actual_screening_id);
    printf("| Loai khach   : %-31s |\n", customer_type_str);
    
    if (seat_multiplier == 2) {
        printf("| Ma ghe       : %-9s (Ghe doi VIP x2)       |\n", actual_seat_code);
    } else {
        printf("| Ma ghe       : %-31s |\n", actual_seat_code);
    }
    
    printf("| Tien ghe     : %-27.0f VND |\n", seat_price);
    printf("| Tien dich vu : %-27.0f VND |\n", service_fee);
    printf("+------------------------------------------------+\n");
    printf("| TONG CONG    : %-27.0f VND |\n", total_bill);
    printf("+================================================+\n\n");

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

    printf("\n[HOAN TAT] Mua ve thanh cong! ID ve cua ban la: %d\n", new_ticket_id);

    free(screenings->screenings);
    free(screenings);
}
// Display tickets owned by the current user
static int display_user_tickets(int user_id) {
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    ScreeningArray *screenings = get_all_screenings(SCREENING_SOURCE_PATH);

    if (tickets == NULL || screenings == NULL) {
        if (tickets) { free(tickets->tickets); free(tickets); }
        if (screenings) { free(screenings->screenings); free(screenings); }
        return 0;
    }

    int count = 0;
    printf("\n+======================================================================================+\n");
    printf("|                              DANH SACH VE CUA BAN                                    |\n");
    printf("+-----------+---------------------------+------------------+--------+------------------+\n");
    printf("| %-9s | %-25s | %-16s | %-6s | %-16s |\n", "ID Ve", "Phim", "Gio chieu", "Ghe", "Trang thai");
    printf("+-----------+---------------------------+------------------+--------+------------------+\n");

    for (int i = 0; i < tickets->count; i++) {
        if (tickets->tickets[i].customer_id == user_id) {
            count++;
            int s_id = tickets->tickets[i].screening_id;
            
            // Find screening details
            Screening *s = NULL;
            for (int j = 0; j < screenings->count; j++) {
                if (screenings->screenings[j].screening_id == s_id) {
                    s = &screenings->screenings[j];
                    break;
                }
            }

            if (s != NULL) {
                char title[100];
                get_movie_title(s->movie_id, title, sizeof(title));

                time_t t = (time_t)s->start_time;
                struct tm *tm_info = localtime(&t);
                char time_str[20];
                strftime(time_str, sizeof(time_str), "%d/%m %H:%M", tm_info);

                // Check if screening has already happened
                time_t now = time(NULL);
                char status[20];
                if (t <= now) strcpy(status, "Da chieu");
                else strcpy(status, "Chua chieu (Hop le)");

                printf("| %-9d | %-25.25s | %-16s | %-6s | %-16s |\n",
                       tickets->tickets[i].ticket_id, title, time_str, tickets->tickets[i].seat_code, status);
            }
        }
    }
    printf("+======================================================================================+\n");

    free(tickets->tickets); free(tickets);
    free(screenings->screenings); free(screenings);

    return count;
}
// Core function: Cancel a ticket
void cancel_ticket(int current_user_id) {
    // 1. Display user's purchased tickets
    int count = display_user_tickets(current_user_id);
    if (count == 0) {
        printf("\nBan chua mua ve nao hoac khong co du lieu.\n");
        return;
    }

    // 2. Ask for Ticket ID
    int target_ticket_id;
    printf("\nNhap ID ve ban muon huy (0 de thoat): ");
    if (scanf("%d", &target_ticket_id) != 1) {
        while (getchar() != '\n');
        printf("Loi: Vui long nhap so hop le!\n");
        return;
    }
    while (getchar() != '\n');

    if (target_ticket_id == 0) return;

    // Load ticket data
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    if (tickets == NULL) return;

    Ticket *target_ticket = NULL;
    for (int i = 0; i < tickets->count; i++) {
        if (tickets->tickets[i].ticket_id == target_ticket_id) {
            target_ticket = &tickets->tickets[i];
            break;
        }
    }

    // Ticket ID not found
    if (target_ticket == NULL) {
        printf("Loi: Khong tim thay ve voi ID da cung cap.\n");
        free(tickets->tickets); free(tickets);
        return;
    }

    // Ticket doesn't belong to current user
    if (target_ticket->customer_id != current_user_id) {
        printf("Loi: Ban khong co quyen huy ve nay.\n");
        free(tickets->tickets); free(tickets);
        return;
    }

    // Load screening data to check time
    ScreeningArray *screenings = get_all_screenings(SCREENING_SOURCE_PATH);
    Screening *target_screening = NULL;
    if (screenings != NULL) {
        for (int i = 0; i < screenings->count; i++) {
            if (screenings->screenings[i].screening_id == target_ticket->screening_id) {
                target_screening = &screenings->screenings[i];
                break;
            }
        }
    }

    // Screening already started or ended
    if (target_screening != NULL) {
        time_t now = time(NULL);
        if ((time_t)target_screening->start_time <= now) {
            printf("Loi: Khong the huy ve cua suat chieu da bat dau hoac ket thuc.\n");
            free(tickets->tickets); free(tickets);
            free(screenings->screenings); free(screenings);
            return;
        }
    }

    // Display confirmation box
    char title[100] = "Unknown";
    if (target_screening != NULL) {
        get_movie_title(target_screening->movie_id, title, sizeof(title));
    }

    printf("\n+================================================+\n");
    printf("|                XAC NHAN HUY VE                 |\n");
    printf("+================================================+\n");
    printf("| ID Ve        : %-31d |\n", target_ticket->ticket_id);
    printf("| Phim         : %-31.31s |\n", title);
    printf("| Ma ghe       : %-31s |\n", target_ticket->seat_code);
    printf("+================================================+\n\n");

    // Confirm and execute deletion
    if (is_decision_yes("Ban co chac chan muon huy ve nay khong")) {
        delete_ticket_by_id(target_ticket->ticket_id, TICKET_SOURCE_PATH);
        printf("\n[THONG BAO] Huy ve thanh cong! Ghe ngoi da duoc giai phong.\n");
    } else {
        printf("\n[THONG BAO] Da huy qua trinh xoa ve.\n");
    }

    free(tickets->tickets); free(tickets);
    if (screenings) { free(screenings->screenings); free(screenings); }
}

void view_purchase_history() {
    printf("Chuc nang xem lich su dang phat trien.\n");
}