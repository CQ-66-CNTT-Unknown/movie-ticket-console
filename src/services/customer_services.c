#include "../../include/services/customer_services.h"
#include "../../include/constanst.h" 
#include "../../include/repos/ticket_repo.h"
#include "../../include/repos/movie_repo.h"
#include "../../include/utils/input_utils.h"
#include "../../include/utils/decision_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
//Function to display seat map
static void display_seat_map(int screening_id) {
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    char seats[5][5]; 
    
    // Initialize all empty seats (character 'O').
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            seats[i][j] = 'O';
        }
    }
    
    // If ticket data is available, mark reserved seats with an 'X'.
    if (tickets != NULL) {
        for(int i = 0; i < tickets->count; i++) {
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

    // Print the map to the screen.
    printf("\n===== BẢN ĐỒ CHỖ NGỒI (Suất chiếu %d) =====\n", screening_id);
    printf("  1 2 3 4 5\n");
    for(int i = 0; i < 5; i++) {
        printf("%c ", 'A' + i);
        for(int j = 0; j < 5; j++) {
            printf("%c ", seats[i][j]);
        }
        printf("\n");
    }
    printf("Ghi chú: O (Ghế trống), X (Ghế đã đặt)\n");
    printf("===========================================\n");
}
//create new ticket id
static int generate_new_ticket_id() {
    TicketArray *tickets = get_all_tickets(TICKET_SOURCE_PATH);
    int max_id = 0;
    if (tickets != NULL) {
        for (int i = 0; i < tickets->count; i++) {
            if (tickets->tickets[i].ticket_id > max_id) {
                max_id = tickets->tickets[i].ticket_id;
            }
        }
        if (tickets->tickets) free(tickets->tickets);
        free(tickets);
    }
    return max_id + 1;
}
//--ticket purchasing function--
void book_ticket(int screening_id, char seat_code) {
    int actual_screening_id;     
    char actual_seat_code[10];   
    int current_user_id = 1;

    Screening *selected_screening = NULL;
    ScreeningArray *screenings = get_all_screenings(SCREENING_SOURCE_PATH);

    if (screenings == NULL || screenings->count == 0) {
        printf("Hiện tại không có suất chiếu nào.\n");
        if (screenings) free(screenings);
        return;
    }
    while (true) {
        printf("\nNhập ID suất chiếu muốn mua (hoặc nhập 0 để thoát): ");
        if (scanf("%d", &actual_screening_id) != 1) {
            while(getchar() != '\n'); 
            printf("!!Vui lòng nhập số hợp lệ!!\n");
            continue;
        }
        if (actual_screening_id == 0) {
            free(screenings->screenings);
            free(screenings);
            return;
        }
 // error:id does not exist       
    bool found = false;
        for (int i = 0; i < screenings->count; i++) {
            if (screenings->screenings[i].screening_id == actual_screening_id) {
                selected_screening = &screenings->screenings[i];
                found = true;
                break;
            }
        }

        if (!found) {
            printf("Lỗi: ID suất chiếu không tồn tại. Vui lòng thử lại.\n");
        } else {
            break;  
        }
    }      
//--Display the 5x5 seating chart--
  display_seat_map(actual_screening_id);

    while (true){
        printf("Nhập mã chỗ ngồi hoặc 'Q' để thoát: ");
        scanf("%s", actual_seat_code);

        if (strcmp(actual_seat_code, "Q") == 0 || strcmp(actual_seat_code, "q") == 0) {
            free(screenings->screenings);
            free(screenings);
            return;
        }  
        actual_seat_code[0] = toupper(actual_seat_code[0]); //Capitalize the first letter

        if (strlen(actual_seat_code) != 2 || actual_seat_code[0] < 'A' || actual_seat_code[0] > 'E' || actual_seat_code[1] < '1' || actual_seat_code[1] > '5') {
            printf("Lỗi: Mã chỗ ngồi không hợp lệ (Phải từ A1 đến E5). Vui lòng nhập lại.\n");
            continue;
        }
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
            printf("Lỗi (E3): Ghế %s đã có người mua, vui lòng chọn ghế khác.\n", actual_seat_code);
            continue;
        }

        break;
}
// Confirm ticket purchase information
printf("\n--- XÁC NHẬN THÔNG TIN ĐẶT VÉ ---\n");
    printf("Suất chiếu ID: %d\n", actual_screening_id);
    printf("Mã ghế: %s\n", actual_seat_code);
    printf("Giá vé: %.0f VND\n", selected_screening->price);
    printf("---------------------------------\n");
    
    if (!is_decision_yes("Xác nhận thanh toán và mua vé")) {
        printf("Đã hủy quá trình mua vé.\n");
        free(screenings->screenings);
        free(screenings);
        return;
    }

}
void cancel_ticket(int ticket_id) {
    // TODO
}

void view_purchase_history() {
    // TODO
}
