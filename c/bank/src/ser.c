#include "bank/ser.h"

static capn_text ToCapnText(String* text) {
  return (capn_text){
      .len = StringSize(text),
      .seg = NULL,
      .str = text->arr,
  };
}

void SerializeBank(Bank* bank, const char* filename) {
  struct capn c;
  capn_init_malloc(&c);
  capn_ptr cr = capn_root(&c);
  struct capn_segment* cs = cr.seg;
  struct ProtoBank pb = {0};
  pb.accounts =
      new_ProtoBank_Account_list(cs, BankAccountListSize(&bank->accounts));
  for (int i = 0; i < BankAccountListSize(&bank->accounts); i++) {
    BankAccount* acc = BankAccountListGet(&bank->accounts, i);
    struct ProtoBank_Account pa = {0};
    pa.address = ToCapnText(&acc->address);
    pa.age = acc->age;
    pa.amt = acc->amt;
    pa.citizenship = ToCapnText(&acc->citizenship);
    pa.deposit = new_ProtoBank_Date(cs);
    pa.dob = new_ProtoBank_Date(cs);
    pa.name = ToCapnText(&acc->name);
    pa.no = acc->no;
    pa.phone = ToCapnText(&acc->phone);
    pa.type = (enum ProtoBank_Account_Type)acc->type;
    struct ProtoBank_Date deposit = {
        .year = acc->deposit.year,
        .month = acc->deposit.month,
        .day = acc->deposit.day,
    };
    write_ProtoBank_Date(&deposit, pa.deposit);
    struct ProtoBank_Date dob = {
        .year = acc->dob.year,
        .month = acc->dob.month,
        .day = acc->dob.day,
    };
    write_ProtoBank_Date(&dob, pa.dob);
    set_ProtoBank_Account(&pa, pb.accounts, i);
  }

  ProtoBank_ptr pbp = new_ProtoBank(cs);
  write_ProtoBank(&pb, pbp);
  int setp_ret = capn_setp(cr, 0, pbp.p);
  if (setp_ret != 0) {
    fprintf(stderr, "capn_setp failed: %d\n", setp_ret);
    exit(1);
  }
  int sz = capn_size(&c);
  uint8_t* buf = malloc(sz);
  capn_write_mem(&c, buf, sz, 0);
  capn_free(&c);

  FILE* fp = fopen(filename, "wb");
  if (fp == NULL) {
    fprintf(stderr, "fopen failed: %s\n", filename);
    exit(1);
  }
  fwrite(buf, sz, 1, fp);
  fclose(fp);

  free(buf);
}
