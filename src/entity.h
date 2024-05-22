/*
 * =====================================================================================
 *
 *       Filename:  entity.h
 *
 *    Description: 
 *
 *
 *        Version:  1.0
 *        Created:  05/20/2024 03:01:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>

struct Entity
{
  SDL_Point	position;
  SDL_Point	aabbSize;  // TODO AABBs
  SDL_Texture* 	texture;

  void (*onAabbIntersect)(struct Entity* me, struct Entity* intersectingEntity);
};


void Entity_Init(struct Entity* ent);

#endif
