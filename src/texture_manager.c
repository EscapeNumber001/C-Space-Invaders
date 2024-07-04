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
  return TextureManager_LoadEx(rend, tm, filename, 0, (SDL_Point){0},  0, false);
}

bool TextureManager_LoadEx(SDL_Renderer* rend, struct TextureManager* tm, char* filename,
			    int textureLengthPx, SDL_Point textureFrameResolutionPx,
			    int animationFps, bool loopAnimation)
{ 
  // TODO: The variable names in this block of code are potentially confusing and should be changed
  struct CachedTexture* t = malloc(sizeof(struct CachedTexture));
  t->filename = filename;
  SDL_Surface* tmp = SDL_LoadBMP(filename);

  // WARN: This may be insecure
  char filenameWithJsonExtension[256];
  strcpy(filenameWithJsonExtension, filename);
  strcat(filenameWithJsonExtension, ".json");
  json_object* root = json_object_from_file(filenameWithJsonExtension);
  json_object* jsonTextureLengthPx = json_object_object_get(root, "textureLengthPx");
  json_object* jsonAnimationFps    = json_object_object_get(root, "animationFps");
  json_object* jsonLoopAnimation   = json_object_object_get(root, "loopAnimation");
  t->textureLengthPx   = json_object_get_int(jsonTextureLengthPx);
  t->animationFps      = json_object_get_int(jsonAnimationFps);
  t->loopAnimation     = json_object_get_boolean(jsonLoopAnimation);
  if (root == NULL)
  {
    printf("[TextureManager][ERROR] Failed to parse JSON file %s\n", filenameWithJsonExtension);
  }

  if (tmp == NULL)
  {
    printf("[TextureManager][ERROR] Encountered SDL Error while loading texture: %s", SDL_GetError());
    free(t);
    return false;
  }
  SDL_Texture* tmp2 = SDL_CreateTextureFromSurface(rend, tmp);
  if (tmp2 == NULL)
  {
    printf("[TextureManager][ERROR] Encountered SDL Error while converting surface to texture: %s", SDL_GetError());
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

void CachedTexture_Unload(struct TextureManager* tm, struct CachedTexture* texture)
{
  struct CachedTexture* prev = NULL;
  struct CachedTexture* t = tm->first;
  while (t)
  {
    if (t->next == texture)
    {
      prev = t;
      break;
    }
    t = t->next;
  }

  if (texture == tm->first)
  {
    if (texture->next)
    {
      tm->first = texture->next;
    }
    else
    {
      tm->first = NULL;
    }
  }

  if (tm->current == texture)
    tm->current = prev ? prev : tm->first;

  if (prev)
    prev->next = texture->next;
  else
    tm->first = texture->next;

  free(texture->texture);
  free(texture);
}
