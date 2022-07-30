#include <bank.capnp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define TEXT(chars) \
  { sizeof chars - 1, chars, NULL }

void writeTestBank(void) {
  struct capn c;
  capn_init_malloc(&c);
  capn_ptr cr = capn_root(&c);
  struct capn_segment* cs = cr.seg;
  struct Bank bank = {0};
  bank.accounts = new_Bank_Account_list(cs, 1);
  struct Bank_Account acc = {
      .address = TEXT("123 Main St"),
      .age = 32,
      .amt = 1600,
      .citizenship = TEXT("US"),
      .deposit = new_Bank_Date(cs),
      .dob = new_Bank_Date(cs),
      .name = TEXT("John Doe"),
      .no = 0,
      .phone = TEXT("5555555555"),
      .type = Bank_Account_Type_savings,
  };
  struct Bank_Date deposit = {
      .day = 1,
      .month = 1,
      .year = 2018,
  };
  write_Bank_Date(&deposit, acc.deposit);
  struct Bank_Date dob = {
      .day = 26,
      .month = 4,
      .year = 1999,
  };
  write_Bank_Date(&dob, acc.dob);
  set_Bank_Account(&acc, bank.accounts, 0);
  Bank_ptr bp = new_Bank(cs);
  write_Bank(&bank, bp);
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

void readTestBank(void) {
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
  Bank_ptr bp;
  bp.p = capn_getp(capn_root(&c), 0, 1);
  struct Bank bank;
  read_Bank(&bank, bp);

  printf("bank.accounts.len = %d\n", capn_len(bank.accounts));

  struct Bank_Account acc;
  get_Bank_Account(&acc, bank.accounts, 0);
  printf("bank.accounts[0].name = %.*s\n", acc.name.len, acc.name.str);

  free(buf);
  capn_free(&c);
}

int main(void) {
  writeTestBank();
  readTestBank();
}
