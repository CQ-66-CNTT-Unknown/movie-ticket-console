#include "../../include/repos/ticket_repo.h"
#include "../../include/constanst.h"
#include "../../include/utils/csv_utils.h"
#include "../../include/utils/get_temp_file_path.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TicketArray *get_all_tickets(const char *ticket_source_path) {
    FILE *ticket_source = fopen(ticket_source_path, "r");
    if (ticket_source == NULL) {
        fprintf(stderr, "Error opening ticket source file: %s\n", ticket_source_path);
        return NULL;
    }

    TicketArray *ticket_array = (TicketArray *) malloc(sizeof(TicketArray));
    if (ticket_array == NULL) {
        fprintf(stderr, "Memory allocation failed for loading tickets\n");
        fclose(ticket_source);
        return NULL;
    }

    ticket_array->tickets = NULL;
    ticket_array->count = 0;
    ticket_array->capacity = 0;

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), ticket_source); // Skip header line

    while (fgets(buffer, sizeof(buffer), ticket_source)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        char *fields[N_TICKETS_FIELDS];
        parse_csv_line(buffer, fields, N_TICKETS_FIELDS);

        if (ticket_array->count >= ticket_array->capacity) {
            int new_capacity = (ticket_array->capacity == 0) ? 4 : ticket_array->capacity * 2;
            Ticket *new_tickets = (Ticket *) realloc(ticket_array->tickets, new_capacity * sizeof(Ticket));

            if (new_tickets == NULL) {
                fprintf(stderr, "Memory allocation failed for loading tickets\n");
                free(ticket_array->tickets);
                fclose(ticket_source);
                free(ticket_array);
                return NULL;
            }

            ticket_array->tickets = new_tickets;
            ticket_array->capacity = new_capacity;
        }

        Ticket *current_ticket = &ticket_array->tickets[ticket_array->count];

        current_ticket->ticket_id = atoi(fields[0]);
        current_ticket->screening_id = atoi(fields[1]);
        current_ticket->customer_id = atoi(fields[2]);

        strncpy(current_ticket->seat_code, fields[3], sizeof(current_ticket->seat_code) - 1);
        current_ticket->seat_code[sizeof(current_ticket->seat_code) - 1] = '\0';

        ticket_array->count++;
    }

    fclose(ticket_source);
    return ticket_array;
}

void delete_ticket_record_by_screening_id(int screening_id, const char *ticket_source_path) {
    FILE *ticket_source = fopen(ticket_source_path, "r");
    if (ticket_source == NULL) {
        fprintf(stderr, "Failed to open ticket source file: %s\n", ticket_source_path);
        return;
    }

    char *temp_path = get_temp_file_path(ticket_source_path);

    FILE *temp_file = fopen(temp_path, "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file for ticket deletion\n");
        fclose(ticket_source);
        free(temp_path);
        return;
    }

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, LINE_DATA_BUFFER_SIZE, ticket_source); // Read and write header line
    fprintf(temp_file, "%s", buffer);

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, ticket_source)) {
        char temp_buffer[LINE_DATA_BUFFER_SIZE];
        strcpy(temp_buffer, buffer);

        char *fields[N_TICKETS_FIELDS];
        parse_csv_line(buffer, fields, N_TICKETS_FIELDS);

        if (atoi(fields[1]) != screening_id) {
            fprintf(temp_file, "%s", temp_buffer);
        }
    }

    fclose(ticket_source);
    fclose(temp_file);

    remove(ticket_source_path);
    rename(temp_path, ticket_source_path);

    free(temp_path);
}

void delete_ticket_by_id(int ticket_id, const char *ticket_source_path) {
    FILE *ticket_source = fopen(ticket_source_path, "r");
    if (ticket_source == NULL) {
        fprintf(stderr, "Failed to open ticket source file: %s\n", ticket_source_path);
        return;
    }

    char *temp_path = get_temp_file_path(ticket_source_path);
    FILE *temp_file = fopen(temp_path, "w");
    if (temp_file == NULL) {
        fprintf(stderr, "Failed to create temporary file for ticket deletion\n");
        fclose(ticket_source);
        free(temp_path);
        return;
    }

    char buffer[LINE_DATA_BUFFER_SIZE];
    fgets(buffer, LINE_DATA_BUFFER_SIZE, ticket_source); // Read and write header line
    fprintf(temp_file, "%s", buffer);

    while (fgets(buffer, LINE_DATA_BUFFER_SIZE, ticket_source)) {
        char temp_buffer[LINE_DATA_BUFFER_SIZE];
        strcpy(temp_buffer, buffer);

        char *fields[N_TICKETS_FIELDS];
        parse_csv_line(buffer, fields, N_TICKETS_FIELDS);

        // Only write lines that do NOT match the target ticket_id
        if (atoi(fields[0]) != ticket_id) {
            fprintf(temp_file, "%s", temp_buffer);
        }
    }

    fclose(ticket_source);
    fclose(temp_file);

    remove(ticket_source_path);
    rename(temp_path, ticket_source_path);

    free(temp_path);
}