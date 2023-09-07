#include "header.h"
#include <string.h>

char currentUsername[50] = ""; // Initialize the global variable

void setCurrentUsername(const char *username) {
    strncpy(currentUsername, username, sizeof(currentUsername));
    currentUsername[sizeof(currentUsername) - 1] = '\0'; // Ensure null-termination
}
