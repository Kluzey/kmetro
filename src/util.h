#define CURSOR_OFF "\33[?25l"
#define CURSOR_ON "\33[?25h"
#define BOLD_ON "\033[1m"
#define BOLD_OFF "\033[m"

typedef struct {
    float numerator, denominator;
} fraction;

typedef struct {
    int bpm;
    fraction timeSig;
} globalConfig;

void die(const char *message);
globalConfig parseArguements(int argc, char *argv[]);
