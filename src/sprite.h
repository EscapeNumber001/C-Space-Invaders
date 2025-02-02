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
#include "texture_manager.h"

#define SPRITE_SPRITESHEET_PIXELS_PER_FRAME	16
#define SPRITE_ANIMATION_FPS			1
#define SPRITE_SPRITESHEET_COLOR_KEY		(SDL_Color){255, 0, 255, 255} // Magenta

struct Sprite
{
  struct CachedTexture* 	texture;  // TODO: Maybe rename this to "spritesheet" or something to that effect?
  SDL_Rect			spritesheetCropRect;  // What portion of the spritesheet is rendered to the screen.
  SDL_Point			spriteScalePx;
  bool				animationPaused;
  bool				isBackgroundSprite;  // The order in which this sprite will be rendered when overlapping with other sprites. Higher numbers render on top.
  struct Sprite* 		next;

  int 				_framesElapsed;  // (since last animation frame)
};

struct SpriteManager
{
  SDL_Renderer* renderer;
  struct Sprite* firstSpr;
  struct Sprite* currentSpr;
};

void SpriteManager_Init(struct SpriteManager* sm, SDL_Renderer* renderer);
struct Sprite* SpriteManager_CreateSprite(struct SpriteManager* sm, struct CachedTexture* texture);
void SpriteManager_RemoveSprite(struct SpriteManager* sm, struct Sprite* sprite);
void SpriteManager_AnimateSprites(struct SpriteManager* sm, int gameFps);

void Sprite_NextAnimation(struct Sprite* spr);
int Sprite_GetAnimationFrame(struct Sprite* spr);
void Sprite_SetAnimationFrame(struct Sprite* spr, int frame);

#endif
