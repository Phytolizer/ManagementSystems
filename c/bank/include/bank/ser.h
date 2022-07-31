#pragma once

#include <bank.capnp.h>

#include "bank/bank.h"

void SerializeBank(Bank* bank, const char* filename);
