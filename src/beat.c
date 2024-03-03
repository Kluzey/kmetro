#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "util.h"

int convertBpmToMicroSeconds(int bpm)
{
    return 60000000/bpm;
}

int getTermRows(void)
{
    struct winsize termSize;

    if( (ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize)) == -1)
        die("ioctl failed");

    return termSize.ws_row;
}

int printBeats(fraction timeSig)
{
    if(timeSig.numerator > getTermRows())
        die("Numberator number exceeded limit");

    static unsigned int currentBeat = 1;

    putchar('\r');

    for(unsigned int i = 1; i <= timeSig.numerator; ++i) {
        if(i == currentBeat) {
            printf(BOLD_ON "%d " BOLD_OFF, i);
            continue;
        }
        printf("%d ", i);
    }

    if((int)currentBeat == timeSig.numerator)
        currentBeat = 0;
    currentBeat++;

    fflush(stdout);

    return currentBeat;
}
