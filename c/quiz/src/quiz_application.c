#include "quiz_application.h"
#include <stdio.h>

void QuizApplication_write_file(const char* filename, ByteArray data)
{
    FILE* file = fopen(filename, "wb");
    fwrite(data.data, sizeof(uint8_t), data.size, file);
    fclose(file);
}

int main(void)
{
    return 0;
}
