#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <utility>
#include <vector>

#include "GameDefault.h"
#include "basic_GameSprite.h"
#include "iVec2.h"

class GameMap;
class GameMapStruct;

//��ͼ�������
enum class GameMapTileType {
    //δ֪
    unknow,
    //����
    air,
    //�ݵ�
    grass,
    //����
    dirt,
    //ʯ��
    rock
};

class basic_GameMapBuilding;

//һ����ͼ����Ƭ
class GameMapTile {
public:
    GameMapTileType type;

    const Rect& getRect();

private:
    friend class GameMapStruct;
    //������ײ���ľ���
    Rect tileRect;
};

class GameMapPhysicsObject : public basic_GameSprite {
public:
    /**
     *������������Ϣ�ĵ�ͼ��
     *@param ��
     *@return ��ͼ��
     */
    static GameMapPhysicsObject* create();

    virtual void onContact(basic_GameSprite* contactTarget) override {}
    virtual void getContact(basic_GameSprite* contactFrom) override {}
    virtual void kill() override {}
    virtual GameSpriteType getGameSpriteType() override
    {
        return {GameSpriteType::Type0::mapTile, GameSpriteType::Type1::unknow};
    }

private:
    /**
     *��ʼ����ͼ��
     *@param ��
     *@return �Ƿ��ʼ���ɹ�
     */
    virtual bool initGameSprite();
};

class GameMapStruct {
public:
    /**
     *��ʼ����ͼ,�����õ�ͼ�Ĵ�С,��������Ƭ����
     *@param width_tile:��ͼ�Ŀ��,��Ƭ����
     *@param height_tile:��ͼ�ĸ߶�,��Ƭ����
     *@param tilePixleSize:��ͼ�����ش�С
     *@return ��
     */
    void init(UINT width, UINT height, float tilePixelSize);

    /**
     *��ȡ����λ�õ���Ƭ
     *@param x: ������
     *@param y: ������
     *@return ��
     */
    GameMapTile& at(UINT x, UINT y);

private:
    /**
     *��ʼ����ͼ��
     *@param ��
     *@return ��
     */
    void initMapBlock();

    std::vector<std::vector<GameMapTile>> _map;
    int width = 0;
    int height = 0;
};

class GameMapPhysicsTile {
public:
    GameMapPhysicsObject* physicsTile = nullptr;
    bool needPhysics = false;
    /*ʹ�ü���,ÿһ֡����һ��
     *���ٵ�0���Զ������������ģ���
     */
    int count = 0;
    bool isInDirty = false;
    iVec2 posInMap;
};

class GameMap final : public Node {
public:
    /**
     *������ͼ����,�������GameMap::initMap(...)
     *@param ��
     *@return ��ͼ����ָ��
     */
    static GameMap* create();

    /**
     *��ʼ����ͼ,�����õ�ͼ�Ĵ�С(��������Ƭ����)
     *@param width_tile:��ͼ�Ŀ��(��Ƭ����)
     *@param height_tile:��ͼ�ĸ߶�(��Ƭ����)
     *@param tilePixelSize:һ����Ƭ�����ش�С(��Ƭ��������,���赥���趨����)
     *@param physicsBody:ÿһ����ͼ���������Ϣ
     *@return ��
     */
    void initMap(UINT width_tile, UINT height_tile, float tilePixelSize,
                 const std::string& physicsBody);

    /**
     *��õ�ͼ�����ش�С
     *@param ��
     *@return ��ͼ�����ش�С
     */
    Size getMapPixleSize();

    /**
     *��õ�ͼ��Ĵ�С
     *@param ��
     *@return ��С
     */
    Size getMapTileSize();

    /**
     *����ʵ��������ӳ�䵽��ͼ��������
     *@param position: ��ʵ��������
     *@return ��ͼ��������
     */
    iVec2 convertInMap(const Vec2& position);

