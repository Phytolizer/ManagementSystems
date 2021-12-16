#include "define.h"
#include "management/array.h"
#include "management/util.h"
#include "question.h"
#include <quiz/proto/Quiz.pb-c.h>

ARRAY_IMPLEMENTATION(ByteArray, uint8_t, ARRAY_DESTRUCTOR_NONE(ByteArray));

typedef struct
{
    const char* question;
    const char* options[4];
    int correct_answer;
} ConstantQuestion;

ConstantQuestion g_first_round[] = {
    {
        "Which of the following is a palindrome number?",
        {
            "42042",
            "101010",
            "23232",
            "01234",
        },
        2,
    },
    {
        "The country with the highest environmental performance index is...",
        {
            "France",
            "Denmark",
            "Switzerland",
            "Finland",
        },
        2,
    },
    {
        "Which animal laughs like a human being?",
        {
            "Polar Bear",
            "Hyena",
            "Donkey",
            "Chimpanzee",
        },
        1,
    },
    {
        "Who was awarded the youngest player award in the FIFA World Cup in 2006?",
        {
            "Wayne Rooney",
            "Lucas Podolski",
            "Lionel Messi",
            "Cristiano Ronaldo",
        },
        2,
    },
    {
        "Which is the third highest mountain in the world?",
        {
            "Mt. K2",
            "Mt. Kanchanjungha",
            "Mt. Makalu",
            "Mt. Kilimanjaro",
        },
        1,
    },
    {
        "What is a group of frogs known as?",
        {
            "A traffic",
            "A toddler",
            "A police",
            "An army",
        },
        3,
    },
};

ConstantQuestion g_second_round[] = {
    {
        "What is the national game of England?",
        {
            "Football",
            "Basketball",
            "Cricket",
            "Baseball",
        },
        2,
    },
    {
        "The study of earthquakes is called...",
        {
            "Seismology",
            "Cosmology",
            "Orology",
            "Etymology",
        },
        0,
    },
    {
        "Among the top 10 highest peaks in the world, how many lie in Nepal?",
        {
            "6",
            "7",
            "8",
            "9",
        },
        2,
    },
    {
        "The Laws of Electromagnetic Induction were given by...",
        {
            "Faraday",
            "Tesla",
            "Maxwell",
            "Coulomb",
        },
        0,
    },
    {
        "In what unit is electric power measured?",
        {
            "Coulombs",
            "Watts",
            "Power",
            "Units",
        },
        1,
    },
    {
        "Which element is found in Vitamin B12?",
        {
            "Zinc",
            "Cobalt",
            "Calcium",
            "Iron",
        },
        1,
    },
    {
        "What is the name of Japan in Japanese?",
        {
            "Polska",
            "Hellas",
            "Drukyul",
            "Nippon",
        },
        3,
    },
    {
        "How many times can a piece of paper be folded at the most?",
        {
            "6",
            "7",
            "8",
            "Depends on the size of the paper",
        },
        1,
    },
    {
        "What is the capital of Denmark?",
        {
            "Copenhagen",
            "Helsinki",
            "Ajax",
            "Galatasaray",
        },
        0,
    },
    {
        "Which is the longest river in the world?",
        {
            "Nile",
            "Koshi",
            "Ganga",
            "Amazon",
        },
        0,
    },
    {
        "What color is the \"black box\" in airplanes?",
        {
            "White",
            "Black",
            "Orange",
            "Red",
        },
        2,
    },
    {
        "Which city is known as \"The City of Seven Hills\"?",
        {
            "Rome",
            "Vatican City",
            "Madrid",
            "Berlin",
        },
        0,
    },
    {
        "Name the country where mosquitoes are not found.",
        {
            "Japan",
            "Italy",
            "Argentina",
            "France",
        },
        3,
    },
    {
        "Who is the author of \"Pulpasa Cafe\"?",
        {
            "Narayan Wagle",
            "Lal Gopal Subedi",
            "B.P. Koirala",
            "Khagendra Sangraula",
        },
        0,
    },
    {
        "Which blood group is known as the \"universal recipient\"?",
        {
            "A",
            "AB",
            "B",
            "O",
        },
        1,
    },
    {
        "What is the unit of measurement of distance between stars?",
        {
            "Light-years",
            "Coulombs",
            "Nautical miles",
            "Kilometers",
        },
        0,
    },
    {
        "The country famous for the Samba dance is...",
        {
            "Brazil",
            "Venezuela",
            "Nigeria",
            "Bolivia",
        },
        0,
    },
    {
        "Wind speed is measured by what?",
        {
            "Lysimeter",
            "Air vanes",
            "Hydrometer",
            "Anemometer",
        },
        3,
    },
    {
        "Which city in the world is popularly known as \"The City of Temple\"?",
        {
            "Delhi",
            "Bhaktapur",
            "Kathmandu",
            "Agra",
        },
        2,
    },
    {
        "Which hardware was used in first-generation computers?",
        {
            "Transistors",
            "Valves",
            "I.C.",
            "S.S.I.",
        },
        1,
    },
    {
        "The ozone layer is being destroyed regularly because of what?",
        {
            "L.P.G.",
            "Nitrogen",
            "Methane",
            "C.F.C.",
        },
        3,
    },
    {
        "Who won the Women's Australian Open Tennis in 2007?",
        {
            "Martina Hingis",
            "Maria Sarapova",
            "Kim Clijster",
            "Serena Williams",
        },
        3,
    },
    {
        "Which film was given the Best Motion Picture award at the Oscars in 2010?",
        {
            "The Secret in Their Eyes",
            "Shutter Island",
            "The King's Speech",
            "The Reader",
        },
        2,
    },
};

ByteArray Quiz_enter_data(void)
{
    Quiz__Proto__Quiz message = QUIZ__PROTO__QUIZ__INIT;

    size_t buffer_size = quiz__proto__quiz__get_packed_size(&message);
    ByteArray result = {
        .data = malloc(buffer_size),
        .size = buffer_size,
    };
    quiz__proto__quiz__pack(&message, result.data);
    return result;
}

ByteArray Quiz_load_default(void)
{
    Quiz__Proto__Quiz message = QUIZ__PROTO__QUIZ__INIT;

    message.n_questions = (sizeof(g_first_round) + sizeof(g_second_round)) / sizeof(ConstantQuestion);
    message.questions = malloc(sizeof(Quiz__Proto__Question*) * message.n_questions);
    for (size_t i = 0; i < sizeof(g_first_round) / sizeof(ConstantQuestion); i++)
    {
        Quiz__Proto__Question question = QUIZ__PROTO__QUESTION__INIT;
        question.question = management__strdup(g_first_round[i].question);
        question.n_options = 4;
        question.options = malloc(sizeof(char*) * question.n_options);
        for (size_t j = 0; j < question.n_options; j++)
        {
            question.options[j] = management__strdup(g_first_round[i].options[j]);
        }
        question.correct_answer = g_first_round[i].correct_answer;
        message.questions[i] = management__memdup(&question, sizeof(Quiz__Proto__Question));
    }
    message.prize = 1000000;

    size_t buffer_size = quiz__proto__quiz__get_packed_size(&message);
    ByteArray result = {
        .data = malloc(buffer_size),
        .size = buffer_size,
    };
    quiz__proto__quiz__pack(&message, result.data);
    return result;
}
