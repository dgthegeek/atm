#include <termios.h>
#include "header.h"

const char *DB_FILE = "./data/atm.db";

// Function to check if a username already exists
int isUsernameTaken(const char *username)
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1; // Return taken
    }

    char query[100];
    sprintf(query, "SELECT * FROM Users WHERE Name = '%s'", username);

    int taken = 0;
    rc = sqlite3_exec(db, query, NULL, NULL, NULL);
    if (rc == SQLITE_ROW) // Username found
    {
        taken = 1; // Return taken
    }
    else if (rc != SQLITE_OK)
    {
        printf("Error checking username: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    return taken;
}

void registerMenu()
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    struct User newUser;
    printf("Enter a username: ");
    scanf("%s", newUser.name);

    if (isUsernameTaken(newUser.name))
    
    {
        printf("Username is already taken.\n");
        sqlite3_close(db);
        return;
    }

    printf("Enter a password: ");
    scanf("%s", newUser.password);

    // Prepare the query
    sqlite3_stmt *stmt;
    const char *query = "INSERT INTO Users (Name, Password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, newUser.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, newUser.password, -1, SQLITE_STATIC);

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Error inserting user: %s\n", sqlite3_errmsg(db));
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Commit the transaction
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error committing transaction: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_close(db);
    printf("Registration successful!\n");
}


int loginMenu()
{

    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0; // Return failure status
    }
validation:
    char username[50], password[100];
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    char query[100];
    sprintf(query, "SELECT * FROM Users WHERE Name = ? AND Password = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0; // Return failure status
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int success = 0;
    if (rc == SQLITE_ROW) // User found with correct password
    {
        success = 1; // Return success status
    }
    else
    {
        printf("Invalide username or password, try again\n");
        goto validation;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return success;
}
