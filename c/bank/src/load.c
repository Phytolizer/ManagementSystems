#include "load.h"
#include <bank/proto/BankState.pb-c.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool State_load(State* state, FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    uint8_t* file_data = malloc(size);
    fread(file_data, size, 1, fp);
    Bank__Proto__State* raw_state = bank__proto__state__unpack(NULL, size, file_data);
    if (raw_state == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < raw_state->n_accounts; ++i)
    {
        Bank__Proto__Account* raw_account = raw_state->accounts[i];
        Account account;
        Account_init(&account);
        String_init(&account.name);
        account.id = raw_account->id;
        account.balance = raw_account->balance;
        account.name.size = strlen(raw_account->name);
        account.name.data = malloc(account.name.size + 1);
        strcpy(account.name.data, raw_account->name);
        AccountVector_push(&state->accounts, account);
    }
    for (size_t i = 0; i < raw_state->n_transaction_history; ++i)
    {
        Bank__Proto__Transaction* raw_transaction = raw_state->transaction_history[i];
        Transaction transaction;
        transaction.amount = raw_transaction->amount;
        transaction.account_id = raw_transaction->account_id;
        transaction.date.year = raw_transaction->date->year;
        transaction.date.month = raw_transaction->date->month;
        transaction.date.day = raw_transaction->date->day;
        TransactionVector_push(&state->transaction_history, transaction);
    }
    bank__proto__state__free_unpacked(raw_state, NULL);
    return true;
}
