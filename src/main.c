#include <AL/alc.h>
#include <AL/alut.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

#include "util.h"
#include "audio.h"
#include "beat.h"
#include "timer.h"

#define METRONOME_HIGH_PATH "/home/kluzey/Documents/Code/Github/kmetro/assets/Perc_Stick_hi.wav"
#define METRONOME_LOW_PATH  "/home/kluzey/Documents/Code/Github/kmetro/assets/Perc_Stick_lo.wav"

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

    long long interval = convertBpmToNanoSeconds(config.timeSig.denominator / 4 * config.bpm); 

    pthread_t timer_thread;
    timerData td;
    td.interval = interval;
    pthread_create(&timer_thread, NULL, timer, &td);

    run = 1;
    atomic_store(&td.ready, false);
    while(run) {
        if(atomic_load(&td.ready)) {
            if(printBeats(config.timeSig) == 2)
                playWavFile(metroHigh);
            else
                playWavFile(metroLow);
            atomic_store(&td.ready, false);
            fflush(stdout);
        }
    }

    pthread_cancel(timer_thread);
    pthread_join(timer_thread, NULL);
    cleanup();
    return EXIT_SUCCESS;
}
