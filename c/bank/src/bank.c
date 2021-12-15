#include "config.h"
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static const struct option options[] = {
    {"help", no_argument, NULL, 'h'},  {"version", no_argument, NULL, 'V'},   {"verbose", no_argument, NULL, 'v'},
    {"debug", no_argument, NULL, 'd'}, {"log", required_argument, NULL, 'l'}, {"load", required_argument, NULL, 'L'},
};

int print_help(char** argv, int return_code)
{
    printf("%s\n", CMAKE_PROJECT_NAME);
    printf("Usage: %s [OPTION]...\n", argv[0]);
    printf("\n");
    printf("  -h, --help            display this help and exit\n");
    printf("  -V, --version         output version information and exit\n");
    printf("  -v, --verbose         verbose output\n");
    printf("  -d, --debug           debug output\n");
    printf("  -l, --log=FILE        log to FILE\n");
    printf("  -L, --load=FILE       load FILE\n");
    printf("\n");
    printf("Report bugs to %s <%s>.\n", CMAKE_AUTHOR_NAME, CMAKE_AUTHOR_EMAIL);
    return return_code;
}

int print_version(void)
{
    printf("%s v%s\n", CMAKE_PROJECT_NAME, CMAKE_PROJECT_VERSION);
    return 0;
}

int main(int argc, char** argv)
{
    int opt;
    bool verbose = false;
    bool debug = false;
    char* logfile = NULL;
    char* loadfile = NULL;
    while ((opt = getopt_long(argc, argv, "hVvldl:L:", options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            return print_help(argv, 0);
        case 'V':
            return print_version();
        case 'v':
            verbose = true;
            break;
        case 'd':
            debug = true;
            break;
        case 'l':
            logfile = optarg;
            break;
        case 'L':
            loadfile = optarg;
            break;
        default:
            return print_help(argv, 1);
        }
    }
    return 0;
}
