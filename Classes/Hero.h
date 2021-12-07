#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"
USING_NS_CC;

#include <map>
#include <string>
#include <vector>

#include "GameItem.h"
#include "basic_GameSprite.h"

class HeroItemBag final {
public:
    using ItemMap = std::map<GameItemType, std::vector<basic_GameItem*>>;

    HeroItemBag();
    ~HeroItemBag();
    /**
     *���һ����Ʒ
     *@param item:��Ʒ
     *@return ��
     */
    void addItem(basic_GameItem* item);

    /**
     *�Ƴ���Ʒ
     *@param item:Ҫ�Ƴ�����Ʒ
     *@return ��
     */
    void removeItem(basic_GameItem* item);

    /**
     *�õ�������Ʒ
     *@param ��
     *@return ������Ʒ
     */
    const ItemMap& getAllItems();

private:
    ItemMap items;
};

enum class HeroActionType {
    LWalk,
    RWalk,
    LRun,
    RRun,
};

class basic_Hero : public basic_GameSprite {
public:
    /**
     *��ȡѪ��
     *@param ��
     *@return Ѫ��
     */
    virtual int getHitPoint() final;

    /**
     *����Ѫ��
     *@param hitPoint:Ѫ��
     *@return ��
     */
    virtual void setHitPoint(int hitPoint) final;

    /**
     *��ȡ����
     *@param ��
     *@return ����
     */
    virtual int getEnergy() final;

    /**
     *��������
     *@param energy:����
     *@return ��
     */
    virtual void setEnergy(int energy) final;

    /**
     *�����Ʒ��
     *@param ��
     *@return ��Ʒ��
     */
    virtual HeroItemBag& getItemBag() final;

    /**
     *����Ӣ�۶���״̬
     *@param type:״̬
     *@return ��
     */
    virtual void setActionType(HeroActionType type);

    /**
     *��ȡӢ�۶���״̬
     *@param
     *@return ״̬
     */
    virtual HeroActionType getActionType();

    /**
     *����ײ
     *@param contactTarget: ײ��Ŀ��
     *@return ��
     */
    virtual void onContact(basic_GameSprite* contactTarget) override;

    /**
     *�������̹���
     *@param pos: ���ָ���Ĺ���λ��
     *@return ��
     */
    virtual void attackNear(const Vec2& pos) = 0;

    /**
     *����Զ�̹���
     *@param pos: ���ָ���Ĺ���λ��
     *@param holdTime: ������ʱ��,��
     *@return ��
     */
    virtual void attackFar(const Vec2& pos, float holdTime) = 0;

private:
    HeroActionType actionType;
    HeroItemBag itemBag;
    int hitPoint = 0;
    int energy = 0;
};

#endif
