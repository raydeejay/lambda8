/**************************************
 ** HOOKS
 *************************************/

#include "lambda8.h"
#include "aria.h"

ar_Value *f_spr(ar_State *S, ar_Value *args) {
    int i = (int) ar_check_number(S, ar_car(args));
    SDL_RenderCopy( gRenderer, gSprites[i], NULL, NULL );
    return NULL;
}

ar_Value *f_pix(ar_State *S, ar_Value *args) {
    double a = ar_check_number(S, ar_car(args));
    double b = ar_check_number(S, ar_nth(args, 1));

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawPoint(gRenderer, a, b);
    return NULL;
}

ar_Value *f_rect(ar_State *S, ar_Value *args) {
    double a = ar_check_number(S, ar_car(args));
    double b = ar_check_number(S, ar_nth(args, 1));
    double c = ar_check_number(S, ar_nth(args, 2));
    double d = ar_check_number(S, ar_nth(args, 3));

    SDL_Rect dstrect;

    dstrect.x = a;
    dstrect.y = b;
    dstrect.w = c;
    dstrect.h = d;

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(gRenderer, &dstrect);
    return NULL;
}

ar_Value *f_line(ar_State *S, ar_Value *args) {
    double a = ar_check_number(S, ar_car(args));
    double b = ar_check_number(S, ar_nth(args, 1));
    double c = ar_check_number(S, ar_nth(args, 2));
    double d = ar_check_number(S, ar_nth(args, 3));

    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(gRenderer, a, b, c, d);
    return NULL;
}

ar_Value *f_cls(ar_State *S, ar_Value *args) {
    SDL_RenderClear( gRenderer );
    return NULL;
}

ar_Value *f_define_sprite(ar_State *S, ar_Value *args) {
    char *id = ar_check(S, ar_car(args), AR_TSTRING)->u.str.s;
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

    return success ? ar_new_number(S, gMaxSprite) : NULL;
}

ar_Value *f_define_sfx(ar_State *S, ar_Value *args) {
    char *id = ar_check(S, ar_car(args), AR_TSTRING)->u.str.s;
    int success = 1;

    ++gMaxSfx;
    printf("loading SFX %s\n", id);

    gSfx[gMaxSfx] = Mix_LoadWAV(id);
    if (gSfx[gMaxSfx] == NULL) {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
        success = 0;
    }

    return success ? ar_new_number(S, gMaxSfx) : NULL;
}

ar_Value *f_sfx(ar_State *S, ar_Value *args) {
    int i = (int) ar_check_number(S, ar_car(args));
    Mix_PlayChannel(-1, gSfx[i], 0);
    return NULL;
}

ar_Value *f_printxy(ar_State *S, ar_Value *args) {
    size_t len;
    const char *str = ar_to_stringl(S, ar_car(args), &len);
    double x = ar_check_number(S, ar_nth(args, 1));
    double y = ar_check_number(S, ar_nth(args, 2));
    printText(str, x, y);

    return NULL;
}
