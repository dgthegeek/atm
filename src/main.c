#include "header.h"

void mainMenu(struct User u)
{
validation:
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");


    Scanner(&option);
   

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        fprintf("%d",2);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        fprintf("%d",3);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        fprintf("%d",5);

        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        fprintf("%d",6);

        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        fprintf("%d",7);

        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
        system("sleep 2");
        goto validation;
    }
};

void initMenu(struct User *u)
{
validOption:
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");

    Scanner(&option);


    switch (option)
    {
    case 1:
        loginMenu();
        break;
    case 2:
        registerMenu();
        break;
    case 3:
        exit(1);
        break;
    default:
        printf("Insert a valid operation!\n");
        system("sleep 2");
        goto validOption;
    }
};


int main()
{
    struct User u;

    sqlite3 *db;
    char *errMsg = 0;

    const char *dbPath = "./data/atm.db";

    int rc = sqlite3_open(dbPath, &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    createTable(db);

    initMenu(&u);
    mainMenu(u);
    return 0;
}
