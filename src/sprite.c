#include "sprite.h"

void SpriteManager_Init(struct SpriteManager* sm)
{
  sm->firstSpr   = NULL;
  sm->currentSpr = NULL;
}

struct Sprite* SpriteManager_CreateSprite(struct SpriteManager* sm)
{
  struct Sprite* newSpr = malloc(sizeof(struct Sprite));
  if (sm->firstSpr == NULL)
    sm->firstSpr = newSpr;

  newSpr->next = NULL;

  if (sm->currentSpr)
    sm->currentSpr->next = newSpr;
  sm->currentSpr = newSpr;
  return newSpr;
}

void SpriteManager_RemoveSprite(struct SpriteManager* sm, struct Sprite* sprite)
{
  struct Sprite* prevSpr = NULL;
  struct Sprite* s = sm->firstSpr;
  while (s)
  {
    if (s->next == sprite)
    {
      prevSpr = s;
      break;
    }
    s = s->next;
  }

  if (prevSpr)
    prevSpr->next = sprite->next;

  if (sm->firstSpr == sprite)
    sm->firstSpr = NULL;
}
