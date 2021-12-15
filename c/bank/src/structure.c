#include "structure.h"

VECTOR_IMPLEMENTATION(AccountVector, Account, VECTOR_DESTRUCTOR(AccountVector, Account_deinit));
VECTOR_IMPLEMENTATION(TransactionVector, Transaction, VECTOR_DESTRUCTOR_NONE(TransactionVector));

void String_init(String* string)
{
    string->data = NULL;
    string->size = 0;
}

void String_deinit(String* string)
{
    free(string->data);
}

void Account_init(Account* account)
{
    String_init(&account->name);
    account->id = 0;
    account->balance = 0;
}

void Account_deinit(Account* account)
{
    String_deinit(&account->name);
}

void Transaction_init(Transaction* transaction)
{
    transaction->amount = 0;
    transaction->account_id = 0;
    transaction->date.year = 0;
    transaction->date.month = 0;
    transaction->date.day = 0;
}

void Transaction_deinit(Transaction* transaction)
{
    // Nothing to do.
}

void State_init(State* state)
{
    AccountVector_init(&state->accounts);
    TransactionVector_init(&state->transaction_history);
}

void State_deinit(State* state)
{
    AccountVector_deinit(&state->accounts);
    TransactionVector_deinit(&state->transaction_history);
}
