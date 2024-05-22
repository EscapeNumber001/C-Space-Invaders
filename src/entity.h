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

#define ENTITY_LIST_SIZE 4096

struct Entity;
struct EntityManager;

struct Entity
{
  int		id;
  SDL_Point	position;
  SDL_Point	aabbSize;  // TODO AABBs
  SDL_Texture* 	texture;

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

struct Entity* EntityManager_CreateEntity(struct EntityManager* em);
void EntityManager_RemoveEntity(struct EntityManager* em, struct Entity* ent);
void Entity_Init(struct Entity* ent, int id);

#endif
