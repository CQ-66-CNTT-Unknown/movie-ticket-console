#pragma once

#include "../models/user.h"

/**
 * @brief Get a user by username and password from the system
 * @param username The username to search for
 * @param password The password to verify
 * @param user_source_path The file path for users
 * @return pointer to User if found and password matches, or NULL if not found or password is incorrect
 */
User *get_user_by_credentials(const char *username, const char *password, const char *user_source_path);

/**
 * @brief Get a user by ID from the system
 * @param user_id The ID of the user to search for
 * @param user_source_path The file path for users
 * @return pointer to User if found, or NULL if not found
 */
User *get_user_by_id(int user_id, const char *user_source_path);
