#include "quiz_application.h"
#include "define.h"
#include "deserialize.h"
#include "quiz.h"
#include "serialize.h"
#include <stdio.h>

ByteArray QuizApplication_load_file(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
    {
        return (ByteArray){0};
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    ByteArray result = {
        .data = malloc(size),
        .size = size,
    };
    fread(result.data, 1, size, file);
    fclose(file);
    return result;
}

void QuizApplication_write_file(const char* filename, ByteArray data)
{
    FILE* file = fopen(filename, "wb");
    fwrite(data.data, sizeof(uint8_t), data.size, file);
    fclose(file);
}

int main(void)
{
    Quiz q;
    if (!Quiz_try_load(&q, "quiz.dat"))
    {
        Quiz_load_default(&q);
    }
    Quiz_serialize(&q, "quiz.dat");
    Quiz_deinit(&q);
    return 0;
}
