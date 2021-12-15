#include "find.h"

Account* State_find_account(State* state, int64_t id)
{
    for (size_t i = 0; i < state->accounts.size; i++)
    {
        Account* account = &state->accounts.data[i];
        if (account->id == id)
        {
            return account;
        }
    }
    return NULL;
}
