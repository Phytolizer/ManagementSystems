#include "today.h"
#include <time.h>

Date Date_get_today(void)
{
    Date today;
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    today.year = tm->tm_year + 1900;
    today.month = tm->tm_mon + 1;
    today.day = tm->tm_mday;
    return today;
}
