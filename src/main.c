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

#define WIDTH   	640
#define HEIGHT  	480

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
};

static struct SDLGameContext sdlGameCtx;

static struct EntityManager em;

static struct Entity* ents[ENTITY_LIST_SIZE];

int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  sdlGameCtx.win = SDL_CreateWindow("Game", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
  sdlGameCtx.renderer = SDL_CreateRenderer(sdlGameCtx.win, -1, SDL_RENDERER_ACCELERATED);

  em.first_ent = NULL;
  em.current_ent = NULL;
  em.new_ent = NULL;
  EntityManager_CreateEntity(&em);
  struct Entity* test = EntityManager_CreateEntity(&em);
  EntityManager_CreateEntity(&em);

  EntityManager_RemoveEntity(&em, test);
  free(test);

  SDL_Surface* tmpSurf = SDL_LoadBMP("assets/player.bmp");
  SDL_Texture* entTexture = SDL_CreateTextureFromSurface(sdlGameCtx.renderer, tmpSurf);
  free(tmpSurf);
  
  while (!SDL_QuitRequested())
  {
    struct Entity* ent = em.first_ent;
    while (ent)
    {
      ent = ent->next;
    }

    SDL_RenderPresent(sdlGameCtx.renderer);
  }
  return 0;
}
