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
#include "entity.h"
#include "sprite.h"

#define WIDTH   	640
#define HEIGHT  	480

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
};

static struct SDLGameContext sdlGameCtx;

static struct SpriteManager sm;
static struct EntityManager em;

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  EntityManager_Init(&em);
  SpriteManager_Init(&sm);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface* tmpSurf = SDL_LoadBMP("assets/player.bmp");
  SDL_Texture* entTexture = SDL_CreateTextureFromSurface(sdlGameCtx.renderer, tmpSurf);
  struct Sprite* sprite = SpriteManager_CreateSprite(&sm);
  sprite->texture = entTexture;
  free(tmpSurf);
  
  while (!SDL_QuitRequested())
  {
    SDL_SetRenderDrawColor(sdlGameCtx.renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdlGameCtx.renderer);
    struct Sprite* spr = sm.firstSpr;
    while (spr)
    {
      SDL_Rect x = (SDL_Rect){0, 0, 100, 100};
      SDL_RenderCopy(sdlGameCtx.renderer, spr->texture, NULL, &x);
      //SpriteManager_RemoveSprite(&sm, spr);
      spr = spr->next;
    }


    SDL_RenderPresent(sdlGameCtx.renderer);
  }
  return 0;
}
