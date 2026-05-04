#pragma once

#include "../models/ticket.h"

#include <stdio.h>

/**
 * @brief Retrieves all tickets from the specified source
 * @param ticket_source The file pointer to the ticket source
 * @return A `TicketArray` containing all retrieved tickets
 */
TicketArray get_all_tickets(FILE *ticket_source);