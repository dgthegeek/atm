#ifndef HEADER_H
#define HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "../sqlite-amalgamation-3420000/sqlite3.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdbool.h>


struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int userId;
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
int loginMenu(struct User *u);
void registerMenu();
extern const char *DB_FILE;


// system function
void createNewAcc(struct User *u);
void mainMenu(struct User *u);
void checkAllAccounts(int userId);
void updateAccountInfo(int userId);
void transferAccount(int userId);
void makeTransaction(int userId);
bool isAllDigits(const char *input);


//utils functions
void flushBuffer();
void Scanner(int *i);
int getUserIdByUsername(const char *username);
int isAccountNumberTaken(const int *AccountNbr);
int isUsernameTaken(const char *username);
bool isLeapYear(int year);
bool isValidDate(int day, int month, int year);
void displayAccountInfo(int userId);
void deleteAccount(int userId);
void createTable(sqlite3 *db);

//Session
extern char currentUsername[50]; // Global variable to store the current user's username

void setCurrentUsername(const char *username);

#endif