#pragma once

#include "quiz.h"
#include <management/array.h>
#include <stdint.h>

ARRAY_DECLARATION(ByteArray, uint8_t);

ByteArray Quiz_enter_data(Quiz* quiz);
void Quiz_load_default(Quiz* quiz);
