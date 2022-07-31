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

void WriteTestProtoBank(void) {
  struct capn c;
  capn_init_malloc(&c);
  capn_ptr cr = capn_root(&c);
  struct capn_segment* cs = cr.seg;
  struct ProtoBank bank = {0};
  bank.accounts = new_ProtoBank_Account_list(cs, 1);
  struct ProtoBank_Account acc = {
      .address = TEXT("123 Main St"),
      .age = 32,
      .amt = 1600,
      .citizenship = TEXT("US"),
      .deposit = new_ProtoBank_Date(cs),
      .dob = new_ProtoBank_Date(cs),
      .name = TEXT("John Doe"),
      .no = 0,
      .phone = TEXT("5555555555"),
      .type = ProtoBank_Account_Type_savings,
  };
  struct ProtoBank_Date deposit = {
      .day = 1,
      .month = 1,
      .year = 2018,
  };
  write_ProtoBank_Date(&deposit, acc.deposit);
  struct ProtoBank_Date dob = {
      .day = 26,
      .month = 4,
      .year = 1999,
  };
  write_ProtoBank_Date(&dob, acc.dob);
  set_ProtoBank_Account(&acc, bank.accounts, 0);
  ProtoBank_ptr bp = new_ProtoBank(cs);
  write_ProtoBank(&bank, bp);
  int setp_ret = capn_setp(cr, 0, bp.p);
  if (setp_ret) {
    fprintf(stderr, "capn_setp failed: %d\n", setp_ret);
    exit(1);
  }
  int sz = capn_size(&c);
  uint8_t* buf = malloc(sz);
  capn_write_mem(&c, buf, sz, 0);
  capn_free(&c);

  FILE* fp = fopen("bank.bin", "wb");
  fwrite(buf, sz, 1, fp);
  fclose(fp);
  free(buf);
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
  WriteTestProtoBank();
  ReadTestProtoBank();
}
