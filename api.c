/**************************************
 ** HOOKS
 *************************************/

#include <sys/time.h>

#include "s7.h"

#include "lambda8.h"

#include "api.h"

int palette[16][3] = {
    {0x00, 0x00, 0x00}, /* BLACK */
    {0x00, 0x00, 0x7F}, /*  */
    {0x00, 0x7F, 0x00},
    {0x00, 0x7F, 0x7F},
    {0x7F, 0x00, 0x00},
    {0x7F, 0x00, 0x7F},
    {0x7F, 0x7F, 0x00},
    {0x7F, 0x7F, 0x7F}, /* watered white */
    {0x4F, 0x4F, 0x4F}, /* GREY */
    {0x7F, 0x7F, 0xFF}, /*  */
    {0x7F, 0xFF, 0x7F},
    {0x7F, 0xFF, 0xFF},
    {0xFF, 0x7F, 0x7F},
    {0xFF, 0x7F, 0xFF},
    {0xFF, 0xFF, 0x7F}, /* YELLOW */
    {0xFF, 0xFF, 0xFF}
};

s7_pointer l8_time(s7_scheme *sc, s7_pointer args) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return s7_make_integer(sc, tv.tv_usec / 1000);
}

s7_pointer l8_pix(s7_scheme *sc, s7_pointer args) {
    double x = s7_number_to_real(sc, s7_car(args));
    double y = s7_number_to_real(sc, s7_cadr(args));
    int c = (int) s7_number_to_real(sc, s7_caddr(args));

    SDL_SetRenderDrawColor(gRenderer, palette[c][0], palette[c][1], palette[c][2], 255);
    SDL_RenderDrawPoint(gRenderer, x, y);

    return s7_make_integer(sc, 1);
}

s7_pointer l8_printxy(s7_scheme *sc, s7_pointer args) {
    const char *str = s7_string(s7_car(args));
    double x = s7_number_to_real(sc, s7_cadr(args));
    double y = s7_number_to_real(sc, s7_caddr(args));
    printText(str, x, y);

    return s7_make_integer(sc, 1);
}

s7_pointer l8_spr(s7_scheme *sc, s7_pointer args) {
    double i = s7_number_to_real(sc, s7_car(args));
    double x = s7_number_to_real(sc, s7_cadr(args));
    double y = s7_number_to_real(sc, s7_caddr(args));
    double w = s7_number_to_real(sc, s7_cadddr(args));
    double h = s7_number_to_real(sc, s7_car(s7_cddddr(args)));
    SDL_Rect dst = {x, y, w, h};

    SDL_RenderCopy( gRenderer, gSprites[(int) i],
                    NULL,
                    &dst /* NULL */ );

    return s7_make_integer(sc, 1);
}

s7_pointer l8_define_sprite(s7_scheme *sc, s7_pointer args) {
    const char *id = s7_string(s7_car(args));
    int success = 1;

    SDL_Surface *surf = IMG_Load(id);
    if (surf == NULL) {
        printf("Error creating surface for %s\n", id);
        success = 0;
    }

    ++gMaxSprite;

    gSprites[gMaxSprite] = SDL_CreateTextureFromSurface(gRenderer, surf);
    if (gSprites[gMaxSprite] == NULL) {
        printf("Error creating texture for %s\n", id);
        success = 0;
    } else {
        printf("Loaded sprite #%d from %s\n", gMaxSprite, id);
    }

    SDL_FreeSurface(surf);

    return s7_make_integer(sc, success ? gMaxSprite : -1);
}

s7_pointer l8_define_sfx(s7_scheme *sc, s7_pointer args) {
    const char *id = s7_string(s7_car(args));
    int success = 1;

    ++gMaxSfx;
    printf("loading SFX %s\n", id);

    gSfx[gMaxSfx] = Mix_LoadWAV(id);
    if (gSfx[gMaxSfx] == NULL) {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
        success = 0;
    }

    return success ? s7_make_integer(sc, gMaxSfx) : s7_nil(sc);
}

s7_pointer l8_sfx(s7_scheme *sc, s7_pointer args) {
    int i = s7_integer(s7_car(args));
    Mix_PlayChannel(-1, gSfx[i], 0);
    return s7_t(sc);
}

s7_pointer l8_rect(s7_scheme *sc, s7_pointer args) {
    double a = s7_number_to_real(sc, s7_car(args));
    double b = s7_number_to_real(sc, s7_cadr(args));
    double c = s7_number_to_real(sc, s7_caddr(args));
    double d = s7_number_to_real(sc, s7_cadddr(args));
    int n = s7_integer(s7_car(s7_cddddr(args)));

    SDL_Rect dstrect = { a, b, c, d };

    SDL_SetRenderDrawColor(gRenderer, palette[n][0], palette[n][1], palette[n][2], 255);
    SDL_RenderFillRect(gRenderer, &dstrect);
    return s7_t(sc);
}