    /**
     *����ͼ��������ӳ�䵽��������
     *@param position: ��ͼ��������
     *@return һ��pair,��ǰ��������½Ǻ����Ͻ�����
     * (first:���½�, second:���Ͻ�)
     */
    std::pair<Vec2, Vec2> convertInPixel(const iVec2& position);

    /**
     *��õ�ͼ��������ĵ�ͼ��
     *@param position: ��ͼ��������
     *@return ��ͼ��
     */
    GameMapTile& at(const iVec2& position);

    /**
     *��һ�����ڵ����е�ͼ���Ƿ񶼺Ͳ���������ͬ
     *@param from: ���½���ʼ��
     *@param to: ���Ͻ��յ�
     *@param type: ����
     *@return ��ͼ��
     */
    bool tileSameWith(const iVec2& from, const iVec2& to, GameMapTileType type);

    /**
     *�����ƶ�ʱʹ��,�ҵ��������ߵķ���ǰ���Ƿ��з���
     *@param ��
     *@return first:�Ƿ��з��� second:���ֵķ���
     */
    std::pair<bool, GameMapTile*> findMapTile(const iVec2& from,
                                              const iVec2& to);

    /**
     *ÿһ֡����
     *@param ��
     *@return ��
     */
    void _update();

    /**
     *��õ�ͼ�е����н�����
     *@param ��
     *@return ������
     */
    const std::vector<basic_GameMapBuilding*>& getMapBuildings();

    /**
     *��ͼ�Ƿ��ʼ�����(����Ԥ��Ⱦ�����)
     *@param ��
     *@return ��
     */
    bool isInitFinish();

    /**
     *��õ�ǰ��ʼ���Ľ���
     *@param ��
     *@return ��һ��ģ������x��y����ǰ
     */
    std::pair<iVec2, iVec2> getInitProgress();

private:
    /**
     *������Ƭ����
     *@param ��
     *@return ��
     */
    void createTileSprite();

    /**
     *������Ƭ��������ײ
     *@param physicsBody:����
     *@return ��
     */
    void setTilePhysicsBody(const std::string& physicsBody);

    /**
     *���������
     *@param ��
     *@return ��
     */
    void updatePhysicsTile();

    //��ͼ���ʿ�
    using TileTextureBlock = std::pair<Sprite*, std::array<Sprite*, 4>>;

    /**
     *��ö�Ӧ��ͼ��ľ���(������ͼԤ��Ⱦʱʹ��)
     *@param position: ��ͼ���λ��
     *@return ����
     */
    TileTextureBlock getTileTextureBlock(const iVec2& position);

    /**
     *��Ⱦ��ͼ
     *@param ��
     *@return ��
     */
    void renderMap(unsigned int scaleRate, int mapBlockTileSize);

private:
    GameMapStruct mapStruct;
    std::vector<std::vector<GameMapPhysicsTile>> physicsTiles;
    //��Ҫ���µ������
    std::vector<GameMapPhysicsTile*> dirtyPhysicsTiles;
    std::vector<std::vector<Sprite*>> mapBlocks;
    int mapWidth = 0;
    int mapHeight = 0;
    float tilePixelSize = 0;

    //һ����Ⱦ��ͼ����Ŀ��
    int mapBlockWidth = 0;
    //һ����Ⱦ��ͼ����ĸ߶�
    int mapBlockHeight = 0;
    //һ����Ⱦ��ͼ�����ش�С
    float mapBlockSize = 0;

    //��ͼ���½�����
    // Vec2 mapPosition;
    //������Ϣ
    std::string physicsBody;

    //��ͼ������
    std::vector<basic_GameMapBuilding*> buildings;

    //�ӳ���Ⱦ��ͼʱ,�����Ⱦ���ڼ�����

    int init_Width = 0;
    int init_Height = 0;
    bool initFinish = false;
};

//��Ϸ�еĽ�����
class basic_GameMapBuilding : public basic_GameSprite {
    // TODO:����
};

#endif
