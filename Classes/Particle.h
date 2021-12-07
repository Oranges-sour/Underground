#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "cocos2d.h"
USING_NS_CC;

#include <string>
using namespace std;

#include "basic_GameSprite.h"

class basic_Particle : public basic_GameSprite {
public:
    /**
     *��������
     *@param frame: ����֡
     *@param speedRate: �ٶȱ���
     *@param rotaterate: ��ת����
     *@return ���Ӿ���
     */
    static basic_Particle* create(const string& frame, float liveTime,
                           float speedRate = 1.0f, float rotateRate = 0.0f);

    /**
     *��ʼ������
     *@param liveTime:���ӵĴ��ʱ��
     *@return �Ƿ��ʼ���ɹ�
     */
    virtual bool initGameSprite(const string& frame, float liveTime,
                                float speedRate, float rotateRate);

    void _update();

    virtual void kill()override;

private:
    Vec2 addVec;
    float addRotation;
};

#endif