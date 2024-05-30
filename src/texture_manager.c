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
  return TextureManager_LoadEx(rend, tm, filename, 0, 0, false);
}

bool TextureManager_LoadEx(SDL_Renderer* rend, struct TextureManager* tm, char* filename,
			    int textureLengthPx, int animationFps, bool loopAnimation)
{ 
  // TODO: The variable names in this block of code are potentially confusing and should be changed
  struct CachedTexture* t = malloc(sizeof(struct CachedTexture));
  t->textureLengthPx = textureLengthPx;
  t->animationFps = animationFps;
  t->loopAnimation = loopAnimation;
  t->filename = filename;

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

struct CachedTexture* TextureManager_GetTexture(struct TextureManager* tm, char* filename)
{
  for (struct CachedTexture* t = tm->first; t != NULL; t = t->next)
  {
    if (strcmp(t->filename, filename) == 0)
      return t;
  }
  return NULL;
}
