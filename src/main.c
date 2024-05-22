/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/19/2024 04:52:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
//#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>
#include "entity.h"

#define WIDTH   	640
#define HEIGHT  	480

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
};

static struct SDLGameContext sdlGameCtx;

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  struct Entity e;
  Entity_Init(&e);
  SDL_Surface* tmpSurf = SDL_LoadBMP("assets/player.bmp");
  SDL_Texture* entTexture = SDL_CreateTextureFromSurface(sdlGameCtx.renderer, tmpSurf);
  e.texture = entTexture;
  free(tmpSurf);
  
  while (!SDL_QuitRequested())
  {
    SDL_Rect x = {e.position.x, e.position.y, 64, 64};
    SDL_RenderCopy(sdlGameCtx.renderer, entTexture, NULL, &x);
    SDL_RenderPresent(sdlGameCtx.renderer);
  }
  return 0;
}