s7_pointer l8_rectb(s7_scheme *sc, s7_pointer args) {
    double a = s7_number_to_real(sc, s7_car(args));
    double b = s7_number_to_real(sc, s7_cadr(args));
    double c = s7_number_to_real(sc, s7_caddr(args));
    double d = s7_number_to_real(sc, s7_cadddr(args));
    int n = s7_integer(s7_car(s7_cddddr(args)));

    SDL_Rect dstrect = { a, b, c, d };

    SDL_SetRenderDrawColor(gRenderer, palette[n][0], palette[n][1], palette[n][2], 255);
    SDL_RenderDrawRect(gRenderer, &dstrect);
    return s7_t(sc);
}

// this code is replicated in core.lisp for flexibility
// therefore this function is never used, but left here for reference
s7_pointer l8_circb(s7_scheme *sc, s7_pointer args) {
    double x0 = s7_number_to_real(sc, s7_car(args));
    double y0 = s7_number_to_real(sc, s7_cadr(args));
    double radius = s7_number_to_real(sc, s7_caddr(args));
    int n = s7_integer(s7_cadddr(args));

    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - ((int) radius << 1);

    SDL_SetRenderDrawColor(gRenderer, palette[n][0], palette[n][1], palette[n][2], 255);

    while (x >= y)
    {
        const SDL_Point points[8] = {
            {x0 + x, y0 + y},
            {x0 + y, y0 + x},
            {x0 - y, y0 + x},
            {x0 - x, y0 + y},
            {x0 - x, y0 - y},
            {x0 - y, y0 - x},
            {x0 + y, y0 - x},
            {x0 + x, y0 - y}
        };

        SDL_RenderDrawPoints(gRenderer, points, 8);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - ((int) radius << 1);
        }
    }

    return s7_t(sc);
}

// this primitive is rewritten in core.lisp so the rendered lines comply with scaling
// therefore this function is never used, but left here for reference
s7_pointer l8_line(s7_scheme *sc, s7_pointer args) {
    double a = s7_number_to_real(sc, s7_car(args));
    double b = s7_number_to_real(sc, s7_cadr(args));
    double c = s7_number_to_real(sc, s7_caddr(args));
    double d = s7_number_to_real(sc, s7_cadddr(args));
    int n = s7_integer(s7_car(s7_cddddr(args)));

    SDL_SetRenderDrawColor(gRenderer, palette[n][0], palette[n][1], palette[n][2], 255);
    SDL_RenderDrawLine(gRenderer, a, b, c, d);
    return s7_t(sc);
}

s7_pointer l8_cls(s7_scheme *sc, s7_pointer args) {
    int c = s7_integer(s7_car(args));
    SDL_SetRenderDrawColor(gRenderer, palette[c][0], palette[c][1], palette[c][2], 255);
    SDL_RenderClear(gRenderer);
    return s7_t(sc);
}


typedef s7_pointer (*l8_func)(s7_scheme *sc, s7_pointer args);

struct { const char *name; l8_func fn; int nargs; int optargs; bool restargs; const char *doc; } l8_prims[] = {
    { "time",     l8_time,     0, 0, false, "Returns the time in milliseconds (TBI from the start of the FC)" },

    { "define-sprite", l8_define_sprite, 1, 0, false, "(define-sprite filename) Loads an image from filename, makes a texture and registers it, returning it's handle" },
    { "spr",           l8_spr,        5, 0, false, "(spr id x y w h) Blits sprite id at x,y with size w,h" },
    { "pix",           l8_pix,        3, 0, false, "(pix x y c) Sets the pixel at x,y to color c" },

    { "define-sfx",    l8_define_sfx, 1, 0, false, "(define-sfx filename) Loads a sound effect from filename, returning it's handle" },
    { "sfx",           l8_sfx,        1, 0, false, "(sfx id) Plays the sound efect with number id" },

    { "printxy", l8_printxy, 3, 0, false, "(printxy text x y) Prints text at x,y" },
    { "line",           l8_line,        5, 0, false, "(line x y x2 y2) Draws a line from x,y to x2,y2 in color c" },
    { "rect",           l8_rect,        5, 0, false, "(rect x y w h) Draws a filled rectangle at x,y size w,h in color c" },
    { "rectb",          l8_rectb,       5, 0, false, "(rectb x y w h) Draws a rectangle at x,y size w,h in color c" },
    { "circb",          l8_circb,       4, 0, false, "(circb x y w h) Draws a circle centered at x,y radius n in color c" },
    { "cls",     l8_cls,     1, 0, false, "(cls n) Clears the screen using color n" },

    { NULL, NULL, 0, 0, false, NULL }
};

void initMachineLisp(s7_scheme *sc) {
    // initialise the environment
    s7_define_variable(sc, "SCREEN-WIDTH", s7_make_integer(sc, L8_WIDTH));
    s7_define_variable(sc, "SCREEN-HEIGHT", s7_make_integer(sc, L8_HEIGHT));

    // register the functions
    for (int i = 0; l8_prims[i].name; ++i) {
        s7_define_function(sc, l8_prims[i].name, l8_prims[i].fn, l8_prims[i].nargs, l8_prims[i].optargs, l8_prims[i].restargs, l8_prims[i].doc);
    }

    // load the core library
    // notice that it redefines some primitives...
    s7_load(sc, "core.lisp");
}
