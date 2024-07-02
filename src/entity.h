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
#include <stdbool.h>

struct Entity;
struct EntityManager;

struct Entity
{
  int			id;
  SDL_Point		position;
  SDL_Point		aabbSize;
  struct Sprite* 	sprite;
  bool			_markedForRemoval;  // READ-ONLY value

  struct EntityManager* entManager;

  void (*onUpdate)(struct Entity* me);
  void (*onAabbIntersect)(struct Entity* me, struct Entity* intersectingEntity);

  struct Entity* next;
};


struct EntityManager
{
  int lifetimeEntitiesCreated;
  struct Entity* first_ent;
  struct Entity* current_ent;
  struct Entity* new_ent;
};


// EntityManager public funcs:
void EntityManager_Init(struct EntityManager* em);
struct Entity* EntityManager_CreateEntity(struct EntityManager* em);


// EntityManager internal funcs:
// WARN: Does not free ent! You should use Entity_Destroy instead.
void entitymanager_internal_RemoveEntityFromLL(struct EntityManager* em, struct Entity* ent);


// Entity public funcs:
void Entity_Init(struct Entity* ent, int id);

// Marks ent for deletion at the end of the frame.
// This is the correct way to remove an entity.
void Entity_Destroy(struct EntityManager* em, struct Entity* ent);

SDL_Rect Entity_CalculateAABBRect(struct Entity* ent);

#endif
