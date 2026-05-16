#pragma once

/**
 * @brief Book a ticket for a specific screening
 * @param screening_id The ID of the screening for which to book a ticket
 * @param seat_code The code of the seat to book
 */
void book_ticket(int user_id);

/**
 * @brief Cancel a booked ticket
 * @param current_user_id The ID of the user requesting to cancel
 */
void cancel_ticket(int current_user_id); 

/**
 * @brief View purchase history for a specific user
 * @param current_user_id The ID of the user requesting to view history
 */
void view_purchase_history(int current_user_id);