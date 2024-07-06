/*
 * =====================================================================================
 *
 *       Filename:  space_invaders_demo.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/05/2024 12:21:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "space_invaders_demo.h"

struct DemoSingletons demoSingletons;
int alienMoveDirection;
int msSinceLastMove = 0;
int alienCount = DEMO_MAX_ALIENS;
struct Entity* alienMoveCoordinator;
struct Sprite* bulletSprite;

void _onAlienHit(struct Entity* self, struct Entity* other)
{
  if (strcmp(other->sprite->texture->filename, "assets/bullet.bmp") == 0)
  {
    Entity_Destroy(demoSingletons.em, self);
    Entity_Destroy(demoSingletons.em, other);
    alienCount--;
  }
}


bool isAlien(struct Entity* ent)
{
  if (ent->sprite == NULL)
    return false;

  return (strcmp(ent->sprite->texture->filename, "assets/animationtest.bmp") == 0);
}

struct Entity* getAlienClosestToEdge()
{
  int absoluteClosest = 0xC0FFEE;
  struct Entity* closestEnt = NULL;

  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!isAlien(e))
      goto _continue;

    if (alienMoveDirection == DEMO_MOVE_DIR_RIGHT)
    {
      if (WIDTH - e->position.x < absoluteClosest)
      {
	closestEnt = e;
	absoluteClosest = e->position.x;
      }
    }
    else
    {
      if (e->position.x < absoluteClosest)
      {
	closestEnt = e;
	absoluteClosest = e->position.x;
      }
    }
_continue:
    e = e->next;
  }
  return closestEnt;
}

int lerp(int a, int b, float t) {
    return (int)((1.0f - t) * a + t * b);
}

void moveAllAliensDown()
{
  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!isAlien(e))
      goto _continue;

    e->position.y += DEMO_ALIEN_SPRITE_SIZE.y;

_continue:
    e = e->next;
  }
}

int calculateMoveDelayMs()
{
  // Calculate the percentage of aliens remaining
  float percentageRemaining = (float)alienCount / (float)DEMO_MAX_ALIENS;
    
  // Invert the percentage because we want the delay to decrease as aliens decrease
  float t = 1.0f - percentageRemaining;
    
  // Lerp between the slowest and fastest delay
  return lerp(DEMO_SLOWEST_ALIEN_MOVE_DELAY_MS, DEMO_FASTEST_ALIEN_MOVE_DELAY_MS, t);
}

void coordinateAlienMove(struct Entity* self, int frameDelta)
{
  if (alienCount <= 0)
    return;

  printf("%d\n", calculateMoveDelayMs());
  if (msSinceLastMove < calculateMoveDelayMs())
  {
    msSinceLastMove += frameDelta;
    return;
  }
  
  msSinceLastMove = 0;
  struct Entity* closest = getAlienClosestToEdge();
  if (alienMoveDirection == DEMO_MOVE_DIR_RIGHT)
  {
    if (closest->position.x + DEMO_ALIEN_SPRITE_SIZE.x >= WIDTH)
    {
      alienMoveDirection = -alienMoveDirection;
      moveAllAliensDown();
      return;
    }
  }
  else
  {
    if (closest->position.x - DEMO_ALIEN_SPRITE_SIZE.x < 0)
    {
      alienMoveDirection = -alienMoveDirection;
      moveAllAliensDown();
      return;
    }
  }

  // If we're not at the edge:
  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!isAlien(e))
      goto _continue;
    e->position.x += alienMoveDirection * DEMO_ALIEN_SPRITE_SIZE.x;

_continue:
    e = e->next;
  }
}

void Demo_BulletUpdate(struct Entity* self, int frameDelta)
{
  self->position.y -= 15;

  if (self->position.y < 0)
    Entity_Destroy(demoSingletons.em, self);
}

void Demo_PlayerUpdate(struct Entity* self, int frameDelta)
{
  int* cantShootUntilTick = &((struct PlayerCustomData*)self->customData)->cantShootUntilTick;
  int shootCooldownMs = ((struct PlayerCustomData*)self->customData)->shootCooldownMs;
  SDL_PumpEvents(); 
  const Uint8* keyState = SDL_GetKeyboardState(NULL);
  if (keyState[SDL_SCANCODE_W])
    self->position.y -= 1;
  if (keyState[SDL_SCANCODE_S])
    self->position.y += 1;
  if (keyState[SDL_SCANCODE_A])
    self->position.x -= 1;
  if (keyState[SDL_SCANCODE_D])
    self->position.x += 1;
  if (keyState[SDL_SCANCODE_SPACE] && SDL_GetTicks() > *cantShootUntilTick)
  {
      struct Entity* bullet = EntityManager_CreateEntity(demoSingletons.em);
      bullet->aabbSize = (SDL_Point){25, 100};
      bullet->position = (SDL_Point){self->position.x + 40, self->position.y - 75};
      bullet->sprite = bulletSprite;
      bullet->onUpdate = Demo_BulletUpdate;
      bullet->sprite->spriteScalePx.x = 25;
      *cantShootUntilTick = SDL_GetTicks() + shootCooldownMs;
  }
}

void Demo_Init(SDL_Renderer* renderer, struct EntityManager* em, struct TextureManager* tm, struct SpriteManager* sm)
{
  demoSingletons.em = em;
  demoSingletons.tm = tm;
  demoSingletons.sm = sm;

  TextureManager_Load(renderer, tm, "assets/animationtest.bmp");
  TextureManager_Load(renderer, tm, "assets/player.bmp");
  TextureManager_Load(renderer, tm, "assets/bullet.bmp");

  alienMoveDirection = DEMO_MOVE_DIR_RIGHT;
  bulletSprite = SpriteManager_CreateSprite(sm, TextureManager_GetTexture(tm, "assets/bullet.bmp"));
}

void Demo_StartGame()
{
  alienMoveCoordinator = EntityManager_CreateEntity(demoSingletons.em);
  alienMoveCoordinator->onUpdate = coordinateAlienMove;

  struct Entity* player = EntityManager_CreateEntity(demoSingletons.em);
  struct Sprite* playerSpr = SpriteManager_CreateSprite(demoSingletons.sm, TextureManager_GetTexture(demoSingletons.tm, "assets/player.bmp"));

  player->sprite = playerSpr;
  player->position = (SDL_Point){0, HEIGHT - 100};
  player->aabbSize = (SDL_Point){100, 100};
  player->onUpdate = Demo_PlayerUpdate;
  player->customData = (struct PlayerCustomData*)malloc(sizeof(struct PlayerCustomData));
  ((struct PlayerCustomData*)player->customData)->cantShootUntilTick = 0;
  ((struct PlayerCustomData*)player->customData)->shootCooldownMs = 500;

  for (int y = 0; y < DEMO_NUM_ALIEN_ROWS; y++)
  {
    for (int x = 0; x < DEMO_NUM_ALIEN_COLS; x++)
    {
      struct Entity* alien = EntityManager_CreateEntity(demoSingletons.em);
      struct Sprite* alienSpr = SpriteManager_CreateSprite(demoSingletons.sm, TextureManager_GetTexture(demoSingletons.tm, "assets/animationtest.bmp"));

      alien->sprite = alienSpr;
      alien->position = (SDL_Point){x * 80, y * 80};
      alien->aabbSize = DEMO_ALIEN_SPRITE_SIZE;
      alien->sprite->spriteScalePx = DEMO_ALIEN_SPRITE_SIZE;
      alien->onAabbIntersect = _onAlienHit;
    }
  }
}
