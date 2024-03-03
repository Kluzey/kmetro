#include <AL/alc.h>
#include <AL/alut.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"
#include "audio.h"
#include "beat.h"

#define METRONOME_HIGH_PATH ""
#define METRONOME_LOW_PATH  ""

static int run;

static void signalHandler(int signal);
void cleanup(void);

void signalHandler(int signal)
{
    if(signal == SIGINT)
        run = 0;
}

void cleanup(void)
{
    puts(CURSOR_ON);
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
    alutExit();
}

int main(int argc, char *argv[])
{
    globalConfig config = parseArguements(argc, argv);
    
    signal(SIGINT, signalHandler);

    initAudio();
    
    ALuint metroHigh;
    ALuint metroLow;
    loadSourceFile(METRONOME_HIGH_PATH, &metroHigh);
    loadSourceFile(METRONOME_LOW_PATH, &metroLow);

    printf(CURSOR_OFF "bpm: %i \t %.0f/%.0f\n", config.bpm, config.timeSig.numerator, config.timeSig.denominator);

    float fakebpm;
    fakebpm = config.timeSig.denominator / 4 * config.bpm;

    int delay = convertBpmToMicroSeconds(fakebpm); 

    run = 1;
    while(run) {
        if(printBeats(config.timeSig) == 2)
            playWavFile(METRONOME_HIGH_PATH, metroHigh);
        else
            playWavFile(METRONOME_LOW_PATH, metroLow);
        usleep(delay);
    }

    cleanup();
    return EXIT_SUCCESS;
}
