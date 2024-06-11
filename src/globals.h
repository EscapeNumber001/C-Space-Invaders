/*
 * =====================================================================================
 *
 *       Filename:  globals.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2024 09:43:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SDL2/SDL.h>
#include "entity.h"
#include "sprite.h"
#include "texture_manager.h"

// Display stuff
#define WIDTH   	640
#define HEIGHT  	480
#define FPS		60

struct SDLGameContext
{
  SDL_Window* win;
  SDL_Renderer* renderer;
  int lastFrameTicks;
};

static struct SDLGameContext sdlGameCtx;

static struct SpriteManager sm;
static struct EntityManager em;
static struct TextureManager tm;

#endif
