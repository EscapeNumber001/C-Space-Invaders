#include "entity.h"

void Entity_Init(struct Entity* ent, int id)
{
  ent->id		= id;
  ent->position 	= (SDL_Point){0, 0};
  ent->aabbSize 	= (SDL_Point){0, 0};
  ent->sprite		= NULL;
  ent->_markedForRemoval = false;
  ent->customData	= NULL;
  
  ent->onUpdate		= NULL;
  ent->onAabbIntersect 	= NULL;
}

/*
 * Allocates and initializes a new Entity, adds to em's linked list, and returns a pointer to the new Entity.
 */ 
struct Entity* EntityManager_CreateEntity(struct EntityManager* em)
{
  struct Entity* newEnt = malloc(sizeof(struct Entity));
  newEnt->entManager = em;
  Entity_Init(newEnt, em->lifetimeEntitiesCreated);
  if (em->first_ent == NULL)
    em->first_ent = newEnt;

  newEnt->next = NULL;

  if (em->current_ent)
    em->current_ent->next = newEnt;
  em->current_ent = newEnt;
  em->lifetimeEntitiesCreated++;
  return newEnt;
}

/*
 * Removes ent from em's linked list. **Does not free ent!**
 */
void entitymanager_internal_RemoveEntityFromLL(struct EntityManager* em, struct Entity* ent)
{
  struct Entity* prevEnt = NULL;
  struct Entity* e = em->first_ent;
  while (e)
  {
    if (e->next == ent)
    {
      prevEnt = e;
      break;
    }

    e = e->next;
  }

  if (ent == em->first_ent)
  {
    if (ent->next)
      em->first_ent = ent->next;
    else
      em->first_ent = NULL;
  }

  if (em->current_ent == ent)
    em->current_ent = prevEnt ? prevEnt : em->first_ent;

  if (prevEnt)
    prevEnt->next = ent->next;
  else
    em->first_ent = ent->next;
}


void EntityManager_Init(struct EntityManager* em)
{
  em->first_ent = NULL;
  em->current_ent = NULL;
  em->lifetimeEntitiesCreated = 0;
}

SDL_Rect Entity_CalculateAABBRect(struct Entity* ent)
{
  return (SDL_Rect){ent->position.x, ent->position.y,
    ent->aabbSize.x, ent->aabbSize.y};
}


void Entity_Destroy(struct EntityManager* em, struct Entity* ent)
{
  ent->_markedForRemoval = true;
}
