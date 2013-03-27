#include "mainstate.h"

#define vpos(h) h*SCREEN_HEIGHT
#define hpos(v) v*SCREEN_WIDTH

#define controller GameEngine::getController()
#define blink(a,t) (sin(t*a) > 0 && controller.isTextCaptureMode() ? "_" : " ")

