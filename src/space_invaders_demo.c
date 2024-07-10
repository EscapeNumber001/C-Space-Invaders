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

int score = 0;

void _demo_onAlienHit(struct Entity* self, struct Entity* other)
{
  if (strcmp(other->sprite->texture->filename, "assets/bullet.bmp") == 0)
  {
    if (((struct BulletCustomData*)other->customData)->team == DEMO_TEAM_ALIEN)
      return;
    score += DEMO_SCORE_PER_ALIEN;
    Entity_Destroy(demoSingletons.em, self);
    Entity_Destroy(demoSingletons.em, other);
    alienCount--;
  }
}

void _demo_onPlayerHit(struct Entity* self, struct Entity* other)
{
  if (strcmp(other->sprite->texture->filename, "assets/bullet.bmp") == 0)
  {
    if (((struct BulletCustomData*)other->customData)->team != DEMO_TEAM_PLAYER)
    {
      Entity_Destroy(demoSingletons.em, self);
      _demo_playerDied();
    }
  }
}

bool _demo_isAlien(struct Entity* ent)
{
  if (ent->sprite == NULL)
    return false;

  return (strcmp(ent->sprite->texture->filename, "assets/animationtest.bmp") == 0);
}

struct Entity* _demo_getAlienClosestToEdge()
{
  int absoluteClosest = 0xC0FFEE;
  struct Entity* closestEnt = NULL;

  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!_demo_isAlien(e))
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

int _demo_lerp(int a, int b, float t) {
    return (int)((1.0f - t) * a + t * b);
}

void _demo_moveAllAliensDown()
{
  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!_demo_isAlien(e))
      goto _continue;

    e->position.y += DEMO_ALIEN_SPRITE_SIZE.y;

_continue:
    e = e->next;
  }
}

int _demo_calculateAlienMoveDelayMs()
{
  // Calculate the percentage of aliens remaining
  float percentageRemaining = (float)alienCount / (float)DEMO_MAX_ALIENS;
    
  // Invert the percentage because we want the delay to decrease as aliens decrease
  float t = 1.0f - percentageRemaining;
    
  // Lerp between the slowest and fastest delay
  return _demo_lerp(DEMO_SLOWEST_ALIEN_MOVE_DELAY_MS, DEMO_FASTEST_ALIEN_MOVE_DELAY_MS, t);
}

void _demo_alienMoveCoordinatorUpdate(struct Entity* self, int frameDelta)
{
  if (alienCount <= 0)
    return;

  if (msSinceLastMove < _demo_calculateAlienMoveDelayMs())
  {
    msSinceLastMove += frameDelta;
    return;
  }
  
  msSinceLastMove = 0;
  struct Entity* closest = _demo_getAlienClosestToEdge();
  if (alienMoveDirection == DEMO_MOVE_DIR_RIGHT)
  {
    if (closest->position.x + DEMO_ALIEN_SPRITE_SIZE.x >= WIDTH)
    {
      alienMoveDirection = -alienMoveDirection;
      _demo_moveAllAliensDown();
      return;
    }
  }
  else
  {
    if (closest->position.x - DEMO_ALIEN_SPRITE_SIZE.x < 0)
    {
      alienMoveDirection = -alienMoveDirection;
      _demo_moveAllAliensDown();
      return;
    }
  }

  // If we're not at the edge:
  struct Entity* e = demoSingletons.em->first_ent;
  while (e)
  {
    if (!_demo_isAlien(e))
      goto _continue;
    e->position.x += alienMoveDirection * DEMO_ALIEN_SPRITE_SIZE.x;

_continue:
    e = e->next;
  }
}

void _demo_bulletUpdate(struct Entity* self, int frameDelta)
{
  self->position.y += ((struct BulletCustomData*)self->customData)->velocity;

  if (self->position.y < 0 || self->position.y > HEIGHT)
  {
    Entity_Destroy(demoSingletons.em, self);
  }
}

void _demo_playerUpdate(struct Entity* self, int frameDelta)
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
      bullet->onUpdate = _demo_bulletUpdate;
      bullet->sprite->spriteScalePx.x = 25;
      *cantShootUntilTick = SDL_GetTicks() + shootCooldownMs;

      struct BulletCustomData* cd = (struct BulletCustomData*)malloc(sizeof(struct BulletCustomData));
      cd->team = DEMO_TEAM_PLAYER;
      cd->velocity = -15;
      bullet->customData = cd;
  }
}

