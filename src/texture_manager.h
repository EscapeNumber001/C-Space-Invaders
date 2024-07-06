/*
 * =====================================================================================
 *
 *       Filename:  texture_manager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/30/2024 09:54:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _TEXTUREMAN_H_
#define _TEXTUREMAN_H_

#include <SDL2/SDL.h>
#include <json-c/json.h>
#include <stdbool.h>

#define TEXTURE_SPRITESHEET_COLOR_KEY		(SDL_Color){255, 0, 255, 255} // Magenta

struct CachedTexture
{
  SDL_Texture* 		texture;
  char*			filename;
  int			textureLengthPx;
  SDL_Point		textureFrameResolutionPx;  // The size of each frame in the spritesheet (e.g. 16x16, 32x32, etc.)
  int			animationFps;
  bool			loopAnimation;
  struct CachedTexture* next;
};

struct TextureManager
{
  struct CachedTexture* first;
  struct CachedTexture* current;
};

void TextureManager_Init(SDL_Renderer* rend, struct TextureManager* tm);

// Returns false on error.
bool TextureManager_Load(SDL_Renderer* rend, struct TextureManager* tm, char* filename);
bool TextureManager_LoadEx(SDL_Renderer* rend, struct TextureManager* tm, char* filename,
			    int textureLengthPx, SDL_Point textureFrameResolutionPx, 
			    int animationFps, bool loopAnimation);
struct CachedTexture* TextureManager_GetTexture(struct TextureManager* tm, char* filename);
void CachedTexture_Unload(struct TextureManager* tm, struct CachedTexture* texture);
#endif
