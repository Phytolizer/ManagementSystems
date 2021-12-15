#pragma once

#include <management/vector.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    int32_t year;
    int32_t month;
    int32_t day;
} Date;

typedef struct
{
    char* data;
    size_t size;
} String;

void String_init(String* string);
void String_deinit(String* string);

typedef int64_t Cents;

typedef struct
{
    String name;
    int64_t id;
    Cents balance;
} Account;

void Account_init(Account* account);
void Account_deinit(Account* account);

typedef struct
{
    Cents amount;
    int64_t account_id;
    Date date;
} Transaction;

void Transaction_init(Transaction* transaction);
/// A no-op. This exists in case Transactions start having more fields.
void Transaction_deinit(Transaction* transaction);

VECTOR_DECLARATION(AccountVector, Account);
VECTOR_DECLARATION(TransactionVector, Transaction);

typedef struct
{
    AccountVector accounts;
    TransactionVector transaction_history;
} State;

void State_init(State* state);
void State_deinit(State* state);
