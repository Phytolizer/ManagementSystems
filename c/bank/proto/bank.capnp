@0xb2da9d5fcdfcee4f;

struct Bank {
  accounts @0 :List(Account);

  struct Account {
    no @0 :UInt64;
    name @1 :Text;
    dob @2 :Date;
    age @3 :UInt32;
    address @4 :Text;
    citizenship @5 :Text;
    phone @6 :Text;
    type @7 :Type;
    amt @8 :UInt64;
    deposit @9 :Date;

    enum Type {
      savings @0;
      current @1;
      fixed1 @2;
      fixed2 @3;
      fixed3 @4;
    }
  }

  struct Date {
    year @0 :UInt32;
    month @1 :UInt8;
    day @2 :UInt8;
  }
}
