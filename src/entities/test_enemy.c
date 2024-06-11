/*
 * =====================================================================================
 *
 *       Filename:  test_enemy.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2024 09:15:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "test_enemy.h"

void TestEnemy_Update(struct Entity* ent)
{
  struct Entity* newEnt = EntityManager_CreateEntity(&em);
  struct Sprite* newSpr = SpriteManager_CreateSprite(&sm, TextureManager_GetTexture(&tm, "assets/bullet.bmp"));
  newEnt->sprite = newSpr;
  newEnt->position = (SDL_Point){newEnt->id * 25, 0};
  printf("hi\n");
}
