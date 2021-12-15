#pragma once

#include <stddef.h>

#define NS_(x) management__##x

char* NS_(strdup)(const char* str);
void* NS_(memdup)(const void* mem, size_t size);
