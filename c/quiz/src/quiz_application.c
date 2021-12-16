#include "quiz_application.h"
#include "define.h"
#include "quiz.h"
#include "serialize.h"
#include <stdio.h>

void QuizApplication_write_file(const char* filename, ByteArray data)
{
    FILE* file = fopen(filename, "wb");
    fwrite(data.data, sizeof(uint8_t), data.size, file);
    fclose(file);
}

int main(void)
{
    Quiz q;
    Quiz_load_default(&q);
    Quiz_serialize(&q, "quiz.dat");
    Quiz_deinit(&q);
    return 0;
}
