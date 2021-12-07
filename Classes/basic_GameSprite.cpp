#include "basic_GameSprite.h"

#include "GameDefault.h"
#include "GameManager.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsShapeCache.h"
#include "ResourcesManager.h"

using namespace std;

// void basic_GameSprite::addBuff(GameObjectBuffType objectBuffType, float time)
//{
//	//����Ƿ��д�buff,�о�����ʱ��,û�о�����µ�
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end())
//	{
//		it->second += time;
//	}
//	else
//	{
//		buffs.insert(pair<GameObjectBuffType, float>{ objectBuffType,
// time
//});
//	}
//}

// void basic_GameSprite::subBuff(GameObjectBuffType objectBuffType, float time)
//{
//	//����Ƿ��д�buff,�оͼ���ʱ��,û�о�ʲôҲ����
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end())
//	{
//		//����ʱ��֮���ʣ��ʱ��
//		float nowTime = it->second - time;
//		//������ʱ������ʱ��û��ʣ��(С��0),���Ƴ���buff
//		if (nowTime < 0)
//		{
//			buffs.erase(objectBuffType);
//		}
//		else
//		{
//			it->second = nowTime;
//		}
//	}
//}

// const map<GameObjectBuffType, float>& basic_GameSprite::getBuff()
//{
//	return buffs;
//}

// bool basic_GameSprite::isBuff(GameObjectBuffType objectBuffType, float time)
//{
//	auto it = buffs.find(objectBuffType);
//	if (it != buffs.end() && it->second >= time)
//	{
//		return true;
//	}
//	return false;
//}

void basic_GameSprite::onContact(basic_GameSprite* contactTarget) {}

void basic_GameSprite::getContact(basic_GameSprite* contactFrom) {}

void basic_GameSprite::kill()
{
    GameManager::getInstance()->removeGameSprite(this);
}

GameSpriteType basic_GameSprite::getGameSpriteType()
{
    return {GameSpriteType::Type0::unknow, GameSpriteType::Type1::unknow};
}

void basic_GameSprite::setPhysicsInfo(const GameSpritePhysicsInfo& physicsInfo)
{
    PhysicsShapeCache::getInstance()->setBodyOnSprite(physicsInfo.physicsShape,
                                                      this);
    this->physicsInfo = physicsInfo;
}

GameSpritePhysicsInfo& basic_GameSprite::getPhysicsInfo()
{
    return physicsInfo;
}

void basic_GameSprite::uploadPhysicsBody()
{
    if (!physicsBody) {
        physicsBody = PhysicsShapeCache::getInstance()->createBodyWithName(
            physicsInfo.physicsShape);
        physicsBody->setName("__PhysicsBody");
        physicsBody->retain();
    }
    if (!this->getComponent("__PhysicsBody")) {
        this->addComponent(physicsBody);
    }
}

void basic_GameSprite::unloadPhysicsBody()
{
    this->removeComponent(physicsBody);
}

const std::map<int, Sprite*>& basic_GameSprite::getAllLightSprites()
{
    return lightSprites;
}

Sprite* basic_GameSprite::getLightSprite(int key)
{
    auto iter = lightSprites.find(key);
    if (iter != lightSprites.end()) {
        return iter->second;
    }
    return nullptr;
}

void basic_GameSprite::addLightSprite(Sprite* lightSprite, int key)
{
    lightSprites.insert({key, lightSprite});
    lightSprite->setVisible(false);
    this->addChild(lightSprite, -1);
}

void basic_GameSprite::removeLightSprite(int key)
{
    auto iter = lightSprites.find(key);
    iter->second->removeFromParent();
    lightSprites.erase(iter);
}

bool basic_GameSprite::initGameSprite()
{
    // this->logicSchedule([&](float dt)
    //	{
    //		//buff��ʱ�����
    //		for (auto it = buffs.begin(); it != buffs.end();)
    //		{
    //			//����ʱ��֮���ʣ��ʱ��
    //			float nowTime = it->second - dt;
    //			//������ʱ������ʱ��û��ʣ��(С��0),���Ƴ���buff
    //			if (nowTime < 0)
    //				it = buffs.erase(it);
    //			else
    //			{
    //				it->second = nowTime;
    //				++it;
    //			}
    //		}
    //	}, "__updateBuff");

    return true;
}

void basic_GameSprite::cleanup()
{
    for (auto& it : lightSprites) {
        it.second->removeFromParent();
    }
    if (physicsBody) {
        physicsBody->release();
    }

    Sprite::cleanup();
}
