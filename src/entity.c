#include "entity.h"

void Entity_Init(struct Entity* ent, int id)
{
  ent->id		= id;
  ent->position 	= (SDL_Point){0, 0};
  ent->aabbSize 	= (SDL_Point){0, 0};
  ent->texture		= NULL;
  
  ent->onAabbIntersect 	= NULL;
}

/*
 * Allocates and initializes a new Entity, adds to em's linked list, and returns a pointer to the new Entity.
 */ 
struct Entity* EntityManager_CreateEntity(struct EntityManager* em)
{
  struct Entity* newEnt = malloc(sizeof(struct Entity));
  em->new_ent = newEnt;
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
void EntityManager_RemoveEntity(struct EntityManager* em, struct Entity* ent)
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

  if (prevEnt)
    prevEnt->next = ent->next;
}
