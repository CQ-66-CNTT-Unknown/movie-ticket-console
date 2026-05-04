#include "../../include/repos/ticket_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TicketArray get_all_tickets(FILE *ticket_source) {
    TicketArray ticket_array = {NULL, 0, 0};

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), ticket_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), ticket_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_TICKETS_FIELDS];
        parse_csv_line(buffer, fields, N_TICKETS_FIELDS);

        if (ticket_array.count >= ticket_array.capacity) {
            int new_capacity = (ticket_array.capacity == 0) ? 4 : ticket_array.capacity * 2;
            Ticket *new_tickets = (Ticket *) realloc(ticket_array.tickets, new_capacity * sizeof(Ticket));

            if (new_tickets == NULL) {
                fprintf(stderr, "Memory allocation failed for tickets array\n");
                free(ticket_array.tickets);
                exit(EXIT_FAILURE);
            }

            ticket_array.tickets = new_tickets;
            ticket_array.capacity = new_capacity;
        }

        Ticket *current_ticket = &ticket_array.tickets[ticket_array.count];

        current_ticket->ticket_id = atoi(fields[0]);
        current_ticket->screening_id = atoi(fields[1]);
        current_ticket->customer_id = atoi(fields[2]);

        strncpy(current_ticket->seat_code, fields[3], sizeof(current_ticket->seat_code) - 1);
        current_ticket->seat_code[sizeof(current_ticket->seat_code) - 1] = '\0';

        ticket_array.count++;
    }

    return ticket_array;
}
