#include "header.h"

void createNewAcc(struct User *u)
{
    sqlite3 *db;
    
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }


    struct Record newRecord;
dateValidation:

    printf("Enter today's date (mm/dd/yyyy): ");
    if (scanf("%d/%d/%d", &newRecord.deposit.month, &newRecord.deposit.day, &newRecord.deposit.year) != 3)
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    printf("Enter the account number: ");
    if (scanf("%d", &newRecord.accountNbr) != 1)
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    if (isAccountNumberTaken(&newRecord.accountNbr))
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    printf("Enter the country: ");
    scanf("%s", newRecord.country);

    printf("Enter the phone number: ");
    if (scanf("%d", &newRecord.phone) != 1)
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    printf("Enter amount to deposit: ");
    if (scanf("%lf", &newRecord.amount) != 1)
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", newRecord.accountType);
    
    if (strcmp(newRecord.accountType, "saving") != 0 &&
        strcmp(newRecord.accountType, "current") != 0 &&
        strcmp(newRecord.accountType, "fixed01") != 0 &&
        strcmp(newRecord.accountType, "fixed02") != 0 &&
        strcmp(newRecord.accountType, "fixed03") != 0)
    {
        printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    char query[1000];
    sprintf(query, "INSERT INTO Records (UserId, Month, Day, Year, AccountNbr, Country, Phone, Amount, AccountType) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
       printf("\n Invalid date format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }
    int userId = getUserIdByUsername(currentUsername);

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, newRecord.deposit.month);
    sqlite3_bind_int(stmt, 3, newRecord.deposit.day);
    sqlite3_bind_int(stmt, 4, newRecord.deposit.year);
    sqlite3_bind_int(stmt, 5, newRecord.accountNbr);
    sqlite3_bind_text(stmt, 6, newRecord.country, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, newRecord.phone);
    sqlite3_bind_double(stmt, 8, newRecord.amount);
    sqlite3_bind_text(stmt, 9, newRecord.accountType, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Error inserting record: %s\n", sqlite3_errmsg(db));
        printf("\nPlease restart again...\n\n");
        goto dateValidation;

    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    success();
}


void checkAllAccounts(int userId) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char query[1000];
    sprintf(query, "SELECT * FROM Records WHERE UserId = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int accountNbr = sqlite3_column_int(stmt, 5);
        int day = sqlite3_column_int(stmt, 2);
        int month = sqlite3_column_int(stmt, 3);
        int year = sqlite3_column_int(stmt, 4);
        const char *country = (const char *)sqlite3_column_text(stmt, 6);
        int phone = sqlite3_column_int(stmt, 7);
        double amount = sqlite3_column_double(stmt, 8);
        const char *accountType = (const char *)sqlite3_column_text(stmt, 9);

        printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   accountNbr,
                   day,
                   month,
                   year,
                   country,
                   phone,
                   amount,
                   accountType);
    }

    if (rc != SQLITE_DONE) {
        printf("Error fetching records: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateAccountInfo(int userId) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    
validation:
    
    int accountNbr;
    printf("Enter the account number you want to update: ");
    if (scanf("%d", &accountNbr) != 1) {
        printf("Invalid account number format... Try again\n");
        while (getchar() != '\n'); // Clear the input buffer
        flushBuffer();
        goto validation;
    }

    char query[1000];
    sprintf(query, "SELECT * FROM Records WHERE UserId = ? AND AccountNbr = ?");
    
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        printf("Account not found...\nPleaze try again\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        goto validation;
        return;
    }

    sqlite3_finalize(stmt);

    int choice;
choiceValidation:
    printf("Choose the field to update:\n");
    printf("1 -> Phone\n");
    printf("2 -> Country\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("Invalid choice.\n");
        while (getchar() != '\n'); // Clear the input buffer
        goto choiceValidation;
        sqlite3_close(db);
        return;
    }

    char fieldToUpdate[10];
    if (choice == 1) {
        strcpy(fieldToUpdate, "Phone");
    } else if (choice == 2) {
        strcpy(fieldToUpdate, "Country");
    }

    char newValue[100];
    printf("Enter the new value for %s: ", fieldToUpdate);
    scanf("%s", newValue);

    sprintf(query, "UPDATE Records SET %s = ? WHERE UserId = ? AND AccountNbr = ?", fieldToUpdate);

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, newValue, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Error updating account info: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Account information updated successfully.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
