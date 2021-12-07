#ifndef __GAME_ITEM_H__
#define __GAME_ITEM_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"
class basic_Hero;

enum class GameItemType { unknow, nearWeapon, farWeapon };

/*
��Ϸ����Щ����(ֻ)��Ӣ�ۼ����ʹ�õ���Ʒ
�ڵ�ͼ����ʾ�ľ���
*/
class basic_GameItemSprite : public basic_GameSprite {
public:
    basic_GameItemSprite() {}
    virtual ~basic_GameItemSprite() {}

    /**
     *��Ʒ��������,ʵ�����˺����Զ�Ӣ�۽���һЩ����
     *@param hero:�������Ʒ��Ӣ��
     *@return ��
     */
    virtual void itemPickedUp(basic_Hero* hero) = 0;

    /**
     *��þ�������
     *@param ��
     *@return ����
     */
    virtual GameSpriteType getGameSpriteType() final override;

    // item����Ҫһ�����ײ���
    virtual void onContact(basic_GameSprite* contactTarget) final override{};
    virtual void getContact(basic_GameSprite* contactFrom) final override{};
};

class basic_GameItem {
public:
    basic_GameItem() {}
    virtual ~basic_GameItem() {}

    /**
     *�����Ʒ����
     *@param ��
     *@return ��Ʒ����
     */
    virtual GameItemType getItemType() const { return GameItemType::unknow; }

    /**
     *ʹ����Ʒ
     *@param gameSprite: ʹ����Ʒ�ľ���
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite) = 0;
};

#endif
