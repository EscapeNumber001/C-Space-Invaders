#include "entity.h"

void Entity_Init(struct Entity* ent)
{
  ent->position 	= (SDL_Point){0, 0};
  ent->aabbSize 	= (SDL_Point){0, 0};
  ent->texture		= NULL;
  
  ent->onAabbIntersect 	= NULL;
}
