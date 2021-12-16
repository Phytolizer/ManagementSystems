#pragma once

#include <management/array.h>
#include <management/vector.h>

ARRAY_DECLARATION(String, char);

typedef struct
{
    String question;
    String options[4];
    int correct_answer;
} Question;

void Question_init(Question* question);
void Question_deinit(Question* question);

VECTOR_DECLARATION(QuestionVector, Question);
