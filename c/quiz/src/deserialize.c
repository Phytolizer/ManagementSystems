#include "deserialize.h"
#include "define.h"
#include "management/util.h"
#include "quiz_application.h"
#include <quiz/proto/Quiz.pb-c.h>
#include <string.h>

static void Quiz_deserialize(Quiz* q, ByteArray data);

bool Quiz_try_load(Quiz* q, const char* filename)
{
    ByteArray data = QuizApplication_load_file(filename);
    if (data.size == 0)
    {
        return false;
    }
    Quiz_deserialize(q, data);
    ByteArray_deinit(&data);
    return true;
}

static void Quiz_deserialize(Quiz* q, ByteArray data)
{
    Quiz_init(q);
    Quiz__Proto__Quiz* message = quiz__proto__quiz__unpack(NULL, data.size, data.data);
    q->prize = message->prize;
    for (size_t i = 0; i < message->n_questions; ++i)
    {
        Quiz__Proto__Question* incoming_question = message->questions[i];
        Question question;
        Question_init(&question);
        question.question.size = strlen(incoming_question->question);
        question.question.data = management__memdup(incoming_question->question, question.question.size);
        question.correct_answer = incoming_question->correct_answer;
        for (size_t j = 0; j < incoming_question->n_options; ++j)
        {
            if (j == 4)
            {
                break;
            }
            question.options[j].size = strlen(incoming_question->options[j]);
            question.options[j].data = management__memdup(incoming_question->options[j], question.options[j].size);
        }
        switch (incoming_question->round)
        {
        case QUIZ__PROTO__ROUND__ROUND_1:
            QuestionVector_push(&q->round_1, question);
            break;
        case QUIZ__PROTO__ROUND__ROUND_2:
            QuestionVector_push(&q->round_2, question);
            break;
        default:
            Question_deinit(&question);
            break;
        }
    }
    quiz__proto__quiz__free_unpacked(message, NULL);
}
