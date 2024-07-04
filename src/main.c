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
#include "lua_scripting.h"
#include "texture_manager.h"

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

void BulletUpdate(struct Entity* ent)
{
  ent->position.y -= 5;

  if (ent->position.y < 0)
  {
    ent->_markedForRemoval = true;
    //EntityManager_RemoveEntity(&em, ent);
    //free(ent);
  }
}

void DebugOnIntersect(struct Entity* me, struct Entity* other)
{
  //printf("I've been hit! (%d, %d)\n", me->id, other->id);
}

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  EntityManager_Init(&em);
  SpriteManager_Init(&sm, sdlGameCtx.renderer);
  TextureManager_Init(sdlGameCtx.renderer, &tm);
  //TextureManager_LoadEx(sdlGameCtx.renderer, &tm, "assets/animationtest.bmp", 64, 15, true);
  TextureManager_Load(sdlGameCtx.renderer, &tm, "assets/player.bmp");
  TextureManager_Load(sdlGameCtx.renderer, &tm, "assets/bullet.bmp");

  struct Sprite* sprite = SpriteManager_CreateSprite(&sm, TextureManager_GetTexture(&tm, "assets/player.bmp"));
  struct Sprite* bulletSpr = SpriteManager_CreateSprite(&sm, TextureManager_GetTexture(&tm, "assets/bullet.bmp"));

  struct Entity* p = EntityManager_CreateEntity(&em);
  p->aabbSize = (SDL_Point){100, 100};
  LuaSystem_Init(sdlGameCtx.renderer, &em, &sm, &tm);

  p->sprite = sprite;
  p->onAabbIntersect = DebugOnIntersect;
  
  while (!SDL_QuitRequested())
  {
    if (SDL_GetTicks() - sdlGameCtx.lastFrameTicks < 1000 / FPS > 0)
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
    if (keyState[SDL_SCANCODE_SPACE])
    {
      struct Entity* bullet = EntityManager_CreateEntity(&em);
      bullet->aabbSize = (SDL_Point){100, 100};
      bullet->position = p->position;
      bullet->sprite = bulletSpr;
      bullet->onUpdate = BulletUpdate;
    }

    SpriteManager_AnimateSprites(&sm, FPS);
    SDL_SetRenderDrawColor(sdlGameCtx.renderer, 255, 255, 255, 255);
    SDL_RenderClear(sdlGameCtx.renderer);
    struct Entity* ent = em.first_ent;
    while (ent)
    {
      // Update entities
      if (ent->onUpdate != NULL)
	(ent->onUpdate)(ent);

      
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
	if (SDL_HasIntersection(&a, &b))
	{
	  if (ent->onAabbIntersect != NULL)
	    (ent->onAabbIntersect)(ent, checkCollisionEnt);
	}
	checkCollisionEnt = checkCollisionEnt->next;
      }


      // Rendering
      SDL_Rect screenRenderPos = (SDL_Rect){ent->position.x, ent->position.y, 100, 100};
      if (ent->sprite != NULL)
	SDL_RenderCopy(sdlGameCtx.renderer, ent->sprite->texture->texture, &ent->sprite->spritesheetCropRect, &screenRenderPos);

      
      // Clean up entities which are marked for removal
      struct Entity* oldEnt = ent;
      ent = ent->next;
      if (oldEnt->_markedForRemoval)
      {
	entitymanager_internal_RemoveEntityFromLL(&em, oldEnt);
	free(oldEnt);
      }
    }

    SDL_RenderPresent(sdlGameCtx.renderer);
  }
  return 0;
}
