#include "test.h"

#include "GameManager.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "Random.h"
#include "ResourcesManager.h"

TestHero* TestHero::create(const string& hero, const string& center,
                           const string& out)
{
    auto h = new (std::nothrow) TestHero();
    if (h && h->initGameSprite(hero, center, out)) {
        h->autorelease();
        return h;
    }
    CC_SAFE_DELETE(h);
    return nullptr;
}

bool TestHero::initGameSprite(const string& hero, const string& center,
                              const string& out)
{
    this->cocosSchedule([&](float) { this->_update(); }, "__update");
    basic_Hero::initGameSprite();
    this->center = Sprite::createWithSpriteFrameName(center);
    this->out = Sprite::createWithSpriteFrameName(out);

    this->addChild(this->center, 1);
    this->addChild(this->out, 2);
    return this->initWithSpriteFrameName(hero);
}

void TestHero::onContact(basic_GameSprite* contactTarget)
{
    basic_Hero::onContact(contactTarget);
}

void TestHero::_update()
{
    auto lights = getAllLightSprites();
    for (auto& it : lights) {
        it.second->setPosition(this->getPosition());
    }
    center->setPosition(getContentSize() / 2);
    out->setPosition(getContentSize() / 2);
}

void TestHero::attackNear(const Vec2& pos) {}

void TestHero::attackFar(const Vec2& pos, float holdTime)
{
    auto& ib = this->getItemBag();
    auto& im = ib.getAllItems();
    for (auto& item : im) {
        if (item.first == GameItemType::farWeapon) {
            for (auto& w : item.second) {
                auto weapon = dynamic_cast<FarWeapon*>(w);
                weapon->useItem(this, this->getPosition(), pos, holdTime);
            }
        }
    }
}

TestItemSprite* TestItemSprite::create(const std::string& p)
{
    auto item = new (std::nothrow) TestItemSprite();
    if (item && item->initGameSprite(p)) {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

bool TestItemSprite::initGameSprite(const std::string& p)
{
    basic_GameItemSprite::initGameSprite();
    return this->initWithSpriteFrameName(p);
}

void TestItemSprite::kill()
{
    GameManager::getInstance()->removeGameSprite(this);
}

void TestItemSprite::itemPickedUp(basic_Hero* hero)
{
    hero->getItemBag().addItem(new TestWeapon());
    this->kill();
}

HeroBullet* HeroBullet::create(const string& p)
{
    auto bu = new (std::nothrow) HeroBullet();
    if (bu && bu->initGameSprite(p)) {
        bu->autorelease();
        return bu;
    }
    CC_SAFE_DELETE(bu);
    return nullptr;
}

bool HeroBullet::initGameSprite(const string& p)
{
    basic_Bullet::initGameSprite();
    return this->initWithSpriteFrameName(p);
}

void HeroBullet::delayShoot(const Vec2& startPosition, const Vec2& to,
                            float time)
{
    this->cocosScheduleOnce(
        [this, startPosition, to](float) { shoot(startPosition, to); }, time,
        "delayShoot");
}

void HeroBullet::shoot(const Vec2& startPosition, const Vec2& to)
{
    this->setPosition(startPosition);
    deltaPos = MyMath::getPosOnLine(Vec2(0, 0), to - startPosition, 40);
    this->speedVec = deltaPos;
    this->accVec = Vec2(0, -0.8f);
    this->uploadPhysicsBody();
    this->cocosSchedule([&](float) { this->update1(); }, "moveUpdate");
}

void HeroBullet::update1()
{
    this->setRotation(MyMath::getRotation(Vec2(0, 0), speedVec));
    auto& sps = this->getAllLightSprites();
    for (auto& it : sps) {
        auto liSp = it.second;
        if (!isContactFinish()) {
            liSp->setPosition(this->getPosition());
        } else {
            liSp->setScale(liSp->getScale() - 0.15f);
            int op = liSp->getOpacity();
            if (op - 20 > 0) {
                liSp->setOpacity(op - 20);
            } else {
                liSp->setOpacity(0);
            }
        }
    }
}

void HeroBullet::kill()
{
    auto gm = GameManager::getInstance();
    auto rm = gm->getResourcesManager();
    //��������
    for (int x = 0; x < 6; ++x) {
        auto par = basic_Particle::create(
            rm->getSpriteFrames(ResKey::SpFrame::Particle)[0], 0.1f, 2, 20);
        par->setPosition(this->getPosition());
        par->setScale(0.5f);
        gm->addGameSprite(par, GameRenderOrder::user0);
    }

    auto li = this->getAllLightSprites();
    for (auto& lisp : li) {
        auto& sp = lisp.second;
        sp->setOpacity(255);
        sp->setScale(sp->getScale() + 2);
    }

    this->setVisible(false);
    auto& phy = this->getPhysicsInfo();
    phy.needPhysics = false;
    this->unloadPhysicsBody();

    this->cocosScheduleOnce(
        [&](float) {
            auto gameManager = GameManager::getInstance();
            gameManager->removeGameSprite(this);
        },
        0.3f, "__delayKill");
}

void HeroBullet::onContact(basic_GameSprite* contactTarget)
{
    auto type = contactTarget->getGameSpriteType();
    if ((type.type0 == GameSpriteType::Type0::mapTile ||
         type.type0 == GameSpriteType::Type0::enemy) &&
        !isContactFinish()) {
        setContactFinish(true);
        this->kill();
    }
}

GameSpriteType HeroBullet::getGameSpriteType()
{
    return {GameSpriteType::Type0::bullet, GameSpriteType::Type1::heroBullet};
}

void TestWeapon::useItem(basic_GameSprite* gameSprite, const Vec2& from,
                         const Vec2& to, float holdTime)
{
    int i = 1 + max<float>(0, holdTime - 0.5f) / 0.1f;
    i = min(4, i);
    for (int x = 0; x < i; ++x) {
        auto gameMan = GameManager::getInstance();
        auto rm = gameMan->getResourcesManager();
        auto bullet = HeroBullet::create(
            rm->getSpriteFrames(ResKey::SpFrame::HeroBullet)[0]);
        bullet->setDamage(20);
        {
            GameSpritePhysicsInfo info;
            info.activeContact = true;
            info.gravityEffect = false;
            info.needPhysics = true;
            info.physicsShape = rm->getPhysicsBody(ResKey::Physics::HeroBullet);
            bullet->setPhysicsInfo(info);
        }
        {
            auto lightSp = Sprite::createWithSpriteFrameName("colorLight.png");
            lightSp->setPosition(from);
            lightSp->setOpacity(255 * 0.8f);
            lightSp->setScale(3);
            bullet->addLightSprite(lightSp, 0);
        }
        rand_float r(-50, 50);
        bullet->shoot(from, to + Vec2(r(), r()) * min(i, 3));

        gameMan->addGameSprite(bullet, GameRenderOrder::user0);
    }
}
