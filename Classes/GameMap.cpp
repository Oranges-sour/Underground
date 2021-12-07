#include "GameMap.h"

#include <array>
#include <functional>

#include "GameDefault.h"
#include "GameManager.h"
#include "MyMath.h"
#include "MyResourcesDef.h"
#include "PhysicsShapeCache.h"
#include "Random.h"
#include "ResourcesManager.h"
using namespace std;

/*>>��ͼ�任�涨<<
 * ��ͼ�����е����½�(0,0)�Ŀ���ʾ����(��Ļλ��)�ض������½�
 * ��ͼ�����е����Ͻ�(mapWidth - 1, mapHeight -
 * 1)�Ŀ���ʾ����(��Ļλ��)�ض������Ͻ�
 */

GameMap* GameMap::create()
{
    GameMap* gm = new (std::nothrow) GameMap();
    if (gm) {
        gm->autorelease();
        return gm;
    }
    return nullptr;
}

void GameMap::initMap(UINT width_tile, UINT height_tile, float tilePixelSize,
                      const string& physicsBody)
{
    this->mapWidth = width_tile;
    this->mapHeight = height_tile;
    this->tilePixelSize = tilePixelSize;

    mapStruct.init(width_tile, height_tile, tilePixelSize);

    createTileSprite();
    this->physicsBody = physicsBody;
    setTilePhysicsBody(physicsBody);

    this->setAnchorPoint(Vec2(0, 0));
    this->setPosition(0, 0);
}

Size GameMap::getMapPixleSize()
{
    return Size(mapWidth * tilePixelSize, mapHeight * tilePixelSize);
}

Size GameMap::getMapTileSize() { return Size(mapWidth, mapHeight); }

iVec2 GameMap::convertInMap(const Vec2& position)
{
    const auto size = getMapPixleSize();
    if (Rect(Vec2(0, 0), size).containsPoint(position)) {
        return iVec2(static_cast<int>(position.x / tilePixelSize),
                     static_cast<int>(position.y / tilePixelSize));
    }
    return iVec2(-1, -1);
}

std::pair<Vec2, Vec2> GameMap::convertInPixel(const iVec2& position)
{
    const Vec2 leftBottom(tilePixelSize * position.x,
                          tilePixelSize * position.y);
    const Vec2 rightTop(tilePixelSize * position.x + 1,
                        tilePixelSize * position.y + 1);
    return std::pair<Vec2, Vec2>(leftBottom, rightTop);
}

GameMapTile& GameMap::at(const iVec2& position)
{
    return mapStruct.at(static_cast<UINT>(position.x),
                        static_cast<UINT>(position.y));
}

bool GameMap::tileSameWith(const iVec2& from, const iVec2& to,
                           GameMapTileType type)
{
    bool isSame = true;
    for (int x = from.x; x <= to.x; ++x) {
        for (int y = from.y; y <= to.y; ++y) {
            if (at(iVec2(x, y)).type != type) {
                isSame = false;
                break;
            }
        }
    }
    return isSame;
}

pair<bool, GameMapTile*> GameMap::findMapTile(const iVec2& from,
                                              const iVec2& to)
{
    CCASSERT((from.x == to.x || from.y == to.y), "ֻ������һ��");
    CCASSERT((from.x <= to.x && from.y <= to.y), "from����С��to");

    if (from.x == to.x) {
        //��������
        for (int y = from.y; y <= to.y; ++y) {
            auto& tile = at(iVec2(from.x, y));
            //�����鲻����!
            if (tile.type != GameMapTileType::air) {
                return pair<bool, GameMapTile*>(true, &tile);
            }
        }
    } else {
        //��������
        for (int x = from.x; x <= to.x; ++x) {
            auto& tile = at(iVec2(x, from.y));
            //�����鲻����!
            if (tile.type != GameMapTileType::air) {
                return pair<bool, GameMapTile*>(true, &tile);
            }
        }
    }
    return pair<bool, GameMapTile*>(false, nullptr);
}

void GameMap::_update() { updatePhysicsTile(); }

const vector<basic_GameMapBuilding*>& GameMap::getMapBuildings()
{
    return buildings;
}

bool GameMap::isInitFinish() { return initFinish; }

