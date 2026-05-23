
// -------------- LIBRARYES --------------

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------- ACCOUNT STRUCT --------------

typedef struct account
{
    char *user_name;
    int password;
    int money;
    struct account *next;
} account;

// ------------ FUNTIONS HEADCUPS -------------

void insert_money(account *log);
void withdraw_money(account *log);
void create_account();
void read_accounts();
void update_accounts();
void unload_accounts();
account *login_account();
int hash(char *c);

// ----------- GLOBALS VARIABLES NEEDED ---------------

account *hashT[26];
int moneyInBank;
account *bank = NULL;
const int bank_password = 956846;

int main()
{

    // -------- LOADED ACCOUNTS IN A HASH TABLE -----------

    read_accounts();

    while (1)
    {
        // ---------------- OPTIONS YOU HAVE --------------

        int option = 0;
        printf("\nInsert command\n\n1- Login\n2- Create account\n3- Exit\n\n");
        printf("INPUT:");
        scanf("%i", &option);

        // ------------ OPTION : LOGIN ------------
        if (option == 1)
        {

            // -------- LOGED ACCOUNT ---------

            account *log = login_account();
            if (log == NULL)
            {
                return 1;
            }
            while (1)
            {

                // --------------- OPTIONS WHEN LOGGED IN -------------
                printf("\nInsert command\n\n1- Insert money\n2- Exit money\n3- See current "
                       "money\n4- Go back\n\n");
                printf("INPUT:");
                scanf("%i", &option);

                // ------------- OPTION : INSERT MONEY -----------------
                if (option == 1)
                {
                    insert_money(log);
                }
                // ------------- OPTION: WITHDRAW MONEY -----------------
                else if (option == 2)
                {
                    withdraw_money(log);
                }
                // ------------- OPTION : SEE CURRENT MONEY -----------------
                else if (option == 3)
                {
                    printf("\nCurrent money:%i\n", log->money);
                }
                // --------------- OPTION : GO BACK -------------------
                else if (option == 4)
                {
                    break;
                }
            }
        }

        // ------------- OPTION : GET ACCOUNT ------------
        else if (option == 2)
        {
            create_account();
        }

        // --------------- OPTION : EXIT --------------
        else if (option == 3)
        {
            unload_accounts();
            return 0;
        }
    }
}

void unload_accounts()
{
    // ------- UNLOAD --------

    for (int i = 0; i < 26; i++)
    {
        account *ptr = hashT[i];
        account *tmp = NULL;
        while (ptr != NULL)
        {
            tmp = ptr->next;
            free(ptr->user_name);
            free(ptr);
            ptr = tmp;
        }
    }
}

void withdraw_money(account *log)
{
    // ---------- EXIT MONEY VARIABLE ----------

    int withdraw_money = 0;
    printf("\nEXIT:");
    scanf("%i", &withdraw_money);

    // ---------- CHECK LOGGED MONEY ----------
    if (log->money < withdraw_money)
    {
        printf("\nINSUFFICIENT MONEY\n");
    }
    else if (moneyInBank < withdraw_money)
    {
        printf("\nBANK HAS INSUFFICIENT MONEY\n");
    }
    else
    {
        log->money -= withdraw_money;
        moneyInBank -= withdraw_money;
    }

    // ----------- UPDATE ACCOUNTS ----------

    update_accounts();
}

void insert_money(account *log)
{
    // -------- INSERT MONEY VARIABLE --------

    int inserted = 0;
    printf("\nINSERT:");
    scanf("%i", &inserted);

    log->money += inserted;
    moneyInBank += inserted;

    // ------- UPDATE ACCOUNTS --------

    update_accounts();
}

void update_accounts()
{
    // -------- FILE ----------

    FILE *file = fopen("Users.txt", "w");

    // --------- UPDATE BANK ----------
    if(bank != NULL)
    {
       bank->money = moneyInBank;
    }

    // --------- UPDATE BY REWRITING --------

    for (int i = 0; i < 26; i++)
    {
        for (account *ptr = hashT[i]; ptr != NULL; ptr = ptr->next)
        {
            fprintf(file, "%s", ptr->user_name);
            fprintf(file, " %d", ptr->password);
            fprintf(file, " %d\n", ptr->money);
        }
    }

    fclose(file);
}

