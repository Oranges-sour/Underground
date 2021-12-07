#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "cocos2d.h"
USING_NS_CC;

//#include "GameManager.h"
#include "MySchedule.h"

class GameMapTile;

#include <map>
#include <string>
#include <vector>

#include "GameSpriteType.h"

struct GameSpritePhysicsInfo {
    //�������״�Լ���ײ(PhysicsEditor�е�����)
    std::string physicsShape;

    //�Ƿ��ܵ�����Ӱ��
    bool gravityEffect = false;

    //�Ƿ������ײ������ true:������ײ������ flase:ֻ�ᱻ������ײ��
    bool activeContact = false;

    //�Ƿ���Ҫ����ģ��
    bool needPhysics = false;

    //�Ƿ�����ͼ��ײ
    bool contactWithWall = false;

    //�����Ƿ�վ�ڵ���(ֻ�м�������ͼ����ײ��ֵ������
    bool onGround = false;

    //��������(kg),�������Ħ����ʹ��
    float mass = 1.0f;
};

/*��Ϸ�Ļ�����,��Ϸ�г��ֵ��κ�Ԫ��(���ӵ�,Ӣ��,����,����������Ʒ)
��Ҫ�Ӵ˶���̳ж���.
-----
������ײ��˵��:
        ����һ������,��Ȼ����֡�ı�,����ײ�߽粻�ı�.����������ƽ�������˵��:
           һ�������,����֡�ĸı�ֻ��С��Χ�ĸı�,����Ҫ����������ײ�߽�.����д�Χ
           �ĸı�,Ԥ���ǹ�������Ч,����ʹ������ķ�ʽ���������������,��Ӧʹ�ö�����Ӱ��ϴ��
           ��ײ�߽�(ÿ���������ñ߽綼Ҫ���¼���������).
*/
class basic_GameSprite : public Sprite,
                         public CocosSchedule,
                         public LogicSchedule {
public:
    basic_GameSprite() {}
    virtual ~basic_GameSprite() {}

    /**
     *����ײ��������������,�˾�����ײ����
     *@param contactTarget: ��ײ��������
     *@return ��
     */
    virtual void onContact(basic_GameSprite* contactTarget);

    /**
     *�����ܵ�ײ��,����ײ��������
     *@param contactFrom:ײ��������
     *@return ��
     */
    virtual void getContact(basic_GameSprite* contactFrom);

    /**
    *ɱ���˾���,������������Ч(����ʵ�����).
    *������������Ч��,����ֱ�ӵ��� void removeFromParent()
    *@param ��

    *@return ��
    */
    virtual void kill();

    /**
     *��ö��������
     *@param ��
     *@return ��������
     */
    virtual GameSpriteType getGameSpriteType();

    /**
     *���Buff,����������ʱ��,����Ѿ���ô�buffʱ������
     *@param objectBuffType: buff������
     *@param time: buff�ĳ���ʱ��
     *@return ��
     */
    // virtual void addBuff(GameObjectBuffType objectBuffType, float time)final;

    /**
     *����Buff,�������ٵ�ʱ��,ʱ�����С��0��ֱ���Ƴ�buff
     *@param objectBuffType: buff������
     *@param time: buff�ļ���ʱ��
     *@return ��
     */
    // virtual void subBuff(GameObjectBuffType objectBuffType, float time)final;

    /**
     *��õ�ǰ�ܵ�������buff�Լ�ʣ��ʱ��
     *@param ��
     *@return buff�������Լ���Ӧ��ʱ��
     */
    // virtual const std::map<GameObjectBuffType, float>& getBuff()final;

    /**
     *�Ƿ��ܵ���param��������buff,����ʣ���ʱ���Ƿ񳬹�������������ֵ(��ѡ��)
     *@param objectBuffType: buff������
     *@param time: ʣ��ʱ�����ֵ(��ѡ��)
     *@return �ܵ�:ture  ���ܵ�:false
     */
    // virtual bool isBuff(GameObjectBuffType objectBuffType, float time =
    // -1.0f)final;

    /**
     *���þ����������Ϣ,����ײ���������
     *@param physicsInfo: ������Ϣ
     *@return ��
     */
    virtual void setPhysicsInfo(const GameSpritePhysicsInfo& physicsInfo) final;

    /**
     *��þ����������Ϣ
     *@param ��
     *@return �����������Ϣ
     */
    virtual GameSpritePhysicsInfo& getPhysicsInfo() final;

    /**
     *����������ײ
     */
    virtual void uploadPhysicsBody() final;

    /**
     *ж��������ײ
     */
    virtual void unloadPhysicsBody() final;

    /**
     *������й�Դ��Ⱦ����
     *@param ��
     *@return ��Դ����
     */
    virtual const std::map<int, Sprite*>& getAllLightSprites() final;

    /**
     *��ù�Դ��Ⱦ����
     *@param key: ��ֵ
     *@return ��Դ����
     */
    virtual Sprite* getLightSprite(int key) final;

    /**
     *��ӹ�Դ����
     *@param ��Դ����
     *@return ��
     */
    virtual void addLightSprite(Sprite* lightSprite, int key) final;

    /**
     *�Ƴ���Դ����
     *@param ��Դ����
     *@return ��
     */
    virtual void removeLightSprite(int key) final;

    /**
     *��ʼ��
     *@param ��
     *@return �Ƿ��ʼ���ɹ�
     */
    virtual bool initGameSprite();

    /**
     *�������
     *@param ��
     *@return ��
     */
    virtual void cleanup() override;

public:
    //ÿһ֡���˶�ʸ��(��ǰ֡����ͻᱻ���)
    Vec2 moveVec;
    //�˶����ٶ�
    Vec2 speedVec;
    //���ٶ�,ÿһ֡���ٶȶ���Ӱ���ٶ�
    Vec2 accVec;

private:
    // buff��������ʱ���ӳ���
    // std::map<GameObjectBuffType, float> buffs;

    //������ײ
    PhysicsBody* physicsBody = nullptr;
    //��Դ��Ⱦ����
    std::map<int, Sprite*> lightSprites;
    //������Ϣ
    GameSpritePhysicsInfo physicsInfo;
};

#endif