pair<iVec2, iVec2> GameMap::getInitProgress()
{
    //�������
    if (init_Height >= 0) {
        iVec2 w(init_Width, mapBlockWidth - 1);
        iVec2 h(mapBlockHeight - 1 - init_Height, mapBlockHeight - 1);
        return pair<iVec2, iVec2>(w, h);
    }
    iVec2 w(mapBlockWidth - 1, mapBlockWidth - 1);
    iVec2 h(mapBlockHeight - 1, mapBlockHeight - 1);
    return pair<iVec2, iVec2>(w, h);
}

void GameMap::createTileSprite()
{
    //��ʱ����,��ֹ�ڴ�ռ�ù���
    //���ڲ��ʵ��ڴ�ռ�ù���,���еĵ�ͼ���ʶ��ᱻ��С��Ⱦ

    vector<vector<Sprite*>> tiles;
    tiles.resize(mapHeight, vector<Sprite*>(mapWidth));

    //һ����ͼ����Ƭ�����Ŀ�͸�
    constexpr int mapBlockTileSize = 32;

    //��Ⱦʱ��ͼ����ʵ��С: size = realSize / scaleRate,Ϊ�˽�ʡ�ڴ�
    constexpr unsigned int scaleRate = 2;

    //����һ����ͼ�Ĵ�С
    mapBlockSize = tilePixelSize * mapBlockTileSize;

    //��������Ĵ�ͼ����
    mapBlockWidth =
        MyMath::floatRoundInt(mapWidth / static_cast<float>(mapBlockTileSize));
    mapBlockHeight =
        MyMath::floatRoundInt(mapHeight / static_cast<float>(mapBlockTileSize));
    mapBlocks.resize(mapBlockHeight, vector<Sprite*>(mapBlockWidth));
    for (int x = 0; x < mapBlockWidth; ++x) {
        for (int y = 0; y < mapBlockHeight; ++y) {
            auto sp = Sprite::create();
            mapBlocks[y][x] = sp;
            sp->setAnchorPoint(Vec2(0, 1));
            sp->setScaleY(-static_cast<float>(scaleRate));
            sp->setScaleX(static_cast<float>(scaleRate));
            this->addChild(sp, 2);
        }
    }

    for (int x = 0; x < mapBlockWidth; ++x) {
        for (int y = 0; y < mapBlockHeight; ++y) {
            auto& bigMap = mapBlocks[y][x];
            bigMap->setPosition(Vec2(mapBlockSize * x, y * mapBlockSize));
        }
    }

    init_Width = 0;
    init_Height = 0;

    this->schedule(
        [scaleRate, mapBlockTileSize, this](float) {
            //��Ⱦ��ͼ
            renderMap(scaleRate, mapBlockTileSize);
            ++init_Width;

            if (init_Width == mapBlockWidth) {
                init_Width = 0;
                ++init_Height;
            }

            if (init_Height == mapBlockHeight) {
                initFinish = true;
                this->unschedule("initMap");
            }
        },
        "initMap");
}

void GameMap::setTilePhysicsBody(const string& physicsBody)
{
    physicsTiles.resize(mapHeight, vector<GameMapPhysicsTile>(mapWidth));

    for (int x = 0; x < mapWidth; ++x) {
        for (int y = 0; y < mapHeight; ++y) {
            //ֻ�з��鲻�ǿ���,����Ҫ������ײ
            auto& tile = physicsTiles[y][x];
            if (mapStruct.at(x, y).type != GameMapTileType::air)
                tile.needPhysics = true;
            else
                tile.needPhysics = false;
            tile.physicsTile = nullptr;
            tile.posInMap.x = x;
            tile.posInMap.y = y;
        }
    }
}

