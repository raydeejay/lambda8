#ifndef L8_API_H_
#define L8_API_H_

#include "s7.h"

//Screen dimension constants
#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 384
#define L8_WIDTH 256
#define L8_HEIGHT 128
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME (1000 / SCREEN_FPS)

extern s7_pointer l8_time(s7_scheme *sc, s7_pointer args);

extern s7_pointer l8_pix(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_spr(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_printxy(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_define_sprite(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_define_sfx(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_sfx(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_rect(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_line(s7_scheme *sc, s7_pointer args);
extern s7_pointer l8_cls(s7_scheme *sc, s7_pointer args);

extern void initMachineLisp(s7_scheme *s7);

#endif // L8_API_H_
