#pragma once

#include "../models/user.h"

/**
 * @brief Log in a user with the given username and password, the function ends until the user successfully logs in
 * @return A pointer to the logged-in User struct. The caller is responsible for freeing the memory allocated for the User struct.
 */
User *login();
