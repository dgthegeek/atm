#include "header.h"

void createTable(sqlite3 *db)
{
    char *errMsg = 0;

    const char *Users =
        "CREATE TABLE IF NOT EXISTS Users ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "Name TEXT NOT NULL,"
        "Password TEXT NOT NULL);";

    const char *Records =
        "CREATE TABLE IF NOT EXISTS Records ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "UserId INTEGER,"
        "Month INTEGER,"
        "Day INTEGER,"
        "Year INTEGER,"
        "AccountNbr INTEGER UNIQUE,"
        "Country TEXT,"
        "Phone INTEGER,"
        "Amount REAL,"
        "AccountType TEXT);";

    int rc = sqlite3_exec(db, Users, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    int rc1 = sqlite3_exec(db, Records, 0, 0, &errMsg);
    if (rc1 != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}