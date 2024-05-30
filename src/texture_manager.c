/*
 * =====================================================================================
 *
 *       Filename:  texture_manager.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/30/2024 10:15:48 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "texture_manager.h"

bool TextureManager_Load(SDL_Renderer* rend, struct TextureManager* tm, char* filename)
{
  // TODO: The variable names in this block of code are potentially confusing and should be changed
  struct CachedTexture* t = malloc(sizeof(struct CachedTexture));
  SDL_Surface* tmp = SDL_LoadBMP(filename);
  if (tmp == NULL)
  {
    free(t);
    return false;
  }
  SDL_Texture* tmp2 = SDL_CreateTextureFromSurface(rend, tmp);
  if (tmp2 == NULL)
  {
    free(t);
    free(tmp);
    return false;
  }
  t->texture = tmp2;
  free(tmp);

  t->next = NULL;
  if (tm->first == NULL)
    tm->first = t;
  if (tm->current != NULL)
    tm->current->next = t;
  tm->current = t;
  return true;
}
