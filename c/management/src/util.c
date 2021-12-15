#include "management/util.h"
#include <stdlib.h>
#include <string.h>

char* NS_(strdup)(const char* str)
{
    char* dup = (char*)malloc(strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}

void* NS_(memdup)(const void* mem, size_t size)
{
    void* dup = malloc(size);
    memcpy(dup, mem, size);
    return dup;
}