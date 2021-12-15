#include "structure.h"

VECTOR_IMPLEMENTATION(AccountVector, Account, VECTOR_DESTRUCTOR(AccountVector, Account_deinit));
VECTOR_IMPLEMENTATION(TransactionVector, Transaction, VECTOR_DESTRUCTOR_NONE(TransactionVector));

void State_init(State* state)
{
    AccountVector_init(&state->accounts);
    TransactionVector_init(&state->transaction_history);
}

void State_deinit(State *state)
{
    AccountVector_deinit(&state->accounts);
    TransactionVector_deinit(&state->transaction_history);
}
