#include <AL/al.h>
#include <AL/alc.h>

extern ALCdevice *device;
extern ALCcontext *context;

void initAudio(void);
void loadSourceFile(char* filePath, ALuint *source);
void playWavFile(const char *filePath, ALuint source);
