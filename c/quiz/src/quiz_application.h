#pragma once

#include "define.h"

ByteArray QuizApplication_load_file(const char* filename);
void QuizApplication_write_file(const char* filename, ByteArray data);
