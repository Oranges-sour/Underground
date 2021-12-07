#ifndef __RESOURCES_MANAGER_H__
#define __RESOURCES_MANAGER_H__



#include "cocos2d.h"
USING_NS_CC;

#include <string>
#include <vector>
#include <map>

/*��Ϸ��Դ������.�Զ���Ӻ�ɾ����Դ
��ͬ��Դ���͵ļ�ֵ�����ظ�,������Ӱ��.��ͬ������Դ��һ������һ��
*/
class ResourcesManager
{
public:
	/**
	*����,�������Ƿ��������������Զ��ͷ���Դ
	*@param autoReleaseResources:�Ƿ��Զ��ͷ���Դ
	*/
	ResourcesManager(bool autoReleaseResources = true);

	/**
	*����,Ĭ�ϻ��Զ��ͷ���Դ,���ڹ��캯��������
	*/
	~ResourcesManager();

	/**
	*���shader��Դ����,��һ�����Զ���shader����cache��
	*@param vsh:������ɫ���ļ�����
	*@param fsh:Ƭ����ɫ���ļ�����
	*@param key:��ֵ
	*@return ��
	*/
	void addShader(const std::string& vsh, const std::string& fsh, int key);

	/**
	*��Ӿ����
	*@param filename:�ļ�����
	*@return ��
	*/
	void addSpriteSheet(const std::string& filename);

	/**
	*���һ�龫��֡,ʹ�ü�ֵ��ȡʱ���᷵����һ���龫��֡
	*�������ӽ����Ǿ���֡���ļ�����,��������.
	*�������Ҫ����ResourcesManager::addSpriteSheet(...)�����
	*@param frames:һ�龫��֡
	*@param key:��ֵ
	*@return ��
	*/
	void addSpriteFrames(const std::vector<std::string>& frames, int key);

	/**
	*��Ӳ���,����ӽ��ڴ�
	*@param filename:�ļ�����
	*@param key:��ֵ
	*@return ��
	*/
	void addTexture(const std::string& filename, int key);

	/**
	*��Ӿ����������״(plist)
	*@param filename:�ļ�����
	*@return ��
	*/
	void addPhysicsShapes(const std::string& filename);

	/**
	*���һ�����������Body������
	*@param name:Body������
	*@param key:��ֵ
	*@return ��
	*/
	void addPhysicsBody(const std::string& name, int key);

	/**
	*���һ����������Body
	*@param key:��ֵ
	*@return Body������
	*/
	const std::string& getPhysicsBody(int key);

	/**
	*���һ��shader
	*@param key:��ֵ
	*@return GLProgram��ָ��
	*/
	GLProgram* getShader(int key);

	/**
	*���һ�龫��֡
	*@param key:��ֵ
	*@return һ�龫��֡
	*/
	const std::vector<std::string>& getSpriteFrames(int key);

	/**
	*��ò�������
	*@param key:��ֵ
	*@return ��������
	*/
	const std::string& getTexture(int key);

	/**
	*�ͷ���Դ������,���ͷ������ɴ�ResourcesManager�������Դ.
	*��Ĭ����������������,���ڹ��캯���о����Ƿ��Զ�����
	*��������,�����,shader��
	*@param ��
	*@return ��
	*/
	void releaseResources();
private:
	std::map<int, GLProgram*> shaderPool;
	std::map<int, std::string> texturePool;
	std::map<int, std::string> physicsBodyPool;
	std::map<int, std::vector<std::string>> spriteFramePool;
	std::vector<std::string> spriteSheetPool;
	std::vector<std::string> physicsShapePool;

	bool autoReleaseResources = true;
};

#endif
