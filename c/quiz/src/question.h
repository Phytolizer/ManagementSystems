#pragma once

#include <management/array.h>

ARRAY_DECLARATION(String, char);

typedef struct
{
    String question;
    String options[4];
    int correct_answer;
} Question;
