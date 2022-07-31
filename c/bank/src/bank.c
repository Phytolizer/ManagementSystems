#include "bank/bank.h"

#include <bank.capnp.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bank/de.h"
#include "bank/ser.h"

#define LIST_TYPE BankAccount
#define LIST_NAME BankAccount
#define LIST_IMPL
#include "bank/list.h"

static String StringFromCString(const char* cstr, size_t len) {
  String str;
  InitString(&str);
  ResizeString(&str, len);
  memcpy(str.arr, cstr, len);
  return str;
}

static double ComputeInterest(double t, double amount, int rate) {
  return (rate * t * amount) / 100.0;
}

static void PrintCentered(const char* text) {
  // get width of terminal
  struct winsize w;
  int ioctl_ret = ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  if (ioctl_ret == -1) {
    perror("ioctl");
    exit(1);
  }
  if (w.ws_col == 0) {
    w.ws_col = 80;
  }
  size_t len = strlen(text);
  size_t pad = (w.ws_col - len) / 2;
  for (size_t i = 0; i < pad; i++) {
    putchar(' ');
  }
  printf("%s\n", text);
}

static int CreateAccount(Bank* bank) {
  printf("\x1b[2J\x1b[H");
  PrintCentered("=== ADD RECORD ===");
  printf("\n\n\n");
  BankAccount add = {0};
  bool valid = false;
  while (!valid) {
    printf("Enter today's date (YYYY-MM-DD): ");
    // space for YYYY-MM-DD + 1 for newline + 1 for NUL
    char date[12];
    if (fgets(date, 12, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    Date deposit;
    if (sscanf(date, "%" SCNu32 "-%" SCNu8 "-%" SCNu8, &deposit.year,
               &deposit.month, &deposit.day) != 3) {
      fprintf(stderr, "Invalid date: '%s'\n", date);
      continue;
    }
    valid = true;
    add.deposit = deposit;
  }
  valid = false;
  while (!valid) {
    printf("Enter account number: ");
    // 20 for UINT64_MAX + 1 for newline + 1 for NUL
    char no_str[22];
    if (fgets(no_str, 22, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    uint64_t no;
    if (sscanf(no_str, "%" SCNu64, &no) != 1) {
      fprintf(stderr, "Invalid account number: %s\n", no_str);
      exit(1);
    }
    valid = true;
    for (size_t i = 0; i < BankAccountListSize(&bank->accounts); i++) {
      if (BankAccountListGet(&bank->accounts, i)->no == no) {
        fprintf(stderr, "Account number already exists: %" PRIu64 "\n", no);
        valid = false;
        break;
      }
    }
    if (!valid) {
      continue;
    }
    add.no = no;
  }
  valid = false;
  while (!valid) {
    printf("Enter name: ");
    // space for name + 1 for NUL
    char name[257];
    if (fgets(name, 257, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    size_t len = strlen(name);
    if (len > 0 && name[len - 1] == '\n') {
      name[len - 1] = '\0';
      --len;
    }
    if (len == 0) {
      fprintf(stderr, "Name cannot be empty\n");
      continue;
    }
    valid = true;
    add.name = StringFromCString(name, len);
  }
  valid = false;
  while (!valid) {
    printf("Enter the date of birth (YYYY-MM-DD): ");
    // space for YYYY-MM-DD and newline
    char date[12];
    if (fgets(date, 12, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    Date dob;
    if (sscanf(date, "%" SCNu32 "-%" SCNu8 "-%" SCNu8, &dob.year, &dob.month,
               &dob.day) != 3) {
      fprintf(stderr, "Invalid date: %s\n", date);
      continue;
    }
    valid = true;
    add.dob = dob;
  }
  valid = false;
  while (!valid) {
    printf("Enter the age: ");
    // space for age + 1 for NUL
    char age_str[11];
    if (fgets(age_str, 11, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    uint32_t age;
    if (sscanf(age_str, "%" SCNu32, &age) != 1) {
      fprintf(stderr, "Invalid age: %s\n", age_str);
      continue;
    }
    valid = true;
    add.age = age;
  }
  valid = false;
  while (!valid) {
    printf("Enter the address: ");
    // space for address + 1 for newline + 1 for NUL
    char address[258];
    if (fgets(address, 258, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    size_t len = strlen(address);
    if (len > 0 && address[len - 1] == '\n') {
      address[len - 1] = '\0';
      --len;
    }
    if (len == 0) {
      fprintf(stderr, "Address cannot be empty\n");
      continue;
    }
    valid = true;
    add.address = StringFromCString(address, len);
  }
  valid = false;
  while (!valid) {
    printf("Enter the citizenship number: ");
    // space for citizenship number + 1 for newline + 1 for NUL
    char citizenship[17];
    if (fgets(citizenship, 17, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    size_t len = strlen(citizenship);
    if (len > 0 && citizenship[len - 1] == '\n') {
      citizenship[len - 1] = '\0';
      --len;
    }
    if (len == 0) {
      fprintf(stderr, "Citizenship number cannot be empty\n");
      continue;
    }
    valid = true;
    add.citizenship = StringFromCString(citizenship, len);
  }
  valid = false;
  while (!valid) {
    printf("Enter the phone number: ");
    // space for (###) ###-#### + 1 for newline + 1 for NUL
    char phone[16];
    if (fgets(phone, 16, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    size_t len = strlen(phone);
    if (len > 0 && phone[len - 1] == '\n') {
      phone[len - 1] = '\0';
      --len;
    }
    if (len == 0) {
      fprintf(stderr, "Phone number cannot be empty\n");
      continue;
    }
    valid = true;
    add.phone = StringFromCString(phone, len);
  }
  valid = false;
  while (!valid) {
    printf("Enter the amount to deposit: ");
    // space for amount + 1 for decimal pt + 1 for newline + 1 for NUL
    char amount_str[23];
    if (fgets(amount_str, 23, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    char* endp = NULL;
    uint64_t amt = strtoull(amount_str, &endp, 10);
    if (endp == amount_str || *endp != '.') {
      fprintf(stderr, "Invalid amount: %s\n", amount_str);
      continue;
    }
    amt *= 100;
    amt += strtoull(endp + 1, &endp, 10);
    if (endp == endp + 1 || *endp != '\n' && *endp != '\0') {
      fprintf(stderr, "Invalid amount: %s\n", amount_str);
      continue;
    }
    valid = true;
    add.amt = amt;
  }
  valid = false;
  while (!valid) {
    printf("Type of account:\n");
    printf(" 1. Savings\n");
    printf(" 2. Current\n");
    printf(" 3. Fixed for 1 year\n");
    printf(" 4. Fixed for 2 years\n");
    printf(" 5. Fixed for 3 years\n");
    printf("Enter your choice: ");
    // space for choice + 1 for newline + 1 for NUL
    char choice_str[3];
    if (fgets(choice_str, 3, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    char* endp = NULL;
    long choice = strtol(choice_str, &endp, 10);
    if (endp == choice_str || *endp != '\n' && *endp != '\0' || choice < 1 ||
        choice > 5) {
      fprintf(stderr, "Invalid choice: %s\n", choice_str);
      continue;
    }
    valid = true;
    add.type = (BankAccountType)(choice - 1);
  }
  BankAccountListAdd(&bank->accounts, add);
  printf("Account created successfully\n");
  while (true) {
    printf("What to do next?\n");
    printf(" 1. Create another account\n");
    printf(" 2. Return to main menu\n");
    printf(" 3. Quit\n");
    printf("Enter your choice: ");
    // space for choice + 1 for newline + 1 for NUL
    char choice_str[3];
    if (fgets(choice_str, 3, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    char* endp = NULL;
    long choice = strtol(choice_str, &endp, 10);
    if (endp == choice_str || *endp != '\n' && *endp != '\0' || choice < 1 ||
        choice > 3) {
      fprintf(stderr, "Invalid choice: %s\n", choice_str);
      continue;
    }
    return choice;
  }
}

int ListAccounts(Bank* bank) {
  printf("Accounts:\n");
  for (size_t i = 0; i < BankAccountListSize(&bank->accounts); ++i) {
    BankAccount* account = BankAccountListGet(&bank->accounts, i);
    printf("#%" PRIu64 ": %" PRIstr ", address: %" PRIstr ", phone: %" PRIstr
           "\n",
           account->no, STRING_ARG(account->name), STRING_ARG(account->address),
           STRING_ARG(account->phone));
  }
  while (true) {
    printf("\n");
    printf("What to do next?\n");
    printf(" 1. Return to main menu\n");
    printf(" 2. Quit\n");
    printf("Enter your choice: ");
    char choice_str[3];
    if (fgets(choice_str, 3, stdin) == NULL) {
      if (errno != 0) {
        perror("fgets");
      } else {
        printf("Goodbye.\n");
      }
      exit(1);
    }
    char* endp = NULL;
    long choice = strtol(choice_str, &endp, 10);
    if (endp == choice_str || *endp != '\n' && *endp != '\0' || choice < 1 ||
        choice > 2) {
      fprintf(stderr, "Invalid choice: %s\n", choice_str);
      continue;
    }
    return choice;
  }
}

void FreeBank(Bank* bank) {
  for (size_t i = 0; i < BankAccountListSize(&bank->accounts); i++) {
    BankAccount* acc = BankAccountListGet(&bank->accounts, i);
    FreeString(&acc->name);
    FreeString(&acc->address);
    FreeString(&acc->citizenship);
    FreeString(&acc->phone);
  }
  FreeBankAccountList(&bank->accounts);
}

void ShowMenu(Bank* bank) {
  while (true) {
    printf("What to do?\n");
    printf(" 1. Create an account\n");
    printf(" 2. List accounts\n");
    printf(" 3. Quit\n");
    printf("Enter your choice: ");
    // space for choice + 1 for newline + 1 for NUL
    char choice_str[3];
    if (fgets(choice_str, 3, stdin) == NULL) {
      perror("fgets");
      exit(1);
    }
    char* endp = NULL;
    long choice = strtol(choice_str, &endp, 10);
    if (endp == choice_str || *endp != '\n' && *endp != '\0' || choice < 1 ||
        choice > 3) {
      fprintf(stderr, "Invalid choice: %s\n", choice_str);
      continue;
    }
    switch (choice) {
      case 1:
        while (true) {
          choice = CreateAccount(bank);
          switch (choice) {
            case 1:
              continue;
            case 2:
              break;
            case 3:
              return;
          }
          break;
        }
        break;
      case 2:
        choice = ListAccounts(bank);
        switch (choice) {
          case 1:
            continue;
          case 2:
            return;
        }
      case 3:
        return;
    }
  }
}

int main(void) {
  Bank bank = {0};
  if (!DeserializeBank(&bank, "bank.bin")) {
    printf("Could not read bank.bin, starting from scratch\n");
  }

  ShowMenu(&bank);

  SerializeBank(&bank, "bank.bin");
  FreeBank(&bank);
}
