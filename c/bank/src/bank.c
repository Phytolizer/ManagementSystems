#include "bank/bank.h"

#include <bank.capnp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define LIST_TYPE BankAccount
#define LIST_NAME BankAccount
#define LIST_IMPL
#include "bank/list.h"

#define TEXT(chars) \
  { sizeof chars - 1, chars, NULL }

void FreeBank(Bank* bank) {
  FreeBankAccountList(&bank->accounts);
}

void ReadTestProtoBank(void) {
  FILE* fp = fopen("bank.bin", "rb");
  struct stat statbuf;
  int fd = fileno(fp);
  if (fstat(fd, &statbuf) < 0) {
    fprintf(stderr, "fstat failed\n");
    exit(1);
  }
  uint8_t* buf = malloc(statbuf.st_size);
  fread(buf, statbuf.st_size, 1, fp);
  fclose(fp);
  struct capn c;
  if (capn_init_mem(&c, buf, statbuf.st_size, 0) != 0) {
    fprintf(stderr, "capn_init_mem failed\n");
    exit(1);
  }
  ProtoBank_ptr bp;
  bp.p = capn_getp(capn_root(&c), 0, 1);
  struct ProtoBank bank;
  read_ProtoBank(&bank, bp);

  printf("bank.accounts.len = %d\n", capn_len(bank.accounts));

  struct ProtoBank_Account acc;
  get_ProtoBank_Account(&acc, bank.accounts, 0);
  printf("bank.accounts[0].name = %.*s\n", acc.name.len, acc.name.str);

  free(buf);
  capn_free(&c);
}

int main(void) {
  ReadTestProtoBank();
}
