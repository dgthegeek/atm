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
    char dateInput[50];
    char date[50];
    scanf(" %[^\n]",dateInput);
    sscanf(dateInput, "%d/%d/%d/", &newRecord.deposit.month, &newRecord.deposit.day, &newRecord.deposit.year);
    sprintf(date, "%d/%d/%d/", &newRecord.deposit.month, &newRecord.deposit.day, &newRecord.deposit.year);
    if (!isValidDate(newRecord.deposit.day, newRecord.deposit.month, newRecord.deposit.year)||strcmp(date, dateInput)) {
        printf("\n Invalid data format! Please restart again...\n\n");
        flushBuffer(); 
        goto dateValidation;
    }
    flushBuffer();
    newRecord.accountNbr = readInteger("Enter the account number : ");
    
    if (isAccountNumberTaken(&newRecord.accountNbr))
    {
        printf("\nAccount number already exist! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    printf("Enter the country: ");
char countryStr[50]; // Use a string to read the country name
if (scanf("%49s", countryStr) != 1) {
    printf("\nInvalid data format! Please restart again...\n\n");
    flushBuffer(); // Clear the input buffer
    goto dateValidation;
}

// Verify if the country string contains non-alphabetic characters
for (int i = 0; countryStr[i] != '\0'; i++) {
    if (!isalpha(countryStr[i])) {
        printf("\nInvalid country name format! Please restart again...\n\n");
        flushBuffer(); // Clear the input buffer
        goto dateValidation;
    }
}

// Copy the country string to the newRecord structure
strcpy(newRecord.country, countryStr);
    flushBuffer();
    newRecord.phone = readInteger("Enter your phone number: ");
    flushBuffer();
    newRecord.amount = readInteger("Enter the deposit amount: ");
    

    printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", newRecord.accountType);

    if (strcmp(newRecord.accountType, "savings") != 0 &&
        strcmp(newRecord.accountType, "current") != 0 &&
        strcmp(newRecord.accountType, "fixed01") != 0 &&
        strcmp(newRecord.accountType, "fixed02") != 0 &&
        strcmp(newRecord.accountType, "fixed03") != 0)
    {
        printf("\n Invalid data format! Please restart again...\n\n");
        flushBuffer();
        goto dateValidation;
    }

    char query[1000];
    sprintf(query, "INSERT INTO Records (UserId, Month, Day, Year, AccountNbr, Country, Phone, Amount, AccountType) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("\nError preparing the statement! Please restart again...\n\n");
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
}

void checkAllAccounts(int userId)
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char query[1000];
    sprintf(query, "SELECT * FROM Records WHERE UserId = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
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

    if (rc != SQLITE_DONE)
    {
        printf("Error fetching records: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateAccountInfo(int userId)
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    flushBuffer();

validation:
    int accountNbr = readInteger("Enter the account number: ");
    char query[1000];
    sprintf(query, "SELECT * FROM Records WHERE UserId = ? AND AccountNbr = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        printf("Account not found...\nPleaze try again\n");
        flushBuffer();
        goto validation;
        return;
    }

    sqlite3_finalize(stmt);

    
    flushBuffer();
choiceValidation:
    printf("1 -> Phone\n");
    printf("2 -> Country\n");
    int choice = readInteger("Choose the field to update: ");
    
    if (choice != 1 && choice != 2)
    {
        printf("Invalid choice.\n");
        flushBuffer();
        goto choiceValidation;
    }

    char fieldToUpdate[10];
    if (choice == 1)
    {
        strcpy(fieldToUpdate, "Phone");
    }
    else if (choice == 2)
    {
        strcpy(fieldToUpdate, "Country");
    }

    char newValue[100];
    printf("Enter the new value for %s: ", fieldToUpdate);
    scanf("%s", newValue);

    sprintf(query, "UPDATE Records SET %s = ? WHERE UserId = ? AND AccountNbr = ?", fieldToUpdate);

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, newValue, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        printf("Error updating account info: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Account information updated successfully.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void displayAccountInfo(int userId)
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    flushBuffer();
validation:
    int accountNbr = readInteger("Enter the account number : ");
    char query[1000];
    sprintf(query, "SELECT * FROM Records WHERE UserId = ? AND AccountNbr = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        printf("Account not found...\nPleaze try again\n");
        flushBuffer();
        goto validation;
        return;
    }
    else
    {

        printf("_____________________\n");
        printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
               accountNbr,
               sqlite3_column_int(stmt, 3),
               sqlite3_column_int(stmt, 2),
               sqlite3_column_int(stmt, 4),
               sqlite3_column_text(stmt, 6),
               sqlite3_column_int(stmt, 7),
               sqlite3_column_double(stmt, 8),
               sqlite3_column_text(stmt, 9));
                 
        if (strcmp(sqlite3_column_text(stmt, 9), "current")==0)
        {
            printf("You will not get interests because the account is of type current");
        } 
        else if (strcmp(sqlite3_column_text(stmt, 9), "saving")==0)
        {
            double interest = ((sqlite3_column_double(stmt, 8) * 0.07) / 12) / 100;
            printf("\n->You will get $%lf as interest for 10 days every month\n", interest);
        }
        else if (strcmp(sqlite3_column_text(stmt, 9), "fixed01")==0)
        {
            double interest = (sqlite3_column_double(stmt, 8) * 0.04) / 100;
            printf("\n->You will get %lf as interest for 10 days every month\n", interest);
        }
        else if (strcmp(sqlite3_column_text(stmt, 9), "fixed02")==0)
        {
            double interest = (sqlite3_column_double(stmt, 8) * 0.05 * 2) / 100;
            printf("\n->You will get %lf as interest for 10 days every month\n", interest);
        }
        else if (strcmp(sqlite3_column_text(stmt, 9), "fixed03")==0)
        {
            double interest = (sqlite3_column_double(stmt, 8) * 0.08 * 3) / 100;
            printf("\n->You will get %lf as interest for 10 days every month\n", interest);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void deleteAccount(int userId) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    flushBuffer();
validation:

    int accountNbr = readInteger("Enter the account number : ");
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
        printf("Account not found...  Try again\n");
        flushBuffer();
        goto validation;
    } else {
        // The account exists, proceed with the deletion
        sqlite3_finalize(stmt);
        
        // Prepare the delete statement
        sprintf(query, "DELETE FROM Records WHERE UserId = ? AND AccountNbr = ?");
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            printf("Error preparing delete statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, accountNbr);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            printf("Account deleted successfully.\n");
        } else {
            printf("Error deleting account: %s\n", sqlite3_errmsg(db));
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}

void transferAccount(int userId) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    flushBuffer();
validation:
    int accountNbr = readInteger("Enter the account number : ");
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
        flushBuffer();
        goto validation;
    }

    sqlite3_finalize(stmt);

    char newUsername[50];
userValidation:
    printf("Enter the username of the user you want to transfer the account to: ");
    scanf("%s", newUsername);

    sprintf(query, "SELECT ID FROM Users WHERE Name = ?");
    
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, newUsername, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        printf("User not found...\nPleaze try again\n");
        flushBuffer();
        goto userValidation;
    }

    int newUserId = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Update the account's userId
    sprintf(query, "UPDATE Records SET UserId = ? WHERE UserId = ? AND AccountNbr = ?");
    
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, newUserId);
    sqlite3_bind_int(stmt, 2, userId);
    sqlite3_bind_int(stmt, 3, accountNbr);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Account transferred successfully.\n");
    } else {
        printf("Error transferring account: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void makeTransaction(int userId) {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    flushBuffer();
validation:
    int accountNbr = readInteger("Enter the account number : ");
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
         printf("Account not found...  Try again\n");
        flushBuffer();
        goto validation;
    }

    int accountId = sqlite3_column_int(stmt, 0);
    double currentAmount = sqlite3_column_double(stmt, 8);
    char accountType[20];
    strcpy(accountType, (char*)sqlite3_column_text(stmt, 9));
    sqlite3_finalize(stmt);

    // Check if account type allows transactions
    if (strcmp(accountType, "fixed01") == 0 || strcmp(accountType, "fixed02") == 0 || strcmp(accountType, "fixed03") == 0) {
        printf("This account type does not allow transactions.\n");
        sqlite3_close(db);
        return;
    }
    flushBuffer();
    double transactionAmount;
choiceValidation:
    printf("Choose the type of transaction:\n");
    printf("1 -> Deposit\n");
    printf("2 -> Withdraw\n");
    int choice = readInteger("Enter your choice : ");

    if (choice != 1 && choice != 2) {
        printf("Invalid choice.\n");
        flushBuffer();
        goto choiceValidation;
    }

    if (choice == 1) {
amountValidation:
flushBuffer();
    int transactionAmount = readInteger("Enter the amount to deposit : ");
        if ( &transactionAmount <= 0) {
            printf("Invalid amount format or value.\n");
            goto amountValidation;
        }

        currentAmount += transactionAmount;
    } else if (choice == 2) {
amountValidation1:
        int transactionAmount = readInteger("Enter the amount to withdraw : ");
        if (&transactionAmount <= 0 || transactionAmount > currentAmount) {
            printf("Invalid amount format or value.\n");
            goto amountValidation1;
        }

        currentAmount -= transactionAmount;
    }

    // Update the account balance
    sprintf(query, "UPDATE Records SET Amount = ? WHERE ID = ?");
    
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_double(stmt, 1, currentAmount);
    sqlite3_bind_int(stmt, 2, accountId);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Transaction completed successfully.\n");
    } else {
        printf("Error processing transaction: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
