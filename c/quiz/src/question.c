#include "question.h"
#include "management/array.h"
#include "management/vector.h"

ARRAY_IMPLEMENTATION(String, char, ARRAY_DESTRUCTOR_NONE(String));
VECTOR_IMPLEMENTATION(QuestionVector, Question, VECTOR_DESTRUCTOR(QuestionVector, Question_deinit));

void Question_init(Question* question)
{
    String_init(&question->question);
    for (int i = 0; i < 4; ++i)
    {
        String_init(&question->options[i]);
    }
    question->correct_answer = 0;
}

void Question_deinit(Question* question)
{
    String_deinit(&question->question);
    for (int i = 0; i < 4; ++i)
    {
        String_deinit(&question->options[i]);
    }
}
