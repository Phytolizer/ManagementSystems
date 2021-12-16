#pragma once

#include <management/array.h>
#include <stdint.h>

ARRAY_DECLARATION(ByteArray, uint8_t);

ByteArray Quiz_enter_data(void);
ByteArray Quiz_load_default(void);
