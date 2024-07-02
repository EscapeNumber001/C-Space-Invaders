/*
 * =====================================================================================
 *
 *       Filename:  lua_scripting.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/2024 10:34:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "lua_scripting.h"

lua_State* luaState;
SDL_Renderer* luaSystem_renderer;
struct EntityManager* luaSystem_em;
struct SpriteManager* luaSystem_sm;
struct TextureManager* luaSystem_tm;

void LuaSystem_Init(SDL_Renderer* renderer, struct EntityManager* em, struct SpriteManager* sm, struct TextureManager* tm)
{
  luaState = luaL_newstate(); 
  lua_register(luaState, "CreateEntity", LuaSystem_lFunc_CreateEntity);
  lua_register(luaState, "DestroyEntity", LuaSystem_lFunc_DestroyEntity);
  lua_register(luaState, "LoadTexture", LuaSystem_lFunc_LoadTexture);
  lua_register(luaState, "SetEntityTexture", LuaSystem_lFunc_SetEntityTexture);

  luaSystem_renderer = renderer;
  luaSystem_em = em;
  luaSystem_sm = sm;
  luaSystem_tm = tm;
  // DEBUG Runs a preset Lua test script
  luaL_dostring(luaState, "x = CreateEntity(); LoadTexture('assets/animationtest.bmp', 64, 15, true); SetEntityTexture(x, 'assets/animationtest.bmp'); --DestroyEntity(x)");
}

int LuaSystem_lFunc_CreateEntity(lua_State* l)
{
  struct Entity* e = EntityManager_CreateEntity(luaSystem_em);
  e->sprite = SpriteManager_CreateSprite(luaSystem_sm, NULL);
  lua_pushlightuserdata(l, e);
  return 1;
}

int LuaSystem_lFunc_DestroyEntity(lua_State* l)
{
  if (!lua_islightuserdata(l, 1))
  {
    printf("[Lua:DestroyEntity()][ERROR] Argument 1 is not of type lightuserdata!\n");
    return 0;
  }

  struct Entity* e = (struct Entity*)lua_topointer(l, 1);
  Entity_Destroy(luaSystem_em, e);
  return 0;
}

int LuaSystem_lFunc_LoadTexture(lua_State* l)
{
  char* filename = (char*)lua_tostring(l, 1);
  int   imgLen   = lua_tointeger(l, 2);
  int   animFps  = lua_tointeger(l, 3);
  bool  loop     = lua_toboolean(l, 4);
  TextureManager_LoadEx(luaSystem_renderer, luaSystem_tm, filename, imgLen, animFps, loop);
  return 0;
}

int LuaSystem_lFunc_SetEntityTexture(lua_State* l)
{
  struct Entity* foo = (struct Entity*)lua_topointer(l, 1);
  if (!lua_islightuserdata(l, 1))
  {
    printf("[Lua:SetEntityTexture()][ERROR] Argument 1 is not of type lightuserdata!\n");
    return 0;
  }

  char* name = (char*)lua_tostring(l, 2);

  struct CachedTexture* t = TextureManager_GetTexture(luaSystem_tm, name);
  if (t == NULL)
  {
    printf("[Lua:SetEntityTexture()][ERROR] Texture %s hasn't been loaded yet!\n", name); 
    return 0;
  }
  
  foo->sprite->texture = t;
  return 0;
}