void GameMap::updatePhysicsTile()
{
    //����ģ���ͣ����֡��
    constexpr int PHYSICS_STAY_FRAMES = 20;

    auto sprites = GameManager::getInstance()->getAllSprites();
    for (auto& s : sprites) {
        auto& a = s.second;
        for (auto& sp : a) {
            if (!sp->getPhysicsInfo().needPhysics) {
                continue;
            }
            const auto spriteBox = sp->getBoundingBox();
            //��þ������Ͻ�����
            const auto spriteLeftBottomPos =
                Vec2(spriteBox.getMinX(), spriteBox.getMinY());
            //���½�
            const auto spriteRightTopPos =
                Vec2(spriteBox.getMaxX(), spriteBox.getMaxY());

            auto mapLeftBottomPos = this->convertInMap(spriteLeftBottomPos);
            auto mapRightTopPos = this->convertInMap(spriteRightTopPos);

            for (int x = mapLeftBottomPos.x; x <= mapRightTopPos.x; ++x) {
                for (int y = mapLeftBottomPos.y; y <= mapRightTopPos.y; ++y) {
                    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                        auto& pt = physicsTiles[y][x];
                        if (pt.needPhysics) {
                            pt.count = PHYSICS_STAY_FRAMES;
                            pt.isInDirty = true;
                            dirtyPhysicsTiles.push_back(&pt);
                        }
                    }
                }
            }
        }
    }

    //��Ҫ��dirty��ɾ����
    vector<GameMapPhysicsTile*> needToErase;
    for (int x = 0; x < dirtyPhysicsTiles.size(); ++x) {
        auto& pt = dirtyPhysicsTiles[x];
        //������Ϊ0,׼����������ģ���
        if (pt->count > 0) {
            //����֮ǰû�д�����,��ʼ����
            if (pt->physicsTile == nullptr) {
                auto pyt = GameMapPhysicsObject::create();

                GameSpritePhysicsInfo pyInfo;
                pyInfo.physicsShape = physicsBody;
                pyInfo.activeContact = false;
                pyInfo.gravityEffect = false;

                pyt->setPhysicsInfo(pyInfo);
                pyt->setAnchorPoint(Vec2(0, 0));
                pt->physicsTile = pyt;
                this->addChild(pyt, 2);
            }
        } else {
            //��ɾ����
            if (pt->physicsTile != nullptr) {
                pt->physicsTile->removeFromParent();
                pt->physicsTile = nullptr;
            }
            needToErase.push_back(pt);
            pt->isInDirty = false;
        }
    }

    for (auto it = needToErase.begin(); it != needToErase.end(); ++it) {
        dirtyPhysicsTiles.erase(
            remove(dirtyPhysicsTiles.begin(), dirtyPhysicsTiles.end(), *it),
            dirtyPhysicsTiles.end());
    }

    //�ȶԲ�Ϊ0�ļ���ֵ�����������
    const auto subPhysicsTileCount = [&]() {
        const auto doSub = [&](UINT from, UINT to, UINT) {
            for (int x = from; x < to; ++x) {
                auto& c = dirtyPhysicsTiles[x]->count;
                if (c > 0) {
                    c -= 1;
                }
            }
        };
        MyTool::callInThread(dirtyPhysicsTiles.size(), doSub);
    };

    const auto setPhysicsTilePos = [&]() {
        const auto doSet = [&](UINT from, UINT to, UINT) {
            for (int x = from; x < to; ++x) {
                auto& phyTile = dirtyPhysicsTiles[x];
                phyTile->physicsTile->setPosition(
                    Vec2(phyTile->posInMap.x * tilePixelSize,
                         phyTile->posInMap.y * tilePixelSize));
            }
        };
        MyTool::callInThread(dirtyPhysicsTiles.size(), doSet);
    };

    subPhysicsTileCount();
    setPhysicsTilePos();
}

