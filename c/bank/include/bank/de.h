#pragma once

#include <bank.capnp.h>

#include "bank/bank.h"

bool DeserializeBank(Bank* bank, const char* filename);
