#include "serialize.h"
#include "define.h"
#include "management/util.h"
#include "quiz_application.h"
#include <quiz/proto/Quiz.pb-c.h>

static Quiz__Proto__Question* Question_serialize(Question* q, Quiz__Proto__Round round);

void Quiz_serialize(Quiz* quiz, const char* filename)
{
    Quiz__Proto__Quiz message = QUIZ__PROTO__QUIZ__INIT;
    message.prize = quiz->prize;
    message.n_questions = quiz->round_1.size + quiz->round_2.size;
    message.questions = malloc(sizeof(Quiz__Proto__Question*) * message.n_questions);
    for (size_t i = 0; i < quiz->round_1.size; ++i)
    {
        message.questions[i] = Question_serialize(&quiz->round_1.data[i], QUIZ__PROTO__ROUND__ROUND_1);
    }
    for (size_t i = 0; i < quiz->round_2.size; ++i)
    {
        message.questions[quiz->round_1.size + i] =
            Question_serialize(&quiz->round_2.data[i], QUIZ__PROTO__ROUND__ROUND_2);
    }
    size_t buffer_size = quiz__proto__quiz__get_packed_size(&message);
    ByteArray result = {
        .data = malloc(buffer_size),
        .size = buffer_size,
    };
    quiz__proto__quiz__pack(&message, result.data);
    QuizApplication_write_file(filename, result);

    ByteArray_deinit(&result);
    for (size_t i = 0; i < message.n_questions; ++i)
    {
        quiz__proto__question__free_unpacked(message.questions[i], NULL);
    }
    free(message.questions);
}

static Quiz__Proto__Question* Question_serialize(Question* q, Quiz__Proto__Round round)
{
    Quiz__Proto__Question result = QUIZ__PROTO__QUESTION__INIT;
    result.question = management__memdup(q->question.data, q->question.size);
    result.correct_answer = q->correct_answer;
    result.n_options = 4;
    result.options = malloc(sizeof(char*) * 4);
    for (int i = 0; i < 4; ++i)
    {
        result.options[i] = management__memdup(q->options[i].data, q->options[i].size);
    }
    result.round = round;
    return management__memdup(&result, sizeof(Quiz__Proto__Question));
}
