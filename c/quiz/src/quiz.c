#include "quiz.h"

void Quiz_init(Quiz* quiz)
{
    QuestionVector_init(&quiz->round_1);
    QuestionVector_init(&quiz->round_2);
    quiz->prize = 0;
}

void Quiz_deinit(Quiz* quiz)
{
    QuestionVector_deinit(&quiz->round_1);
    QuestionVector_deinit(&quiz->round_2);
}
