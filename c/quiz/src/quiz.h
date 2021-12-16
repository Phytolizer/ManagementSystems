#pragma once

#include "question.h"
#include <stdint.h>

typedef struct
{
    QuestionVector round_1;
    QuestionVector round_2;
    int64_t prize;
} Quiz;

void Quiz_init(Quiz* quiz);
void Quiz_deinit(Quiz* quiz);
