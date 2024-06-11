/*
 * =====================================================================================
 *
 *       Filename:  test_enemy.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/2024 09:05:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _TEST_ENEMY_H_
#define _TEST_ENEMY_H_

#include "../entity.h"
#include "../globals.h"

struct TestEntityData
{
  int waitUntilFrameNumber;
};

void TestEnemy_Update(struct Entity* ent);

#endif
