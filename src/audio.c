#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"
#include "audio.h"

ALCdevice *device;
ALCcontext *context;

void initAudio(void) 
{
    if(alutInitWithoutContext(NULL, NULL) == AL_FALSE)
        die("Failed to init ALUT.");
    if(!(device = alcOpenDevice(NULL)))
        die("Failed to open audio device");
    if(!(context = alcCreateContext(device, NULL)))
        die("Failed to create context");

    alcMakeContextCurrent(context);
}

void loadSourceFile(char *filePath, ALuint *source)
{
    ALuint buffer;
    alGenSources((ALuint)1, source);
    alGenBuffers((ALint)1, &buffer);
    if((buffer = alutCreateBufferFromFile(filePath)) == AL_NONE)
        die("Failed to load audio file");

    alSourcei(*source, AL_BUFFER, buffer);
}

void playWavFile(const char *filePath, ALuint source)
{
    if(access(filePath, F_OK) != 0)
        die("Specified audio file does not exist");

    ALCenum error;

    alSourcePlay(source);

    error = alGetError();
    if(error != AL_NO_ERROR)
        die("OpenAL error occured");
}
