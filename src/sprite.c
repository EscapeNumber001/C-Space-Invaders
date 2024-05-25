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
  newSpr->spritesheetCropRect = (SDL_Rect){0, 0, SPRITE_SPRITESHEET_PIXELS_PER_FRAME, SPRITE_SPRITESHEET_PIXELS_PER_FRAME};

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


void SpriteManager_AnimateSprites(struct SpriteManager* sm)
{
  struct Sprite* spr = sm->firstSpr;
  while (spr != NULL)
  {
    Sprite_NextAnimation(spr);
    spr = spr->next;
  }
}

void Sprite_NextAnimation(struct Sprite* spr)
{
  if (spr->spritesheetCropRect.x + SPRITE_SPRITESHEET_PIXELS_PER_FRAME >= spr->spritesheetLengthPx)
  {
    spr->spritesheetCropRect.x = 0;
    return;
  }

  spr->spritesheetCropRect.x += SPRITE_SPRITESHEET_PIXELS_PER_FRAME;
}



