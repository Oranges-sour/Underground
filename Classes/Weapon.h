#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "cocos2d.h"
USING_NS_CC;

#include "GameItem.h"

class basic_Weapon : public basic_GameItem {
public:
    basic_Weapon() {}
    virtual ~basic_Weapon() {}

    /**
     *ʹ����Ʒ
     *@param gameSprite: ʹ�������ľ���
     *@param from: ��ʼ������
     *@param to: �յ�����
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) = 0;

private:
    /**
     *��ԭʼ��ʹ����Ʒ,��weapon�б�����
     *@param ��
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite) final override {}
};

//��������
class NearWeapon : public basic_Weapon {
public:
    /**
     *ʹ������
     *@param gameSprite: ʹ�������ľ���
     *@param from: ��ʼ������
     *@param to: �յ�����(������ʾһ��ʹ�õķ���
     *@param range: �����˺��뾶
     *@param angle: ��Բ���Ž�
     *@param holdTime: ����ʱ��
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to, float range, float angle,
        float holdTime = 0.0f)
    {
    }

private:
    /**
     *����
     *@param ��
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) final override
    {
    }
};

//Զ������
class FarWeapon : public basic_Weapon {
public:
    FarWeapon() {}
    virtual ~FarWeapon() {}

    /**
     *ʹ����Ʒ
     *@param gameSprite: ʹ�������ľ���
     *@param from: ��ʼ������
     *@param to: �յ�����
     *@param holdTime: ����ʱ��
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to, float holdTime = 0.0f)
    {
    }

private:
    /**
     *����
     *@param ��
     *@return ��
     */
    virtual void useItem(basic_GameSprite* gameSprite, const Vec2& from,
        const Vec2& to) final override
    {
    }
};

#endif