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
void book_ticket(int screening_id, char seat_code) {
    // TODO
}

void cancel_ticket(int ticket_id) {
    // TODO
}

void view_purchase_history() {
    // TODO
}
