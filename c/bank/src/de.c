#include "bank/de.h"

#include <string.h>

static String StringFromCapnText(capn_text text) {
  String str;
  InitString(&str);
  ResizeString(&str, text.len);
  memcpy(str.arr, text.str, text.len);
  return str;
}

bool DeserializeBank(Bank* bank, const char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    return false;
  }
  struct capn c;
  int capn_init_ret = capn_init_fp(&c, fp, 0);
  if (capn_init_ret != 0) {
    return false;
  }
  fclose(fp);
  ProtoBank_ptr pbp;
  capn_ptr cr = capn_root(&c);
  pbp.p = capn_getp(cr, 0, 1);
  struct ProtoBank pb;
  read_ProtoBank(&pb, pbp);

  InitBankAccountList(&bank->accounts);
  ReserveBankAccountList(&bank->accounts, capn_len(pb.accounts));
  for (size_t i = 0; i < capn_len(pb.accounts); i++) {
    struct ProtoBank_Account pb_acc;
    get_ProtoBank_Account(&pb_acc, pb.accounts, i);
    struct ProtoBank_Date deposit;
    read_ProtoBank_Date(&deposit, pb_acc.deposit);
    struct ProtoBank_Date dob;
    read_ProtoBank_Date(&dob, pb_acc.dob);
    BankAccount acc = {
        .no = pb_acc.no,
        .name = StringFromCapnText(pb_acc.name),
        .dob =
            (Date){
                .year = dob.year,
                .month = dob.month,
                .day = dob.day,
            },
        .age = pb_acc.age,
        .address = StringFromCapnText(pb_acc.address),
        .citizenship = StringFromCapnText(pb_acc.citizenship),
        .phone = StringFromCapnText(pb_acc.phone),
        .type = (BankAccountType)pb_acc.type,
        .amt = pb_acc.amt,
        .deposit =
            (Date){
                .year = deposit.year,
                .month = deposit.month,
                .day = deposit.day,
            },
    };
    BankAccountListAdd(&bank->accounts, acc);
  }

  capn_free(&c);
  return true;
}