int hash(char *c)
{
    char ch = toupper(c[0]);
    if(ch < 'A' || ch > 'Z')
    {
        return 0;
    }
    return (ch - 'A');
}

void create_account()
{
    // -------------- VARIABLES NEEDED ------------------

    account *n = malloc(sizeof(account));
    int password = 0;
    char *new_user_name = malloc(sizeof(char) * 10);

    // ------------- GET VARIABLES AND SAVE ----------------

    printf("\n");
    printf("UserName:");
    scanf("%s", new_user_name);
    printf("Password:");
    scanf("%i", &password);

    n->user_name = new_user_name;
    n->password = password;
    n->next = NULL;
    n->money = 0;
    new_user_name = NULL;

    // ---------- INTRODUCE IN HASH TABLE ------------

    int place = hash(n->user_name);
    if (hashT[place] == NULL)
    {
        hashT[place] = n;
        n->next = NULL;
    }
    else
    {
        n->next = hashT[place];
        hashT[place] = n;
    }

    // ----------- INTRODUCE IN TEXT FILE --------------

    FILE *file = fopen("Users.txt", "a");
    fprintf(file, "%s", n->user_name);
    fprintf(file, " %d", n->password);
    fprintf(file, " %d\n", n->money);

    fclose(file);
}

void read_accounts()
{
    // ----------- VARIABLES NEEDED --------------

    char *name = malloc(sizeof(char) * 10);
    int password = 0;
    int money = 0;

    //--------------- LOAD USERS AND PASSWORD FROM FILE -----------------

    FILE *file = fopen("Users.txt", "a+");
    if (file == NULL)
    {
        printf("No accounts");
        return;
    }
    rewind(file);

    // ---------------- READ FILE ------------------

    while (fscanf(file, "%s", name) != EOF && fscanf(file, "%i", &password) != EOF &&
           fscanf(file, "%i", &money) != EOF)
    {
        account *n = malloc(sizeof(account));
        n->user_name = malloc(sizeof(char) * 10);
        strcpy(n->user_name, name);
        n->password = password;
        n->money = money;

        // ----------- CHECK IF BANK ACCOUNT EXISTS ------------

        if (strcmp(n->user_name, "Bank") == 0)
        {
            if (n->password == bank_password)
            {
                bank = n;
                moneyInBank = bank->money;
            }
        }

        // ------------ INSERT IN HASH TABLE --------------

        int place = hash(n->user_name);
        if (hashT[place] == NULL)
        {
            hashT[place] = n;
            n->next = NULL;
        }
        else
        {
            n->next = hashT[place];
            hashT[place] = n;
        }
    }

    // ------------ IF BANK DATA IS NOT SAVED , SAVE IT IN BANK ACCOUNT ------------------

    if (bank == NULL)
    {
        fprintf(file, "%s", "Bank");
        fprintf(file, " %d", bank_password);
        int initial_money = 1000;
        fprintf(file, " %d\n", initial_money);
    }

    fclose(file);
    free(name);

}

account *login_account()
{
    // ------------ VARIABLES NEEDED ------------

    char *name = malloc(sizeof(char) * 10);
    int password = 0;
    account *log = NULL;

    // ------------- GET USERNAME ----------------

    printf("\n");
    printf("UserName:");
    scanf("%s", name);

    // ----------- CHECK IF USERNAME EXISTS --------------

    int pos = hash(name);

    if (hashT[pos] == NULL)
    {
        printf("Inexistent Username\n");
        free(name);
        return NULL;
    }

    for (account *ptr = hashT[pos]; ptr != NULL; ptr = ptr->next)
    {
        if (strcmp(ptr->user_name, name) == 0)
        {
            log = ptr;
        }
    }

    if (log == NULL)
    {
        printf("Inexistent Username\n");
        free(name);
        return NULL;
    }

    // ---------------- CHEK IF PASSWORD IS CORRECT -------------

    for (int i = 2; i >= 0; i--)
    {
        printf("Password:");
        scanf("%i", &password);

        if (password == log->password)
        {
            printf("Logged\n");
            free(name);
            return log;
        }
        else
        {
            printf("\nIncorrect password \nRemaining %i attemps\n\n", i);
        }
    }

    printf("So many attemps\n\n");
    free(name);
    
    return NULL;
}
