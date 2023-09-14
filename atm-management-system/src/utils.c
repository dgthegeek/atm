#include "header.h"

void flushBuffer()
{
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n')
    {
        // Reading characters until newline or end-of-file is encountered
    }
}

void flushInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

short Atoi(char text[255])
{
    size_t i;
    for (i = 0; i < strlen(text); i++)
    {
        int r = text[i];
        if ((r < 48 || r > 57))
        {
            return -1;
        }
    }
    return 1;
}

void Scanner(int *a)
{
    char text[255];
invalid:
    scanf("%[^\n]s", text);
    short b = Atoi(text);
    if (b == -1)
    {
        printf("Please Enter a valid Int\n");
        flushInputBuffer();
        goto invalid;
    }
    *a = atoi(text);
    if (*a > INT_MAX)
    {
        printf("Please Enter a valid Int\n");
        flushInputBuffer();
        goto invalid;
    }
}



int readInteger(const char *prompt) {
    int number;
    char input[20];

    while (1) {
        printf("%s", prompt);

        if (scanf("%19[^\n]s", input) != 1) {
            printf("\nInvalid data format! Please try again...\n\n");
            flushBuffer(); // Assuming you have a flushBuffer() function to clear the input buffer
            continue;
        }

        int validInput = 1;

        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                validInput = 0;
                break;
            }
        }

        if (!validInput) {
            printf("\nInvalid account number format! Please try again...\n\n");
            flushBuffer(); // Assuming you have a flushBuffer() function to clear the input buffer
            continue;
        }

        if (sscanf(input, "%d", &number) != 1) {
            printf("\nInvalid account number format! Please try again...\n\n");
            flushBuffer(); // Assuming you have a flushBuffer() function to clear the input buffer
            continue;
        }

        // If we reach this point, we have a valid integer
        break;
    }

    return number;
}



const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
  
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",

	    u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(&u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(&u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(&u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}


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
    sprintf(query, "SELECT * FROM Users WHERE Name = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1; // Return taken
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int taken = 0;
    if (rc == SQLITE_ROW) // Username found
    {
        taken = 1; // Return taken
    }
    else if (rc != SQLITE_DONE)
    {
        printf("Error checking username: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return taken;
}

// Function to check if a username already exists
int isAccountNumberTaken(const int *AccountNbr)
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
    sprintf(query, "SELECT * FROM Records WHERE AccountNbr = ?");

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1; // Return taken
    }

    sqlite3_bind_text(stmt, 1, AccountNbr, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int taken = 0;
    if (rc == SQLITE_ROW) // Username found
    {
        taken = 1; // Return taken
    }
    else if (rc != SQLITE_DONE)
    {
        printf("Error checking username: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return taken;
}

int getUserIdByUsername(const char *username) {
    int userId = -1; // Initialize with an invalid value
    sqlite3 *db;
    
    int rc1 = sqlite3_open(DB_FILE, &db);
    if (rc1 != SQLITE_OK)
    {
        printf("Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *query = "SELECT ID FROM Users WHERE Name = ?";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return userId; // Return the invalid value
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
    } else if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error retrieving user ID: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return userId;
}

bool isLeapYear(int year) {
    // Check if the year is a leap year
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

bool isValidDate(int day, int month, int year) {
    // Check if the year is within a reasonable range (adjust this as needed)
    if (year < 1 || year > 9999) {
        return false;
    }

    // Check if the month is valid (between 1 and 12)
    if (month < 1 || month > 12) {
        return false;
    }

    // Check if the day is valid for the given month
    int maxDay = 31; // Default maximum day

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    } else if (month == 2) {
        if (isLeapYear(year)) {
            maxDay = 29;
        } else {
            maxDay = 28;
        }
    }

    if (day < 1 || day > maxDay) {
        return false;
    }

    // If all checks pass, the date is valid
    return true;
}
