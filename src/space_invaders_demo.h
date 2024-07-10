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

#define DEMO_TEAM_PLAYER	0
#define DEMO_TEAM_ALIEN		1

#define DEMO_TEXT_SIZE_PX 	32

#define DEMO_SCORE_PER_ALIEN	5

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

struct BulletCustomData
{
  int team;
  int velocity;
};

void Demo_Init(SDL_Renderer* renderer, struct EntityManager* em, struct TextureManager* tm, struct SpriteManager* sm);
void Demo_StartGame();

void _demo_onAlienHit(struct Entity* self, struct Entity* other);
void _demo_onPlayerHit(struct Entity* self, struct Entity* other);
bool _demo_isAlien(struct Entity* ent);
struct Entity* _demo_getAlienClosestToEdge();
int _demo_lerp(int a, int b, float t);
void _demo_moveAllAliensDown();
int _demo_calculateAlienMoveDelayMs();
void _demo_alienMoveCoordinatorUpdate(struct Entity* self, int frameDelta);
void _demo_bulletUpdate(struct Entity* self, int frameDelta);
void _demo_playerUpdate(struct Entity* self, int frameDelta);
void _demo_alienUpdate(struct Entity* self, int frameDelta);
void _demo_displayNumber(char* text, SDL_Point position);
void _demo_playerDied();
#endif
