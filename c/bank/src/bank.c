#include "bank/bank.h"

#include <bank.capnp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "bank/de.h"

#define LIST_TYPE BankAccount
#define LIST_NAME BankAccount
#define LIST_IMPL
#include "bank/list.h"

void FreeBank(Bank* bank) {
  FreeBankAccountList(&bank->accounts);
}

int main(void) {
  Bank bank = {0};
  if (!DeserializeBank(&bank, "bank.bin")) {
    printf("Could not read bank.bin, starting from scratch\n");
  }
}
