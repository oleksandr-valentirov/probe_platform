#include "timers.h"


typedef struct{
    void (*systick_callback)(void *params);
    void *params;
} Callback;

