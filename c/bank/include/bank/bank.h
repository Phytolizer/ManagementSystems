#pragma once

#include <stdint.h>

#include "bank/string.h"

typedef struct {
  uint32_t year;
  uint8_t month;
  uint8_t day;
} Date;

#define BANK_ACCOUNT_TYPES_X \
  X(SAVINGS) \
  X(CURRENT) \
  X(FIXED1) \
  X(FIXED2) \
  X(FIXED3)

typedef enum {
#define X(x) BANK_ACCOUNT_TYPE_##x,
  BANK_ACCOUNT_TYPES_X
#undef X
} BankAccountType;

typedef struct {
  uint64_t no;
  String name;
  Date dob;
  uint32_t age;
  String address;
  String citizenship;
  String phone;
  BankAccountType type;
  uint64_t amt;
  Date deposit;
} BankAccount;

#define LIST_TYPE BankAccount
#define LIST_NAME BankAccount
#include "bank/list.h"

typedef struct {
  BankAccountList accounts;
} Bank;
