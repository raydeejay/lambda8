#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Window* gWindow;

extern SDL_Renderer* gRenderer;

extern TTF_Font *gFont;

extern SDL_Texture *gSprites[];
extern int gMaxSprite;

extern Mix_Chunk *gSfx[72];
extern int gMaxSfx;

extern void printText(const char *str, int x, int y);
