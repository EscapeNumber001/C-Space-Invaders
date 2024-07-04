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

void TextureManager_Init(SDL_Renderer* rend, struct TextureManager* tm)
{
  TextureManager_Load(rend, tm, "assets/placeholder.bmp");
}

bool TextureManager_Load(SDL_Renderer* rend, struct TextureManager* tm, char* filename)
{
  // TODO: The variable names in this block of code are potentially confusing and should be changed
  struct CachedTexture* t = malloc(sizeof(struct CachedTexture));
  t->filename = filename;
  SDL_Surface* tmp = SDL_LoadBMP(filename);

  char filenameWithJsonExtension[256];
  if (strlen(filename) + strlen(".json") >= 256)
  {
    printf("[TextureManager][ERROR] Filename %s too long!\n", filename);
    return false;
  }

  strcpy(filenameWithJsonExtension, filename);
  strcat(filenameWithJsonExtension, ".json");
  json_object* root = json_object_from_file(filenameWithJsonExtension);
  json_object* jsonTextureLengthPx = json_object_object_get(root, "textureLengthPx");
  json_object* jsonAnimationFps    = json_object_object_get(root, "animationFps");
  json_object* jsonLoopAnimation   = json_object_object_get(root, "loopAnimation");
  json_object* jsonFrameSize       = json_object_object_get(root, "textureFrameResolutionPx");
      json_object* jsonFrameSizeX      = json_object_object_get(jsonFrameSize, "x");
      json_object* jsonFrameSizeY      = json_object_object_get(jsonFrameSize, "y");

  t->textureLengthPx   = json_object_get_int(jsonTextureLengthPx);
  t->animationFps      = json_object_get_int(jsonAnimationFps);
  t->loopAnimation     = json_object_get_boolean(jsonLoopAnimation);
  t->textureFrameResolutionPx = (SDL_Point) {
    json_object_get_int(jsonFrameSizeX),
    json_object_get_int(jsonFrameSizeY)
  };

  json_object_put(root);

  if (root == NULL)
  {
    printf("[TextureManager][ERROR] Failed to parse JSON file %s!\n", filenameWithJsonExtension);
    return false;
  }

  if (tmp == NULL)
  {
    printf("[TextureManager][ERROR] Encountered SDL Error while loading texture: %s\n", SDL_GetError());
    free(t);
    return false;
  }
  SDL_Texture* tmp2 = SDL_CreateTextureFromSurface(rend, tmp);
  if (tmp2 == NULL)
  {
    printf("[TextureManager][ERROR] Encountered SDL Error while converting surface to texture: %s\n", SDL_GetError());
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

  if (!(strcmp(filename, "assets/placeholder.bmp") == 0))  // Prevent infinite recursion if placeholder texture can't load
  {
    printf("[TextureManager][WARN] Failed to load texture %s; loading placeholder\n", filename);
    return TextureManager_GetTexture(tm, "assets/placeholder.bmp");
  }
  else
  {
    printf("[TextureManager][FATAL ERROR] Failed to retrieve placeholder texture!\n");
    SDL_Quit();
    return 0;
  }
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
