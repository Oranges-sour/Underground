#include "PhysicsManager.h"

#include <memory>

#include "GameDefault.h"
#include "GameManager.h"
#include "GameMap.h"
#include "MyMath.h"
#include "basic_GameSprite.h"
using namespace std;

PhysicsManager* PhysicsManager::create()
{
    PhysicsManager* pm = new (std::nothrow) PhysicsManager();
    if (pm && pm->init()) {
        pm->autorelease();
        return pm;
    }
    CC_SAFE_DELETE(pm);
    return nullptr;
}

void PhysicsManager::updatePhysics(const vector<basic_GameSprite*>& gameObjects)
{
    const auto call = [&](UINT from, UINT to, UINT) {
        for (int x = from; x < to; ++x) {
            auto& sp = gameObjects[x];
            sp->speedVec += sp->accVec;
            if (sp->getPhysicsInfo().contactWithWall) {
                contactWithWall(sp);
            } else {
                sp->setPosition(sp->getPosition() + sp->moveVec + sp->speedVec);
            }
            //�ƶ�ʸ��ÿ�ζ�����
            sp->moveVec = Vec2::ZERO;
        }
    };
    MyTool::callInThread(gameObjects.size(), call);
}

void PhysicsManager::contactWithWall(basic_GameSprite* sprite)
{
    //ʹ�þ���Ķ���ȷ���ڵ�ͼ�ϵ��ĸ���
    auto boundingBox = sprite->getBoundingBox();
    //�����ڵ�ͼ�����е�4������
    auto gameMap = GameManager::getInstance()->getGameMap();
    auto leftTop = gameMap->convertInMap(
        Vec2(boundingBox.getMinX(), boundingBox.getMaxY()));
    auto leftBottom = gameMap->convertInMap(
        Vec2(boundingBox.getMinX(), boundingBox.getMinY()));
    auto rightTop = gameMap->convertInMap(
        Vec2(boundingBox.getMaxX(), boundingBox.getMaxY()));
    auto rightBottom = gameMap->convertInMap(
        Vec2(boundingBox.getMaxX(), boundingBox.getMinY()));

    auto& speedVec = sprite->speedVec;
    //��������
    if (sprite->getPhysicsInfo().gravityEffect) {
        speedVec.y -= 0.8f;
    }
    //�ٶȲ��ܹ���
    constexpr int MAX_SPEED = 35;
    speedVec.x = max<float>(-MAX_SPEED, min<float>(MAX_SPEED, speedVec.x));
    speedVec.y = max<float>(-MAX_SPEED, min<float>(MAX_SPEED, speedVec.y));

    const auto horizonDelta = horizonMove(
        speedVec, {leftBottom, leftTop, rightBottom, rightTop}, boundingBox);
    sprite->setPosition(sprite->getPosition() + horizonDelta);

    //�����ƶ�����֮�����һ��λ��,��ֹ���ִ���������
    //////////////////////////////////////////////////////////////////////////
    auto boundingBox1 = sprite->getBoundingBox();
    //�����ڵ�ͼ�����е�4������
    auto leftTop1 = gameMap->convertInMap(
        Vec2(boundingBox1.getMinX(), boundingBox1.getMaxY()));
    auto leftBottom1 = gameMap->convertInMap(
        Vec2(boundingBox1.getMinX(), boundingBox1.getMinY()));
    auto rightTop1 = gameMap->convertInMap(
        Vec2(boundingBox1.getMaxX(), boundingBox1.getMaxY()));
    auto rightBottom1 = gameMap->convertInMap(
        Vec2(boundingBox1.getMaxX(), boundingBox1.getMinY()));
    //////////////////////////////////////////////////////////////////////////
    const auto verticalDelta =
        verticalMove(speedVec, {leftBottom1, leftTop1, rightBottom1, rightTop1},
                     boundingBox1);
    sprite->setPosition(sprite->getPosition() + verticalDelta);

    const auto checkOnGround = [&]() -> bool {
        //��������Ƿ��ڵ���
        //�ȼ��費�ڵ�����
        bool onGround = false;
        for (int x = leftBottom.x; x <= rightBottom.x; ++x) {
            auto& tile = gameMap->at(iVec2(x, leftBottom.y - 1));
            //�����鲻����!
            if (tile.type != GameMapTileType::air) {
                const float tileY = tile.getRect().getMaxY();
                const float spY = boundingBox.getMinY();
                const float dis = spY - tileY;
                if (dis < 1.5f) {
                    onGround = true;
                    break;
                }
            }
        }
        return onGround;
    };

    auto& info = sprite->getPhysicsInfo();
    info.onGround = checkOnGround();
    //�ڵ���,�������Ħ����
    if (info.onGround) {
        constexpr float m = 0.2f;
        const float N = info.mass * 10;
        const float f = N * 0.3f;
        const float a = f / info.mass;
        if (speedVec.x < 0) {
            speedVec.x = min<float>(0, speedVec.x + a);
        } else {
            speedVec.x = max<float>(0, speedVec.x - a);
        }
    }
}

