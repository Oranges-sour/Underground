#ifndef __GAME_RENDERER_H__
#define __GAME_RENDERER_H__

#include "cocos2d.h"
USING_NS_CC;

#include <memory>
#include <vector>

#include "GameSpriteType.h"

class basic_GameSprite;

//һ��֡��"��"
struct ShakeBag {
    //��б�Ƕ�
    float angle = 0;
    //����ʱ��
    float time = 0;
    //���
    float amplitude = 0;
    //ÿ�μ��ٵ����
    float delta = 0;
};

//��Ϸ��Ⱦ��
class GameRenderer final : public Node {
public:
    static GameRenderer* create();

    /**
     *��ʼ����Ⱦ��
     *@param
     *@return
     */
    virtual bool init() override;

    /**
     *��ӽڵ�
     *@param �ڵ�ָ��
     *@return ��
     */
    void addNode(Node* node, int zOrder);

    /**
     *�Ƴ��ڵ�
     *@param �ڵ�ָ��
     *@return ��
     */
    void removeNode(Node* node);

    /**
     *������Ⱦ
     *@param ��
     *@return ��
     */
    void _update(const std::vector<basic_GameSprite*>& gameObjects,
                 const Vec2& heroPosition, const Vec2& mousePosition);

    /**
     *���¹�Դ,��Ҫÿ֡����
     *@param gameObjects:��������Ϸ�еĶ���.���ɴ�����й�Դ�ж�
     *@return ��
     */
    SpriteFrame* updateLight(const std::vector<basic_GameSprite*>& gameObjects);

    /**
     *���������֡��
     *@param ��
     *@return ��
     */
    void updateShake();

    /**
     *��������������½�����
     *@param position:����
     *@return ��
     */
    void setGameCameraPosition(const Vec2& position);

    /**
     *����������ǰ�����½�����
     *@param ��
     *@return ���½�����
     */
    const Vec2& getGameCameraPosition();

    /**
     *�����Ƿ��������������
     *@param pos: ����
     *@return �Ƿ���������
     */
    bool isInCamera(const Vec2& pos);

    /**
     *�����Ƿ��������������
     *@param rect: ����
     *@return �Ƿ���������
     */
    bool isInCamera(const Rect& rect);

    /**
     *���������,Ƶ��Ĭ��30Hz
     *@param angle: ��б�Ƕ�
     *@param amplitude: ���(���ؾ���
     *@param time: ����ʱ��
     *@return ��
     */
    void shakeCamera(float angle, float amplitude, float time);

    /**
     *������������ж������Ⱦ֮���ȫͼ��Ⱦshader
     *@param key:�û���Դ�������е�shader��ֵ
     *@param order:��Ⱦ��˳��(��Сorder����)
     *@return ��
     */
    // void addAfterDrawShader(int key, int order);

    /**
     *�Ƴ�ȫͼ��Ⱦshader
     *@param key:�û���Դ�������е�shader��ֵ
     *@return ��
     */
    // void removeAfterDrawShader(int key);

    virtual void cleanup() override;

private:
    std::vector<std::shared_ptr<ShakeBag>> shakes;

    unsigned int shakeCount = 0;

    RenderTexture* lightRenderTexture = nullptr;

    //����Ļ��ʾͼƬ�ľ���
    Sprite* lightDraw = nullptr;
    Node* objectRenderer = nullptr;
    Node* finalDraw = nullptr;
    //���������Ҫ���������
    Vec2 cameraPosNeed;
};

#endif