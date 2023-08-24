#include "header.h"

void mainMenu(struct User u)
{
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
    scanf("%d", &option);
validOption:
    Scanner(&option);
    getchar();

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
        goto validOption;
    }
};

void initMenu(struct User *u)
{

    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");

validOption:
    Scanner(&option);
    getchar();
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
        goto validOption;
    }
};

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
        "ID_User INT NOT NULL,"
        "Date TEXT NOT NULL,"
        "Number INT NOT NULL,"
        "Montant REAL NOT NULL,"
        "Type TEXT NOT NULL);"; 

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
