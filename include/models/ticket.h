#pragma once

/**
 * @brief Ticket structure representing a ticket in the system
 */
typedef struct Ticket {
    int ticket_id; /**< The unique identifier for the ticket */
    int screening_id; /**< The ID of the screening for which the ticket is booked */
    int customer_id; /**< The ID of the customer who booked the ticket */
    char seat_code[2]; /**< The code representing the seat location */
};
