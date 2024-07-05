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

void _onAlienHit(struct Entity* self, struct Entity* other)
{
  if (self->_markedForRemoval)
    return;

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

// Function to linearly interpolate between two values
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
    if (closest->position.x + DEMO_ALIEN_SPRITE_SIZE.x >= WIDTH)  // We multiply by 2 here because SDL sprite coords start from the top-left corner. Otherwise the sprite would hang slightly offscreen before dropping.
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

  // If we're not at the edge
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

void Demo_Init(struct EntityManager* em, struct TextureManager* tm, struct SpriteManager* sm)
{
  demoSingletons.em = em;
  demoSingletons.tm = tm;
  demoSingletons.sm = sm;
  alienMoveDirection = DEMO_MOVE_DIR_RIGHT;
}

void Demo_StartGame()
{
  alienMoveCoordinator = EntityManager_CreateEntity(demoSingletons.em);
  alienMoveCoordinator->onUpdate = coordinateAlienMove;
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
      
      alien->customData = (struct AlienCustomData*)malloc(sizeof(struct AlienCustomData));
      ((struct AlienCustomData*)(alien->customData))->timeSinceLastMove = 0;
    }
  }
}