void _demo_alienUpdate(struct Entity* self, int frameDelta)
{ 
  int chance = rand() % 250;
  if (chance != 0)
    return;
  struct Entity* bullet = EntityManager_CreateEntity(demoSingletons.em);
  bullet->aabbSize = (SDL_Point){25, 100};
  bullet->position = (SDL_Point){self->position.x + 40, self->position.y - 75};
  bullet->sprite = bulletSprite;
  bullet->onUpdate = _demo_bulletUpdate;
  bullet->sprite->spriteScalePx.x = 25;

  struct BulletCustomData* cd = (struct BulletCustomData*)malloc(sizeof(struct BulletCustomData));
  cd->team = DEMO_TEAM_ALIEN;
  cd->velocity = 5;
  bullet->customData = cd;
}

void _demo_displayNumber(char* text, SDL_Point textPosition)
{
  char* c = text;
  int i = 0;
  while (*c != '\0')
  {
    struct Entity* ent = EntityManager_CreateEntity(demoSingletons.em);
    struct Sprite* letterSpr = SpriteManager_CreateSprite(demoSingletons.sm, TextureManager_GetTexture(demoSingletons.tm, "assets/numbers.bmp"));
    ent->position = (SDL_Point){textPosition.x + i * DEMO_TEXT_SIZE_PX, textPosition.y};
    ent->sprite = letterSpr;
    ent->sprite->animationPaused = true;
    ent->sprite->spriteScalePx = (SDL_Point){DEMO_TEXT_SIZE_PX, DEMO_TEXT_SIZE_PX};
    Sprite_SetAnimationFrame(ent->sprite, *c - '0');  // Convert ASCII digit to a raw integer
    i++;
    c++;
  }
}

void _demo_playerDied()
{
  char scoreAsStr[16];
  sprintf(scoreAsStr, "%d", score);
  printf("%s\n", scoreAsStr);
  _demo_displayNumber(scoreAsStr, (SDL_Point){WIDTH / 2, HEIGHT / 2});

  struct Entity* gameOverWindow = EntityManager_CreateEntity(demoSingletons.em);
  struct Sprite* gowSpr = SpriteManager_CreateSprite(demoSingletons.sm, TextureManager_GetTexture(demoSingletons.tm, "assets/gameoverwindow.bmp"));
  gameOverWindow->sprite = gowSpr;
  gameOverWindow->position = (SDL_Point){0};
  gameOverWindow->sprite->spriteScalePx = (SDL_Point){300, 150};
}

void Demo_Init(SDL_Renderer* renderer, struct EntityManager* em, struct TextureManager* tm, struct SpriteManager* sm)
{
  demoSingletons.em = em;
  demoSingletons.tm = tm;
  demoSingletons.sm = sm;

  TextureManager_Load(renderer, tm, "assets/animationtest.bmp");
  TextureManager_Load(renderer, tm, "assets/player.bmp");
  TextureManager_Load(renderer, tm, "assets/bullet.bmp");
  TextureManager_Load(renderer, tm, "assets/numbers.bmp");
  TextureManager_Load(renderer, tm, "assets/gameoverwindow.bmp");

  alienMoveDirection = DEMO_MOVE_DIR_RIGHT;
  bulletSprite = SpriteManager_CreateSprite(sm, TextureManager_GetTexture(tm, "assets/bullet.bmp"));
}

void Demo_StartGame()
{
  alienMoveCoordinator = EntityManager_CreateEntity(demoSingletons.em);
  alienMoveCoordinator->onUpdate = _demo_alienMoveCoordinatorUpdate;

  struct Entity* player = EntityManager_CreateEntity(demoSingletons.em);
  struct Sprite* playerSpr = SpriteManager_CreateSprite(demoSingletons.sm, TextureManager_GetTexture(demoSingletons.tm, "assets/player.bmp"));

  player->sprite = playerSpr;
  player->position = (SDL_Point){0, HEIGHT - 100};
  player->aabbSize = (SDL_Point){100, 100};
  player->onUpdate = _demo_playerUpdate;
  player->onAabbIntersect = _demo_onPlayerHit;
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
      alien->onUpdate = _demo_alienUpdate;
      alien->position = (SDL_Point){x * 80, y * 80};
      alien->aabbSize = DEMO_ALIEN_SPRITE_SIZE;
      alien->sprite->spriteScalePx = DEMO_ALIEN_SPRITE_SIZE;
      alien->onAabbIntersect = _demo_onAlienHit;
    }
  }
}
