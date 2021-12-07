#include "GameManager.h"

#include <algorithm>
#include <future>
#include <thread>
#include <utility>
using namespace std;

#include "ControlManager.h"
#include "GameDefault.h"
#include "GameMap.h"
#include "GameRenderer.h"
#include "Hero.h"
#include "Joystick.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsManager.h"
#include "ResourcesManager.h"
#include "TouchesPool.h"
#include "Weapon.h"
#include "basic_GameSprite.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

GameManager* GameManager::instance = nullptr;

void GameManager::createInstance()
{
    //����ʵ��ʱ,��ǰ��ʵ��ָ��Ӧ��Ϊ��!
    CC_ASSERT(instance == nullptr);
    instance = new (std::nothrow) GameManager();
    if (instance && instance->init()) {
        instance->autorelease();
        return;
    }
    CC_SAFE_DELETE(instance);
}

void GameManager::destroyInstance()
{
    //ʵ����ӦΪ��!
    CC_ASSERT(instance != nullptr);
    instance->removeFromParent();
    instance = nullptr;
}

GameManager* GameManager::getInstance() { return instance; }

void GameManager::initManager()
{
    //��ʼ����Ⱦ��
    gameRenderer = GameRenderer::create();
    //��ʼ����ͼ
    gameMap = GameMap::create();
    //��ʼ�����������
    physicsManager = PhysicsManager::create();
    //��ʼ��������
    controlManager = ControlManager::create();

    this->addChild(physicsManager, 1);
    this->addChild(gameRenderer, 2);
    this->addChild(controlManager, 3);

    gameRenderer->addNode(gameMap, static_cast<int>(GameRenderOrder::map));
    gameRenderer->setVisible(false);
    //������ѭ��
    this->schedule(
        [&](float dt) {
            if (!gameMap->isInitFinish()) {
                return;
            } else {
                gameRenderer->setVisible(true);
            }
            auto vsp = std::move(this->mainUpdateBeforeRender(dt));
            this->gameRenderer->_update(vsp, hero->getPosition(),
                                        controlManager->getMousePosition());
            this->mainUpdateAfterRender(dt);
        },
        /*һ֡����һ��*/ 0, "mainUpdate");
}

vector<basic_GameSprite*> GameManager::mainUpdateBeforeRender(float dt)
{
    vector<basic_GameSprite*> vsp;
    //����vec
    for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter) {
        auto& s = iter->second;
        for (auto i = s.begin(); i != s.end(); ++i) {
            auto& sp = *i;
            vsp.push_back(sp);
        }
    }

    // cocos�̸߳���
    for (auto iter = vsp.begin(); iter != vsp.end(); ++iter) {
        auto& sp = *iter;
        sp->cocosScheduleUpdate(dt);
    }

    // logic�̸߳���
    MyTool::callInThread(vsp.size(), [&](UINT from, UINT to, UINT) {
        for (UINT x = from; x < to; ++x) {
            vsp[x]->logicScheduleUpdate(dt);
        }
    });

    this->updateHeroMove();
    physicsManager->updatePhysics(vsp);
    gameMap->_update();

    return std::move(vsp);
}

void GameManager::mainUpdateAfterRender(float dt)
{
    //����µľ���
    for (auto iter = needToAdd.begin(); iter != needToAdd.end(); ++iter) {
        auto& sp = iter->first;
        auto& type = sp->getGameSpriteType();

        auto i = gameObjects.find(type);
        if (i == gameObjects.end()) {
            gameObjects.insert(make_pair(type, set<basic_GameSprite*>({sp})));
        } else {
            i->second.insert(sp);
        }
        gameRenderer->addNode(sp, static_cast<int>(iter->second));
        //��ӵ�ʱ��retain��һ��,����release��
        sp->release();
    }
    needToAdd.clear();

    //�Ƴ�����
    for (auto iter = needToErase.begin(); iter != needToErase.end(); ++iter) {
        auto& sp = *iter;
        auto& type = sp->getGameSpriteType();

        auto i = gameObjects.find(type);
        if (i != gameObjects.end()) {
            i->second.erase(sp);
        }
        sp->removeFromParent();
    }
    needToErase.clear();
    //����µľ���
    for (auto iter = needToAdd.begin(); iter != needToAdd.end(); ++iter) {
        auto& sp = iter->first;
        auto& type = sp->getGameSpriteType();

        auto i = gameObjects.find(type);
        if (i == gameObjects.end()) {
            gameObjects.insert(make_pair(type, set<basic_GameSprite*>({sp})));
        } else {
            i->second.insert(sp);
        }
        gameRenderer->addNode(sp, static_cast<int>(iter->second));
        //��ӵ�ʱ��retain��һ��,����release��
        sp->release();
    }
    needToAdd.clear();

    //�Ƴ�����
    for (auto iter = needToErase.begin(); iter != needToErase.end(); ++iter) {
        auto& sp = *iter;
        auto& type = sp->getGameSpriteType();

        auto i = gameObjects.find(type);
        if (i != gameObjects.end()) {
            i->second.erase(sp);
        }
        sp->removeFromParent();
    }
    needToErase.clear();
}

void GameManager::pauseGame()
{
    for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter) {
        auto& s = iter->second;
        for (auto i = s.begin(); i != s.end(); ++i) {
            (*i)->pause();
        }
    }
    gamePauseState = true;
}

void GameManager::resumeGame()
{
    for (auto iter = gameObjects.begin(); iter != gameObjects.end(); ++iter) {
        auto& s = iter->second;
        for (auto i = s.begin(); i != s.end(); ++i) {
            (*i)->resume();
        }
    }
    gamePauseState = false;
}

bool GameManager::isGamePause() { return gamePauseState; }

void GameManager::addGameSprite(basic_GameSprite* gameObject,
                                GameRenderOrder layerOrder)
{
    gameObject->retain();
    needToAdd.push_back(make_pair(gameObject, layerOrder));
}

void GameManager::addNode(Node* node, GameRenderOrder layerOrder)
{
    gameRenderer->addNode(node, static_cast<int>(layerOrder));
}

void GameManager::removeGameSprite(basic_GameSprite* gameObject)
{
    auto iter = std::find(needToErase.begin(), needToErase.end(), gameObject);
    if (iter == needToErase.end()) needToErase.push_back(gameObject);
}

ResourcesManager* GameManager::getResourcesManager()
{
    return userResourcesManager;
}

GameRenderer* GameManager::getGameRenderer() { return gameRenderer; }

GameMap* GameManager::getGameMap() { return gameMap; }

const GameManager::GameSpriteContainer& GameManager::getAllSprites()
{
    return gameObjects;
}

void GameManager::cleanup()
{
    delete userResourcesManager;
    Node::cleanup();
}

void GameManager::setHero(basic_Hero* hero)
{
    this->hero = hero;
    this->addGameSprite(hero, GameRenderOrder::user0);
}

basic_Hero* GameManager::getHero() { return hero; }

bool GameManager::init()
{
    userResourcesManager = new ResourcesManager();
    return true;
}

void GameManager::updateHeroMove()
{
    auto& moveVec = controlManager->getMoveVec();
    auto& speedVec = hero->speedVec;
    speedVec.x = moveVec.x * 10;

    if (hero->speedVec.y < 13.0f) {
        hero->speedVec.y += moveVec.y * 2.5f;
    }
}
