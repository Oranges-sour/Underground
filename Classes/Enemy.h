#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"
#include "Weapon.h"

class basic_Enemy : public basic_GameSprite
{
public:
	/**
	*����Ѫ��
	*@param hitpoint: Ѫ��
	*@return ��
	*/
	void setHitPoint(int hitpoint);

	/**
	*���Ѫ��
	*@param ��
	*@return Ѫ��
	*/
	int getHitPoint();


private:
	int hitpoint = 0;
};

#endif