GameMap::TileTextureBlock GameMap::getTileTextureBlock(const iVec2& position)
{
    auto gameMana = GameManager::getInstance();
    auto resMana = gameMana->getResourcesManager();

    TileTextureBlock block;
    //��ʼ��
    block.first = nullptr;
    for (auto& it : block.second) {
        it = nullptr;
    }

    string tileSpriteName;
    switch (at(position).type) {
        case GameMapTileType::dirt:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapDirt)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        case GameMapTileType::grass:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapGrass)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        case GameMapTileType::rock:
            tileSpriteName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapRock)[0];
            block.first = Sprite::createWithSpriteFrameName(tileSpriteName);
            break;
        default:
            break;
    }

    //���������ǿ�����,Ҳ�Ͳ���Ҫ������
    if (block.first == nullptr) {
        return block;
    }

    if (position.x - 1 >= 0 && position.y + 1 < mapHeight) {
        if (at(iVec2(position.x - 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x - 1, position.y + 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y + 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[0];
            block.second[0] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x + 1 < mapWidth && position.y + 1 < mapHeight) {
        if (at(iVec2(position.x + 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x + 1, position.y + 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y + 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[1];
            block.second[1] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x + 1 < mapWidth && position.y - 1 >= 0) {
        if (at(iVec2(position.x + 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x + 1, position.y - 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y - 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[2];
            block.second[2] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    if (position.x - 1 >= 0 && position.y - 1 >= 0) {
        if (at(iVec2(position.x - 1, position.y)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x - 1, position.y - 1)).type ==
                GameMapTileType::air &&
            at(iVec2(position.x, position.y - 1)).type ==
                GameMapTileType::air) {
            string coverName;
            coverName =
                resMana->getSpriteFrames(ResKey::SpFrame::GameMapCover)[3];
            block.second[3] = Sprite::createWithSpriteFrameName(coverName);
        }
    }

    return block;
}

void GameMap::renderMap(unsigned int scaleRate, int mapBlockTileSize)
{
    const int startX = init_Width * mapBlockTileSize;
    const int endX = (init_Width + 1) * mapBlockTileSize;
    const int startY = init_Height * mapBlockTileSize;
    const int endY = (init_Height + 1) * mapBlockTileSize;
    //�����ķ�ΧΪ: [startX, endX) �� [startY, endY)

    vector<vector<TileTextureBlock>> tiles;
    tiles.resize(mapBlockTileSize, vector<TileTextureBlock>(mapBlockTileSize));

    //������ʱ��ͼ��
    for (int x = startX, dx = 0; x < endX; ++x, ++dx) {
        for (int y = startY, dy = 0; y < endY; ++y, ++dy) {
            auto block = getTileTextureBlock(iVec2(x, y));
            tiles[dy][dx] = block;

            auto sp = block.first;
            if (sp) {
                sp->setAnchorPoint(Vec2(0, 0));
                const float rate = 1.0f / scaleRate;
                sp->setScale(rate);
                sp->setPosition(Vec2(dx * tilePixelSize, dy * tilePixelSize) *
                                rate);
            }
            for (auto& it : block.second) {
                if (it) {
                    it->setAnchorPoint(Vec2(0, 0));
                    const float rate = 1.0f / scaleRate;
                    it->setScale(rate);
                    it->setPosition(
                        Vec2(dx * tilePixelSize, dy * tilePixelSize) * rate);
                    it->setBlendFunc({GL_DST_COLOR, GL_ZERO});
                }
            }
        }
    }

    //��ʼ��Ⱦ
    auto rt = RenderTexture::create(mapBlockSize / scaleRate,
                                    mapBlockSize / scaleRate);
    rt->beginWithClear(0, 0, 0, 0);
    //////////////////////////////////////////////////////////////////////////
    for (int x = 0; x < mapBlockTileSize; ++x) {
        for (int y = 0; y < mapBlockTileSize; ++y) {
            auto& block = tiles[y][x];
            if (block.first) {
                block.first->visit();
            }
            for (auto& it : block.second) {
                if (it) {
                    it->visit();
                }
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    rt->end();
    //���ո���Ⱦ�õĵ�ͼ�洢��������
    auto& block = mapBlocks[init_Height][init_Width];
    block->setSpriteFrame(rt->getSprite()->getSpriteFrame());
    block->getTexture()->setAliasTexParameters();
}

void GameMapStruct::init(UINT width, UINT height, float tilePixelSize)
{
    this->width = width;
    this->height = height;
    _map.resize(height, vector<GameMapTile>(width));
    initMapBlock();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].tileRect =
                Rect(Vec2(x * tilePixelSize, y * tilePixelSize),
                     Size(tilePixelSize, tilePixelSize));
        }
    }
}

GameMapTile& GameMapStruct::at(UINT x, UINT y) { return _map[y][x]; }

void GameMapStruct::initMapBlock()
{
    //����
    /*for (int y = 0; y < height; ++y)
        {
                for (int x = 0; x < width; ++x)
                {
                        _map[y][x].type = GameMapBlockType::dirt;
                }
        }
        return;*/

    //�������е�ͼ��
    const auto rangeAll = [this](const function<void(int, int)>& func) {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                func(x, y);
            }
        }
    };

    //���������
    rand_int r0(1, 25);
    rangeAll([&](int x, int y) {
        _map[y][x].type = GameMapTileType::dirt;
        if (r0() == 1) {
            _map[y][x].type = GameMapTileType::air;
        }
    });

    //��չ������
    rand_int r1(1, 3);
    rand_int r2(0, 2);
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::air) {
            int xAdd = r1();
            int yAdd = r2();
            for (int my = y - yAdd; my < y + yAdd; ++my) {
                for (int mx = x - xAdd + 1; mx < x + xAdd; ++mx) {
                    if (my >= 0 && my < height && mx >= 0 && mx < width) {
                        _map[my][mx].type = GameMapTileType::air;
                    }
                }
            }
        }
    });

    //��ֹһ��ߵ�·
    rangeAll([&](int x, int y) {
        if (y - 2 >= 0 && y + 2 < height) {
            /*��״ͼ��:(X���жϵĻ�׼��)
            ####      ####
            #OOO      OOOO
            OOOO  ->  OOOO
            OX##      OX##
            O###      O###
            */
            if (x - 1 >= 0) {
                //����
                if (_map[y][x].type == GameMapTileType::dirt &&
                    _map[y + 1][x].type == GameMapTileType::air &&
                    _map[y + 1][x - 1].type == GameMapTileType::air &&
                    _map[y + 2][x - 1].type == GameMapTileType::dirt) {
                    _map[y + 2][x - 1].type = GameMapTileType::air;
                }
            }

            /*��״ͼ��:(X���жϵĻ�׼��)
            ####      ####
            OOO#      OOOO
            OOOO  ->  OOOO
            ##XO      ##XO
            ###O      ###O
            */
            if (x + 1 < width) {
                //����
                if (_map[y][x].type == GameMapTileType::dirt &&
                    _map[y + 1][x].type == GameMapTileType::air &&
                    _map[y + 1][x + 1].type == GameMapTileType::air &&
                    _map[y + 2][x + 1].type == GameMapTileType::dirt) {
                    _map[y - 2][x + 1].type = GameMapTileType::air;
                }
            }
        }

        if (y - 1 >= 0 && y + 1 < height) {
            /*��״ͼ��: #:����  O:����
            #######      OOOOOOO
            OOOOOOO  ->  OOOOOOO
            #######      OOOOOOO
            */
            if (_map[y][x].type == GameMapTileType::air &&
                _map[y - 1][x].type == GameMapTileType::dirt &&
                _map[y + 1][x].type == GameMapTileType::dirt) {
                _map[y - 1][x].type = GameMapTileType::air;
                _map[y + 1][x].type = GameMapTileType::air;
            }
        }

        if (x - 1 >= 0 && x + 1 < width) {
            /*��״ͼ��: #:����  O:����
            #O#    OOO
            #O# -> OOO
            #O#    OOO
            */
            if (_map[y][x].type == GameMapTileType::air) {
                if (_map[y][x - 1].type == GameMapTileType::dirt &&
                    _map[y][x + 1].type == GameMapTileType::dirt) {
                    _map[y][x - 1].type = GameMapTileType::air;
                    _map[y][x + 1].type = GameMapTileType::air;
                }
            }
        }
    });

    //�����ʯ
    rand_int r3(0, 3);
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::dirt) {
            int temp = 0;
            //������Ե������ľ���
            int dis = r3();

            if (y - dis >= 0) {
                if (_map[y - dis][x].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (y + dis < height) {
                if (_map[y + dis][x].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (x - dis >= 0) {
                if (_map[y][x - dis].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (x + dis < width) {
                if (_map[y][x + dis].type != GameMapTileType::air) {
                    ++temp;
                }
            }

            if (temp == 4) {
                _map[y][x].type = GameMapTileType::rock;
            }
        }
    });

    //�ݵ�
    rangeAll([&](int x, int y) {
        if (_map[y][x].type == GameMapTileType::dirt) {
            if (y + 1 < height) {
                if (_map[y + 1][x].type == GameMapTileType::air) {
                    _map[y][x].type = GameMapTileType::grass;
                }
            }
        }
    });

    //���������Ϳ�
    for (int x = 0; x < 25; ++x) {
        for (int y = 0; y < 25; ++y) {
            _map[x][y].type = GameMapTileType::air;
        }
    }

    //�߽������������

    //����
    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    //����
    for (int x = width - 2; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    //����
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < width; ++x) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    //����
    for (int y = height - 2; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            _map[y][x].type = GameMapTileType::dirt;
        }
    }

    _map[0][0].type = GameMapTileType::grass;
}

GameMapPhysicsObject* GameMapPhysicsObject::create()
{
    auto pt = new (std::nothrow) GameMapPhysicsObject();
    if (pt && pt->initGameSprite()) {
        pt->autorelease();
        return pt;
    }
    CC_SAFE_DELETE(pt);
    return nullptr;
}

bool GameMapPhysicsObject::initGameSprite()
{
    basic_GameSprite::initGameSprite();
    Sprite::initWithSpriteFrameName(ResKey::Frame::PHYSICS_SHOW);
    //��ײ��ʱ�Ƿ������ʾ
    this->setVisible(false);
    return true;
}

const Rect& GameMapTile::getRect() { return tileRect; }
