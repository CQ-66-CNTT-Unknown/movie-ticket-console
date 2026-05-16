#pragma once

#include "../models/ticket.h"

#include <stdio.h>

/**
 * @brief Retrieves all tickets from the specified source
 * @param ticket_source_path The file path to the ticket source
 * @return pointer to TicketArray containing all retrieved tickets or `NULL` if an error occurs
 */
TicketArray *get_all_tickets(const char *ticket_source_path);


/**
 * @brief Deletes a ticket with the specified ID from the source
 * @param ticket_source_path The file path to the ticket source
 * @param screening_id The ID of the screening for which to delete tickets
 */
void delete_ticket_record_by_screening_id(int screening_id, const char *ticket_source_path);


/**
 * @brief Deletes a ticket by its ID
 * @param ticket_id The ID of the ticket to delete
 * @param ticket_source_path The file path to the ticket source
 */
void delete_ticket_by_id(int ticket_id, const char *ticket_source_path);