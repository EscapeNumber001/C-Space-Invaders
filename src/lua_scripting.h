/*
 * =====================================================================================
 *
 *       Filename:  lua.h
 *
 *    Description: 
 *
 *
 *        Version:  1.0
 *        Created:  06/12/2024 10:27:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

/* ==============================================
 * TODO:
 * Func to get all entities in an array
 * Func to move entities
 * Update and AABB collision hooks for entities
 * ==============================================
*/
#ifndef _LUA_SCR_H_
#define _LUA_SCR_H_

#include <lua.h>
#include <lauxlib.h>
#include "globals.h"
#include "texture_manager.h"
#include "entity.h"

void LuaSystem_Init(SDL_Renderer* renderer, struct EntityManager* em, struct SpriteManager* sm, struct TextureManager* tm);
int LuaSystem_lFunc_CreateEntity(lua_State* l);

// Args:
// Entity* (lightuserdata)
int LuaSystem_lFunc_DestroyEntity(lua_State* l);

// Args:
// Entity* (lightuserdata)
// Texture filename (string)
int LuaSystem_lFunc_SetEntityTexture(lua_State* l);

// Args:
// Entity* (lightuserdata)
// New X pos (integer)
// New Y pos (integer)
int LuaSystem_lFunc_MoveEntity(lua_State* l);

// Args:
// Filename (string)
// Texture length (integer)
// Animation FPS (integer)
// Loop animation? (bool)
int LuaSystem_lFunc_LoadTexture(lua_State* l);

#endif
