#include "save.h"
#include "management/util.h"
#include <bank/proto/BankState.pb-c.h>
#include <stdint.h>
#include <string.h>

bool State_save(State* state, FILE* fp)
{
    Bank__Proto__State raw_state = BANK__PROTO__STATE__INIT;
    for (size_t i = 0; i < state->accounts.size; ++i)
    {
        raw_state.accounts = realloc(raw_state.accounts, sizeof(Bank__Proto__Account*) * ++raw_state.n_accounts);
        Bank__Proto__Account raw_account = BANK__PROTO__ACCOUNT__INIT;
        Account* account = &state->accounts.data[i];
        raw_account.id = account->id;
        raw_account.balance = account->balance;
        raw_account.name = malloc(account->name.size + 1);
        memcpy(raw_account.name, account->name.data, account->name.size);
        raw_account.name[account->name.size] = '\0';
        raw_state.accounts[raw_state.n_accounts - 1] = management__memdup(&raw_account, sizeof(Bank__Proto__Account));
    }
    for (size_t i = 0; i < state->transaction_history.size; ++i)
    {
        raw_state.transaction_history = realloc(raw_state.transaction_history,
                                                sizeof(Bank__Proto__Transaction*) * ++raw_state.n_transaction_history);
        Bank__Proto__Transaction raw_transaction = BANK__PROTO__TRANSACTION__INIT;
        Transaction* transaction = &state->transaction_history.data[i];
        raw_transaction.amount = transaction->amount;
        raw_transaction.account_id = transaction->account_id;
        Bank__Proto__Date date = BANK__PROTO__DATE__INIT;
        date.year = transaction->date.year;
        date.month = transaction->date.month;
        date.day = transaction->date.day;
        raw_transaction.date = management__memdup(&date, sizeof(Bank__Proto__Date));
        raw_state.transaction_history[raw_state.n_transaction_history - 1] =
            management__memdup(&raw_transaction, sizeof(Bank__Proto__Transaction));
    }
    uint8_t* file_data = malloc(bank__proto__state__get_packed_size(&raw_state));
    bank__proto__state__pack(&raw_state, file_data);
    fwrite(file_data, bank__proto__state__get_packed_size(&raw_state), 1, fp);
    for (size_t i = 0; i < raw_state.n_accounts; ++i)
    {
        free(raw_state.accounts[i]->name);
        free(raw_state.accounts[i]);
    }
    free(raw_state.accounts);
    for (size_t i = 0; i < raw_state.n_transaction_history; ++i)
    {
        free(raw_state.transaction_history[i]->date);
        free(raw_state.transaction_history[i]);
    }
    free(raw_state.transaction_history);
    return true;
}