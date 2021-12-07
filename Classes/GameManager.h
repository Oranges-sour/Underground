#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <array>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#include "GameSpriteType.h"
#include "iVec2.h"

class basic_GameMapBuilding;
class PhysicsManager;
class ControlManager;
class GameRenderer;
class GameMap;
class GameMapTile;
class ResourcesManager;
class basic_Hero;
class basic_GameSprite;

enum class GameRenderOrder { map = 1, user0, user1, user2, user3 };

/*��ʾ:
 * 1.����UI��Ԫ�������GameManager����Ⱦ��,������GameManager����Ⱦ�����յ���Դ��Ӱ��
 */

class GameManager final : public Node {
public:
    /**
     *����ʵ��
     *@param parent: ���ʵ���Ľڵ�
     *@return ��
     */
    static void createInstance();

    /**
     *����ʵ��,���Զ������ʵ���Ľڵ����Ƴ��Լ���������Ϸ����
     *@param ��
     *@return ��
     */
    static void destroyInstance();

    /**
     *���ʵ��
     *@param ��
     *@return ʵ��ָ��
     */
    static GameManager* getInstance();

private:
    static GameManager* instance;

public:
    using GameSpriteContainer =
        std::map<GameSpriteType, std::set<basic_GameSprite*>>;

    /**
     *����Դ������ɺ����
     *@param ��
     *@return ��
     */
    void initManager();

    /**
     *��cocos��schedulerÿһ֡����,֡��Ⱦ֮ǰ����
     *@param dt: ������һ�ε��õļ��ʱ��
     */
    std::vector<basic_GameSprite*> mainUpdateBeforeRender(float dt);

    /**
    *��֡��Ⱦ֮�����
    *@param dt: ������һ�ε��õļ��ʱ��
    */
    void mainUpdateAfterRender(float dt);

    /**
     *��ͣ��Ϸ
     *@param ��
     *@return ��
     */
    void pauseGame();

    /**
     *��ʼ��Ϸ(������ͣ)
     *@param ��
     *@return ��
     */
    void resumeGame();

    /**
     *��Ϸ�Ƿ���ͣ
     *@param ��
     *@return true��Ϸ��ͣ false��Ϸδ��ͣ
     */
    bool isGamePause();

    /**
     *�����Ϸ����,��������Ⱦ��
     *@param gameObject:�����ָ��
     *@param layerOrder: ��Ⱦ��
     *@return ��
     */
    void addGameSprite(basic_GameSprite* gameObject,
                       GameRenderOrder layerOrder);

    /**
     *�����ͨ�ڵ�,ʹ��removeFromParent()�Ƴ�
     *@param node:�ڵ�ָ��
     *@param layerOrder: ��Ⱦ��
     *@return ��
     */
    void addNode(Node* node, GameRenderOrder layerOrder);

    /**
     *�Ƴ���Ϸ����
     *@param ����ָ��
     *@return ��
     */
    void removeGameSprite(basic_GameSprite* gameObject);

    /**
     *��ȡ��Դ������
     *@param ��
     *@return ��Դ��������ָ��
     */
    ResourcesManager* getResourcesManager();

    /**
     *�����Ϸ��Ⱦ��
     *@return ��Ⱦ����ָ��
     */
    GameRenderer* getGameRenderer();

    /**
     *�����Ϸ��ͼ
     *@param ��
     *@return ��Ϸ��ͼ��ָ��
     */
    GameMap* getGameMap();

    /**
     *��������ڸ��¾��������ڵľ���
     *@param ��
     *@return ����
     */
    const GameSpriteContainer& getAllSprites();

    /**
     *����
     *@param ��
     *@return ��
     */
    virtual void cleanup() override;

    //����������Ϸ����(Ӣ��,���Ƹ˵ȵ�)

    //����Ӣ��,���Զ�������Ⱦ
    void setHero(basic_Hero* hero);

    //�����ƶ����Ƹ�,��������Ⱦ(��"��ʾ1")
    // void setMoveJoystick(Joystick* joystick);

    //���Ӣ��
    basic_Hero* getHero();

private:
    /**
     *��ʼ����Ϸ������
     *@param ��
     *@return ��
     */
    virtual bool init() override;

    /**
     *����Ӣ�۵��ƶ�
     *@param ��
     *@return ��
     */
    void updateHeroMove();

private:
    //һЩ��Ҫ������

    /////////////////////////////////////////////////////////

    //��ϷӢ��
    basic_Hero* hero = nullptr;

private:
    //���������
    PhysicsManager* physicsManager = nullptr;
    //��Ϸ��Ⱦ��
    GameRenderer* gameRenderer = nullptr;
    //��Ϸ��ͼ
    GameMap* gameMap = nullptr;
    //������
    ControlManager* controlManager = nullptr;
    //��Ϸ�Ƿ���ͣ
    bool gamePauseState = false;
    //������Ϸ����
    GameSpriteContainer gameObjects;
    //��һ֡��Ҫɾ���Ķ���
    std::vector<basic_GameSprite*> needToErase;
    //��һ֡��Ҫ��ӵĶ���
    std::vector<std::pair<basic_GameSprite*, GameRenderOrder>> needToAdd;

    //��Դ������
    //�û���Դ
    ResourcesManager* userResourcesManager = nullptr;
};

#endif
