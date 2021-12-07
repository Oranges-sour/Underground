#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
USING_NS_CC;

#include "basic_GameSprite.h"

class basic_Bullet : public basic_GameSprite {
public:
    /**
     *�����ӵ�
     *@param startPosition:�ӵ�����ʼ����
     *@return ��
     */
    virtual void shoot(const Vec2& startPosition) = 0;

    /**
     *�����ӵ��Ƿ������ײ
     *@param finish:�Ƿ�
     *@return ��
     */
    void setContactFinish(bool finish) noexcept { contactFinish = finish; }

    /**
     *�Ƿ���ײ��
     *@param ��
     *@return �Ƿ���ײ��
     */
    bool isContactFinish() noexcept { return contactFinish; }

    /**
     *���ù�����
     *@param damage: ������
     *@return ��
     */
    void setDamage(int damage) { this->damage = damage; }

    /**
     *����ӵ�������
     *@param ��
     *@return ������
     */
    int getDamage() { return damage; }

private:
    //�ӵ����˺�
    int damage = 0;
    //��ֹ����ܵ�������ײ��ɶ���˺�
    bool contactFinish = false;
};

#endif
