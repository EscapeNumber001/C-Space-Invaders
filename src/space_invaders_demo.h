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

// === Alien settings ===
#define DEMO_NUM_ALIEN_ROWS 			3
#define DEMO_NUM_ALIEN_COLS 			6
#define DEMO_ALIEN_SPACING 			(SDL_Point){80, 80}
#define DEMO_ALIEN_SPRITE_SIZE 			(SDL_Point){30, 30}
#define DEMO_SLOWEST_ALIEN_MOVE_DELAY_MS 	750
#define DEMO_FASTEST_ALIEN_MOVE_DELAY_MS        17
#define DEMO_ALIEN_SHOOT_INTERVAL		500  // (1/n * 100)% chance per frame
#define DEMO_ALIEN_BULLET_MOVE_SPEED	        3
#define DEMO_SCORE_PER_ALIEN			5
#define DEMO_ALIEN_TEXTURE_FILENAME		"assets/enemy2.bmp"

// === Alien data ===
#define DEMO_MAX_ALIENS			(DEMO_NUM_ALIEN_ROWS * DEMO_NUM_ALIEN_COLS)
#define DEMO_MOVE_DIR_RIGHT 		1
#define DEMO_MOVE_DIR_LEFT  		-1


// === Player settings ===
#define DEMO_PLAYER_MOVE_SPEED		3
#define DEMO_PLAYER_SIZE_PX		75
#define DEMO_PLAYER_BULLET_MOVE_SPEED	15
#define DEMO_PLAYER_TEXTURE_FILENAME	"assets/player.bmp"


// === Teams ===
#define DEMO_TEAM_PLAYER	0
#define DEMO_TEAM_ALIEN		1


// === Etc ===
#define DEMO_TEXT_SIZE_PX 	32


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
void Demo_RestartGame();

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
void _demo_explosionUpdate(struct Entity* self, int frameDelta);
void _demo_explodeEntity(struct Entity* self);
void _demo_displayNumber(char* text, SDL_Point position);
void _demo_playerDied(struct Entity* player);
void _demo_checkwin();
void _demo_gameRestarterUpdate(struct Entity* self, int frameDelta);
#endif
