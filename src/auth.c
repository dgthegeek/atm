#include <termios.h>
#include "header.h"

const char *DB_FILE = "./data/atm.db";


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

validation:
    struct User newUser;
    printf("Enter a username: ");
    scanf("%s", newUser.name);

    if (isUsernameTaken(newUser.name)) {
        printf("Username is already taken.\nPleaze try again...\n");
        sqlite3_close(db);
        goto validation;
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
    printf("Registration succeededful!\n");
    mainMenu(&newUser);
}


int loginMenu(struct User *u)
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

    system("stty -echo");
    printf("Enter your password: ");
    scanf("%s", password);
    system("stty echo");

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
    int succeeded = 0;
    if (rc == SQLITE_ROW) // User found with correct password
    {
        succeeded = 1; // Return succeeded status
    }
    else
    {
        printf("Invalide username or password, try again\n");
        goto validation;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    *u->name = username;
    mainMenu(&u);
}
