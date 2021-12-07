#ifndef __PHYSICS_MANAGER_H__
#define __PHYSICS_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <tuple>
#include <vector>

#include "iVec2.h"

class basic_GameSprite;

class PhysicsManager final : public Node {
public:
    /**
     *�������������
     *@param ��
     *@return ������ָ��
     */
    static PhysicsManager* create();

    /**
     *������Ϸ���������״̬,��������
     *@param gameObjects:��������Ϸ�еĶ���.���ɴ�������ж�.
     *@return ��
     */
    void updatePhysics(const std::vector<basic_GameSprite*>& gameObjects);

private:
    /**
     *��������ǽ����ײ(��ס����)
     *@param sprite: ����
     *@return ��
     */
    void contactWithWall(basic_GameSprite* sprite);

    /**
     *����ˮƽ�ƶ�
     *@param speedVec: �ٶ�ʸ��
     *@param moveSpeed: �ƶ��ٶ�
     *@param checkPoint: ������Ľ��ڵ�ͼ�е�����,
                        ˳��:����, ����, ����, ����
     *@param heroBoundingBox
     *@return ����������Ҫ��������
     */
    Vec2 horizonMove(Vec2& speedVec,
                     const std::tuple<const iVec2&, const iVec2&, const iVec2&,
                                      const iVec2&>& checkPoint,
                     const Rect& boundingBox);

    /**
     *������ֱ�ƶ�
     *@param speedVec: �ٶ�ʸ��
     *@param moveSpeed: �������ƶ��ٶ�
     *@param checkPoint: ������Ľ��ڵ�ͼ�е�����,
                        ˳��:����, ����, ����, ����
     *@param heroBoundingBox
     *@return ����������Ҫ��������
     */
    Vec2 verticalMove(Vec2& speedVec,
                      const std::tuple<const iVec2&, const iVec2&, const iVec2&,
                                       const iVec2&>& checkPoint,
                      const Rect& boundingBox);

    /**
     *��ʼ�����������
     *@param ��
     *@return ��
     */
    virtual bool init() override;

    /**
     *����������ײ
     *@param
     *@return
     */
    void processContact(PhysicsContact& conatct);
};

#endif