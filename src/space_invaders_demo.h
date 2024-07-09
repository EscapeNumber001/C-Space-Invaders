/*
 * =====================================================================================
 *
 *       Filename:  space_invaders_demo.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/05/2024 12:21:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SPACEINVAD_DEMO_
#define _SPACEINVAD_DEMO_

#include "entity.h"
#include "texture_manager.h"
#include "sprite.h"
#include "globals.h"

#define DEMO_NUM_ALIEN_ROWS 3
#define DEMO_NUM_ALIEN_COLS 6
#define DEMO_MAX_ALIENS	(DEMO_NUM_ALIEN_ROWS * DEMO_NUM_ALIEN_COLS)
#define DEMO_ALIEN_SPRITE_SIZE (SDL_Point){60, 60}
#define DEMO_SLOWEST_ALIEN_MOVE_DELAY_MS 1000
#define DEMO_FASTEST_ALIEN_MOVE_DELAY_MS 250

#define DEMO_MOVE_DIR_RIGHT 1
#define DEMO_MOVE_DIR_LEFT  -1

#define DEMO_TEXT_SIZE_PX 	48

struct DemoSingletons
{
  struct EntityManager* em;
  struct TextureManager* tm;
  struct SpriteManager* sm;
};

struct PlayerCustomData
{
  int cantShootUntilTick;
  int shootCooldownMs;
};

void Demo_Init(SDL_Renderer* renderer, struct EntityManager* em, struct TextureManager* tm, struct SpriteManager* sm);
void Demo_StartGame();

#endif
