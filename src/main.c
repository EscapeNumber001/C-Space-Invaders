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
#define FPS		30

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
  int lastFrameTicks;
};

static struct SDLGameContext sdlGameCtx;

static struct SpriteManager sm;
static struct EntityManager em;

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  EntityManager_Init(&em);
  SpriteManager_Init(&sm, sdlGameCtx.renderer);

  struct Sprite* sprite = SpriteManager_CreateSprite(&sm, "assets/animationtest.bmp", 64);
  struct Entity* p = EntityManager_CreateEntity(&em);
  p->sprite = sprite;
  
  while (!SDL_QuitRequested())
  {
    if (SDL_GetTicks() - sdlGameCtx.lastFrameTicks < 1000 / FPS)
    {
      continue;
    }

    sdlGameCtx.lastFrameTicks = SDL_GetTicks();

    SDL_PumpEvents(); 
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_W])
      p->position.y -= 1;
    if (keyState[SDL_SCANCODE_S])
      p->position.y += 1;
    if (keyState[SDL_SCANCODE_A])
      p->position.x -= 1;
    if (keyState[SDL_SCANCODE_D])
      p->position.x += 1;

    /*struct Entity* ent = em.first_ent;
    while (ent)
    {
      (ent->onUpdate)(ent);
      ent = ent->next;
    }*/

    SpriteManager_AnimateSprites(&sm);
    SDL_SetRenderDrawColor(sdlGameCtx.renderer, 25, 25, 25, 25);
    SDL_RenderClear(sdlGameCtx.renderer);
    struct Entity* ent = em.first_ent;
    while (ent)
    {
      SDL_Rect screenRenderPos = (SDL_Rect){ent->position.x, ent->position.y, 100, 100};
      SDL_RenderCopy(sdlGameCtx.renderer, ent->sprite->texture, &ent->sprite->spritesheetCropRect, &screenRenderPos);

      ent = ent->next;
    }

    SDL_RenderPresent(sdlGameCtx.renderer);
  }
  return 0;
}
