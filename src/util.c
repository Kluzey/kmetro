#include <AL/alc.h>
#include <AL/alut.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"
#include "main.h"

void die(const char *message)
{ 
    printf("%s.", message);
    cleanup();
    exit(EXIT_FAILURE);
}

static void printhelp(void)
{
    printf("kmetro, a simple command line metronome.\n\n"
            BOLD_ON "USAGE:" BOLD_OFF " kmetro [OPTIONS]\n\n"
            BOLD_ON "OPTIONS:\n" BOLD_OFF
            "\t-b [number]\t\tset bmp (default: 120)\n"
            "\t-t [number] [number]\tset time signiture (default: 4 4)\n"
            "\t-h\t\t\tprint this help message\n");
    exit(0);
}

globalConfig parseArguements(int argc, char *argv[])
{
    globalConfig config;
    
    // Default Options
    config.bpm = 120;
    config.timeSig.numerator = 4;
    config.timeSig.denominator = 4;

    if(argc == 1)
        return config;

    for(int i = 0; i < argc; ++i) {
        if(argv[i][0] != '-')
            continue;

        char *end;

        switch(argv[i][1]) {
            case 'h':
                printhelp();
            break;
            case 'b':
                ++i;
                if(argv[i] == NULL)
                    die("A number is required for the option '-b'");

                config.bpm = strtol(argv[i], &end, 0);

                if(config.bpm < 0)
                    die("Bpm cannot be negative");
            break;
            case 't':
                ++i;
                if(argv[i] == NULL || argv[i + 1] == NULL)
                    die("Please enter a time signature in [number] [number] format.");
                
                config.timeSig.numerator = strtol(argv[i], &end, 0);
                config.timeSig.denominator = strtol(argv[i + 1], &end, 0);
            break;
        }
    }

    return config;
}
