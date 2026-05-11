#pragma once

/**
 * @brief Generates a temporary file path based on the source file path
 * @param original_path The path to the source file
 * @return A pointer to the generated temporary file path
 */
char *get_temp_file_path(const char *original_path);
