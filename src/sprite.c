#include "sprite.h"

void SpriteManager_Init(struct SpriteManager* sm, SDL_Renderer* renderer)
{
  sm->firstSpr   = NULL;
  sm->currentSpr = NULL;
}

struct Sprite* SpriteManager_CreateSprite(struct SpriteManager* sm, struct CachedTexture* texture)
{
  // Add the sprite to the linked list
  struct Sprite* newSpr = malloc(sizeof(struct Sprite));
  if (sm->firstSpr == NULL)
    sm->firstSpr = newSpr;

  newSpr->next = NULL;
  if (sm->currentSpr)
    sm->currentSpr->next = newSpr;

  sm->currentSpr = newSpr;

  newSpr->texture = texture;
  newSpr->isBackgroundSprite = false;
  newSpr->spriteScalePx = (SDL_Point){100, 100};
  newSpr->spritesheetCropRect = (SDL_Rect){0, 0, texture->textureFrameResolutionPx.x, texture->textureFrameResolutionPx.y};
  newSpr->_framesElapsed = 0;
  newSpr->animationPaused = false;
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


void SpriteManager_AnimateSprites(struct SpriteManager* sm, int gameFps)
{
  struct Sprite* spr = sm->firstSpr;
  while (spr != NULL)
  {
    if (spr->_framesElapsed >= gameFps - spr->texture->animationFps)
    {
      if (!spr->animationPaused)
	Sprite_NextAnimation(spr);
      spr->_framesElapsed = 0;
    }
    spr->_framesElapsed++;
    spr = spr->next;
  }
}

void Sprite_NextAnimation(struct Sprite* spr)
{
  if (spr->spritesheetCropRect.x + spr->texture->textureFrameResolutionPx.x >= spr->texture->textureLengthPx)
  {
    spr->spritesheetCropRect.x = spr->texture->loopAnimation ? 0 : spr->spritesheetCropRect.x;
    return;
  }

  spr->spritesheetCropRect.x += spr->texture->textureFrameResolutionPx.x;
}

int Sprite_GetAnimationFrame(struct Sprite* spr)
{
  return spr->spritesheetCropRect.x / spr->texture->textureFrameResolutionPx.x;
}

void Sprite_SetAnimationFrame(struct Sprite* spr, int frame)
{
  spr->spritesheetCropRect.x = spr->texture->textureFrameResolutionPx.x * frame;
}
