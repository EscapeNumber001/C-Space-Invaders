/*
 * =====================================================================================
 *
 *       Filename:  sprite.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/22/2024 07:03:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SPRITE_SPRITESHEET_PIXELS_PER_FRAME	16
#define SPRITE_ANIMATION_FPS			1

struct Sprite
{
  SDL_Texture* 		texture;  // TODO: Maybe rename this to "spritesheet" or something to that effect?
  SDL_Rect		spritesheetCropRect;  // What portion of the spritesheet is rendered to the screen.
  int			spritesheetLengthPx;
  bool			loopAnimation;
  struct Sprite* 	next;
};

struct SpriteManager
{
  struct Sprite* firstSpr;
  struct Sprite* currentSpr;
};

void SpriteManager_Init(struct SpriteManager* sm);
struct Sprite* SpriteManager_CreateSprite(struct SpriteManager* sm);
void SpriteManager_RemoveSprite(struct SpriteManager* sm, struct Sprite* sprite);
void SpriteManager_AnimateSprites(struct SpriteManager* sm);

void Sprite_NextAnimation(struct Sprite* spr);

#endif
