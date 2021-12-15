#include "config.h"
#include "find.h"
#include "load.h"
#include "save.h"
#include "structure.h"
#include "today.h"
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static const struct option g_options[] = {
    {"help", no_argument, NULL, 'h'},  {"version", no_argument, NULL, 'V'},   {"verbose", no_argument, NULL, 'v'},
    {"debug", no_argument, NULL, 'd'}, {"log", required_argument, NULL, 'l'}, {"load", required_argument, NULL, 'L'},
};

int Bank_print_help(char** argv, int return_code)
{
    printf("%s\n", CMAKE_PROJECT_NAME);
    printf("Usage: %s [OPTION]...\n", argv[0]);
    printf("\n");
    printf("  -h, --help            display this help and exit\n");
    printf("  -V, --version         output version information and exit\n");
    printf("  -v, --verbose         verbose output\n");
    printf("  -d, --debug           debug output\n");
    printf("  -l, --log=FILE        log to FILE\n");
    printf("  -L, --load=FILE       load FILE\n");
    printf("\n");
    printf("Report bugs to %s <%s>.\n", CMAKE_AUTHOR_NAME, CMAKE_AUTHOR_EMAIL);
    return return_code;
}

int Bank_print_version(void)
{
    printf("%s v%s\n", CMAKE_PROJECT_NAME, CMAKE_PROJECT_VERSION);
    return 0;
}

int main(int argc, char** argv)
{
    int opt;
    bool verbose = false;
    bool debug = false;
    char* logfile = NULL;
    char* loadfile = NULL;
    while ((opt = getopt_long(argc, argv, "hVvldl:L:", g_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            return Bank_print_help(argv, 0);
        case 'V':
            return Bank_print_version();
        case 'v':
            verbose = true;
            break;
        case 'd':
            debug = true;
            break;
        case 'l':
            logfile = optarg;
            break;
        case 'L':
            loadfile = optarg;
            break;
        default:
            return Bank_print_help(argv, 1);
        }
    }

    State state;
    State_init(&state);

    if (loadfile != NULL)
    {
        FILE* fp = fopen(loadfile, "rb");
        if (fp == NULL)
        {
            fprintf(stderr, "Failed to open %s for reading.\n", loadfile);
            return 1;
        }
        if (!State_load(&state, fp))
        {
            fprintf(stderr, "Failed to load %s.\n", loadfile);
            return 1;
        }
    }

    bool run = true;
    while (run)
    {
        printf("\n");
        printf("1. Create account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. List accounts\n");
        printf("6. List transactions\n");
        printf("7. Save\n");
        printf("8. Quit\n");
        printf("\n");
        printf("Enter your choice: ");
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            fprintf(stderr, "Failed to read choice.\n");
            return 1;
        }
        switch (choice)
        {
        case 1: {
            printf("Enter account name: ");
            char name[256];
            if (scanf("%255[^\n]%*c", name) != 1)
            {
                fprintf(stderr, "Failed to read account name.\n");
                return 1;
            }
            Account account;
            Account_init(&account);
            String_init(&account.name);
            account.name.size = strlen(name);
            account.name.data = malloc(account.name.size + 1);
            strcpy(account.name.data, name);
            account.id = state.accounts.size;
            account.balance = 0;
            printf("Your account ID is %ld. Don't forget it!\n", account.id);
            AccountVector_push(&state.accounts, account);
            break;
        }
        case 2: {
            printf("Enter account id: ");
            int id;
            if (scanf("%d", &id) != 1)
            {
                fprintf(stderr, "Failed to read account id.\n");
                return 1;
            }
            printf("Enter amount in cents: ");
            Cents amount;
            if (scanf("%ld", &amount) != 1)
            {
                fprintf(stderr, "Failed to read amount.\n");
                return 1;
            }
            Account* account = State_find_account(&state, id);
            if (account == NULL)
            {
                fprintf(stderr, "Failed to find account.\n");
                return 1;
            }
            account->balance += amount;
            Transaction transaction;
            Transaction_init(&transaction);
            transaction.amount = amount;
            transaction.account_id = id;
            transaction.date = Date_get_today();
            TransactionVector_push(&state.transaction_history, transaction);
            break;
        }
        case 3: {
            printf("Enter account id: ");
            int id;
            if (scanf("%d", &id) != 1)
            {
                fprintf(stderr, "Failed to read account id.\n");
                return 1;
            }
            printf("Enter amount in cents: ");
            Cents amount;
            if (scanf("%ld", &amount) != 1)
            {
                fprintf(stderr, "Failed to read amount.\n");
                return 1;
            }
            Account* account = State_find_account(&state, id);
            if (account == NULL)
            {
                fprintf(stderr, "Failed to find account.\n");
                return 1;
            }
            if (account->balance < amount)
            {
                fprintf(stderr, "Insufficient funds.\n");
                return 1;
            }
            account->balance -= amount;
            Transaction transaction;
            Transaction_init(&transaction);
            transaction.amount = amount;
            transaction.account_id = id;
            transaction.date = Date_get_today();
            TransactionVector_push(&state.transaction_history, transaction);
            break;
        }
        case 4: {
            printf("Enter source account id: ");
            int source_id;
            if (scanf("%d", &source_id) != 1)
            {
                fprintf(stderr, "Failed to read source account id.\n");
                return 1;
            }
            printf("Enter destination account id: ");
            int destination_id;
            if (scanf("%d", &destination_id) != 1)
            {
                fprintf(stderr, "Failed to read destination account id.\n");
                return 1;
            }
            printf("Enter amount: ");
            int amount;
            if (scanf("%d", &amount) != 1)
            {
                fprintf(stderr, "Failed to read amount.\n");
                return 1;
            }
            Account* source_account = State_find_account(&state, source_id);
            if (source_account == NULL)
            {
                fprintf(stderr, "Failed to find source account.\n");
                return 1;
            }
            Account* destination_account = State_find_account(&state, destination_id);
            if (destination_account == NULL)
            {
                fprintf(stderr, "Failed to find destination account.\n");
                return 1;
            }
            if (source_account->balance < amount)
            {
                fprintf(stderr, "Insufficient funds.\n");
                return 1;
            }
            source_account->balance -= amount;
            destination_account->balance += amount;
            Transaction transaction;
            Transaction_init(&transaction);
            transaction.amount = amount;
            transaction.account_id = source_id;
            transaction.date = Date_get_today();
            TransactionVector_push(&state.transaction_history, transaction);
            break;
        }
        case 5: {
            printf("Accounts:\n");
            for (size_t i = 0; i < state.accounts.size; ++i)
            {
                Account* account = &state.accounts.data[i];
                printf("%ld: %*s\n", account->id, (int)account->name.size, account->name.data);
            }
            break;
        }
        case 6: {
            printf("Transactions:\n");
            for (size_t i = 0; i < state.transaction_history.size; ++i)
            {
                Transaction* transaction = &state.transaction_history.data[i];
                printf("%ld: $%ld.%ld\n", transaction->account_id, transaction->amount / 100,
                       transaction->amount % 100);
            }
            break;
        }
        case 7: {
            printf("Saving...\n");

            FILE* savefp = fopen("bank.dat", "wb");
            if (savefp == NULL)
            {
                fprintf(stderr, "Failed to open bank.dat for writing.\n");
                return 1;
            }
            State_save(&state, savefp);
            printf("Saved.\n");
            break;
        }
        case 8: {
            printf("Quitting...\n");
            return 0;
        }
        }
    }

    return 0;
}