Vec2 PhysicsManager::horizonMove(
    Vec2& speedVec,
    const std::tuple<const iVec2&, const iVec2&, const iVec2&, const iVec2&>&
        checkPoint,
    const Rect& boundingBox)
{
    if (MyMath::float_equal(0.0f, speedVec.x, 0.01f)) {
        return Vec2(0, 0);
    }

    auto& leftBottom = std::get<0>(checkPoint);
    auto& leftTop = std::get<1>(checkPoint);
    auto& rightBottom = std::get<2>(checkPoint);
    auto& rightTop = std::get<3>(checkPoint);

    int searchX = 0;
    bool toggle = true;
    if (speedVec.x < 0) {
        toggle = false;
    }
    if (toggle) {
        searchX = rightTop.x + 1;
    } else {
        searchX = leftTop.x - 1;
    }
    auto gm = GameManager::getInstance();
    auto gameMap = gm->getGameMap();
    auto bl = gameMap->findMapTile(iVec2(searchX, rightBottom.y),
                                   iVec2(searchX, rightTop.y));

    Vec2 moveDelta(speedVec.x, 0);
    if (bl.first) {
        float tileX = 0;
        float spX = 0;
        if (toggle) {
            tileX = bl.second->getRect().getMinX();
            spX = boundingBox.getMaxX();
        } else {
            tileX = bl.second->getRect().getMaxX();
            spX = boundingBox.getMinX();
        }
        float dis = tileX - spX;
        //��һ�����ؼ�⣬��ֹ����С��������ʧ�������Ĵ�ǽ����
        if (toggle) {
            dis -= 1;
        } else {
            dis += 1;
        }
        if (abs(dis) < abs(speedVec.x)) {
            moveDelta.x = dis;
            speedVec.x = 0;
        }
    }
    return moveDelta;
}

Vec2 PhysicsManager::verticalMove(
    Vec2& speedVec,
    const std::tuple<const iVec2&, const iVec2&, const iVec2&, const iVec2&>&
        checkPoint,
    const Rect& boundingBox)
{
    if (MyMath::float_equal(0.0f, speedVec.y, 0.01f)) {
        return Vec2(0, 0);
    }

    auto& leftBottom = std::get<0>(checkPoint);
    auto& leftTop = std::get<1>(checkPoint);
    auto& rightBottom = std::get<2>(checkPoint);
    auto& rightTop = std::get<3>(checkPoint);

    int searchY = 0;
    bool toggle = true;
    if (speedVec.y < 0) {
        toggle = false;
    }
    if (toggle) {
        searchY = rightTop.y + 1;
    } else {
        searchY = leftBottom.y - 1;
    }
    auto gm = GameManager::getInstance();
    auto gameMap = gm->getGameMap();
    auto bl = gameMap->findMapTile(iVec2(leftBottom.x, searchY),
                                   iVec2(rightBottom.x, searchY));

    Vec2 moveDelta(0, speedVec.y);
    if (bl.first) {
        float tileY = 0;
        float spY = 0;
        if (toggle) {
            tileY = bl.second->getRect().getMinY();
            spY = boundingBox.getMaxY();
        } else {
            tileY = bl.second->getRect().getMaxY();
            spY = boundingBox.getMinY();
        }
        float dis = tileY - spY;
        //��һ�����ؼ�⣬��ֹ����С��������ʧ�������Ĵ�ǽ����
        if (toggle) {
            dis -= 1;
        } else {
            dis += 1;
        }
        if (abs(dis) < abs(speedVec.y)) {
            moveDelta.y = dis;
            speedVec.y = 0;
        }
    }
    return moveDelta;
}

bool PhysicsManager::init()
{
    //��ʼ����ײ���
    auto conatctListener = EventListenerPhysicsContact::create();

    conatctListener->onContactBegin = [&](PhysicsContact& conatct) -> bool {
        processContact(conatct);
        return false;
    };
    conatctListener->onContactPreSolve =
        [](PhysicsContact&, PhysicsContactPreSolve&) -> bool { return false; };
    conatctListener->onContactPostSolve =
        [](PhysicsContact&, const PhysicsContactPostSolve&) -> void {};
    conatctListener->onContactSeparate = [](PhysicsContact&) -> void {};

    Director::getInstance()
        ->getEventDispatcher()
        ->addEventListenerWithSceneGraphPriority(conatctListener, this);

    return true;
}

void PhysicsManager::processContact(PhysicsContact& conatct)
{
    auto const shapeA = conatct.getShapeA();
    auto const shapeB = conatct.getShapeB();

    //��Ϸ�����еĺ�����ײ���������̳���base_GameObject
    auto objectA =
        dynamic_cast<basic_GameSprite*>(shapeA->getBody()->getNode());
    auto objectB =
        dynamic_cast<basic_GameSprite*>(shapeB->getBody()->getNode());

    if (!objectA || !objectB) {
        return;
    }

    const auto& physicsInfoA = objectA->getPhysicsInfo();
    const auto& physicsInfoB = objectB->getPhysicsInfo();

    if (physicsInfoA.activeContact == true &&
        physicsInfoB.activeContact == false) {
        objectA->onContact(objectB);
        objectB->getContact(objectA);
    }

    if (physicsInfoA.activeContact == false &&
        physicsInfoB.activeContact == true) {
        objectA->getContact(objectB);
        objectB->onContact(objectA);
    }

    if (physicsInfoA.activeContact == true &&
        physicsInfoB.activeContact == true) {
        objectA->getContact(objectB);
        objectA->onContact(objectB);

        objectB->getContact(objectA);
        objectB->onContact(objectA);
    }
}