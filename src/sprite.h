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

struct Sprite
{
  SDL_Texture* texture;
  struct Sprite* next;
};

struct SpriteManager
{
  struct Sprite* firstSpr;
  struct Sprite* currentSpr;
};

void SpriteManager_Init(struct SpriteManager* sm);
struct Sprite* SpriteManager_CreateSprite(struct SpriteManager* sm);
void SpriteManager_RemoveSprite(struct SpriteManager* sm, struct Sprite* sprite);

#endif
