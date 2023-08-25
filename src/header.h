#ifndef HEADER_H
#define HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "../sqlite-amalgamation-3420000/sqlite3.h"

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
};

struct User
{
    char name[50];
    char password[100];
};

// authentication functions
int loginMenu();
void registerMenu();
extern const char *DB_FILE;


// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);

//utils functions
void flushBuffer();
void Scanner(int *i);
int getUserIdByUsername(sqlite3 *db, const char *username);
int isAccountNumberTaken(const int *AccountNbr);
int isUsernameTaken(const char *username);


void createTable(sqlite3 *db);

#endif