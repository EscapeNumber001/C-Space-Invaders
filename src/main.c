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
#include "globals.h"
#include "texture_manager.h"
#include "space_invaders_demo.h"

#define PLAYER_SHOOT_DELAY_MS 750;

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
  int lastFrameTicks;
};

struct SDLGameContext sdlGameCtx;

struct SpriteManager sm;
struct EntityManager em;
struct TextureManager tm;

int cantShootUntilTick = 0;

void BulletUpdate(struct Entity* ent, int frameDelta)
{
  ent->position.y -= 15;

  if (ent->position.y < 0)
  {
    Entity_Destroy(&em, ent);
  }
}

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  EntityManager_Init(&em);
  SpriteManager_Init(&sm, sdlGameCtx.renderer);
  TextureManager_Init(sdlGameCtx.renderer, &tm);

  Demo_Init(sdlGameCtx.renderer, &em, &tm, &sm);
  Demo_StartGame();

  while (!SDL_QuitRequested())
  {
    if (SDL_GetTicks() - sdlGameCtx.lastFrameTicks < 1000 / FPS > 0)
    {
      continue;
    }

    SpriteManager_AnimateSprites(&sm, FPS);
    SDL_SetRenderDrawColor(sdlGameCtx.renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdlGameCtx.renderer);
    struct Entity* ent = em.first_ent;
    int i = 0;
    while (ent)
    {
      // Update entities
      if (ent->onUpdate != NULL)
	(ent->onUpdate)(ent, SDL_GetTicks() - sdlGameCtx.lastFrameTicks);

      
      // Check collisions
      struct Entity* checkCollisionEnt = em.first_ent;
      while (checkCollisionEnt)
      {
	if (ent == checkCollisionEnt)
	{
	  checkCollisionEnt = checkCollisionEnt->next;
	  continue;
	}

	SDL_Rect a, b;
	a = Entity_CalculateAABBRect(ent);
	b = Entity_CalculateAABBRect(checkCollisionEnt);
	bool bothExist = !ent->_markedForRemoval && !checkCollisionEnt->_markedForRemoval;
	if (SDL_HasIntersection(&a, &b) && bothExist)
	{
	  if (ent->onAabbIntersect != NULL)
	    (ent->onAabbIntersect)(ent, checkCollisionEnt);
	}
	checkCollisionEnt = checkCollisionEnt->next;
      }


      // Rendering (first pass; background sprites)
      if (ent->sprite != NULL && ent->sprite->isBackgroundSprite)
      {
	SDL_Rect screenRenderPos = (SDL_Rect){ent->position.x, ent->position.y, ent->sprite->spriteScalePx.x, ent->sprite->spriteScalePx.y};
	SDL_RenderCopy(sdlGameCtx.renderer, ent->sprite->texture->texture, &ent->sprite->spritesheetCropRect, &screenRenderPos);
      }

      
      // Clean up entities which are marked for removal
      struct Entity* oldEnt = ent;
      ent = ent->next;
      if (oldEnt->_markedForRemoval)
      {
	if (oldEnt->customData != NULL)
	  free(oldEnt->customData);
	entitymanager_internal_RemoveEntityFromLL(&em, oldEnt);
	free(oldEnt);
      }
    }

    ent = em.first_ent;
    while (ent != NULL)
    {
      // Rendering (second pass; foreground sprites)
      if (ent->sprite != NULL && !ent->sprite->isBackgroundSprite)
      {
	SDL_Rect screenRenderPos = (SDL_Rect){ent->position.x, ent->position.y, ent->sprite->spriteScalePx.x, ent->sprite->spriteScalePx.y};
	SDL_RenderCopy(sdlGameCtx.renderer, ent->sprite->texture->texture, &ent->sprite->spritesheetCropRect, &screenRenderPos);
      }
      ent = ent->next;
    }

    SDL_RenderPresent(sdlGameCtx.renderer);
    sdlGameCtx.lastFrameTicks = SDL_GetTicks();
  }
  return 0;
}
