#ifndef __GAME_DEFAULT_H__
#define __GAME_DEFAULT_H__

#include <functional>

#include "cocos2d.h"
USING_NS_CC;

typedef unsigned int UINT;

namespace GameDef {
//��Ϸһ"��"��ʱ��(1/60��)
constexpr float GAME_TICK_TIME = 1.0f / 60;

constexpr UINT GAME_MAP_SIZE_64 = 64;
constexpr UINT GAME_MAP_SIZE_128 = 128;
constexpr UINT GAME_MAP_SIZE_192 = 192;
constexpr UINT GAME_MAP_SIZE_256 = 256;
}  // namespace GameDef

namespace MyTool {
/**
*����ڴ�vector���µĶ��̲߳�ָ���
*@param vecSize:vector�Ĵ�С
*@param callFunc:���º���
*@return ��
*@����:

std::vector<int> vec;
callInThread(vec.size(),
[&](UINT from, UINT to, UINT threadNumber)
{
        for(int x = from; x < to; ++x)
        {
                vec[x] += 1;
        }
});
*/
void callInThread(
    const size_t vecSize,
    const std::function<void(UINT from, UINT to, UINT threadNumber)>& callFunc);

/**
*���callInThread()��ʹ�õ�����߳�����
*@return ����߳�����
*/
UINT getMaxThreadNumber();

/**
 *�����Ƿ�ȫ��,ÿ����һ��,����״̬�ͻ����һ��
 *@param ��
 *@return ��
 */
void setFullScreen();
}  // namespace MyTool

#endif
