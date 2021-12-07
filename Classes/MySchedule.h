#ifndef __LOGIC_SCHEDULE_H__
#define __LOGIC_SCHEDULE_H__

#include <map>
#include <string>
#include <functional>


/*
�߼��̵߳Ķ�ʱ��
*/
class LogicSchedule
{
public:
	/**
	*�߼���ʱ����������
	*@param dt:������һ�θ��µ�ʱ��
	*@return ��
	*/
	void logicScheduleUpdate(float dt);

	/**
	*�߼���ʱ��,ÿһ֡������
	*@param callback:�ص�����
	*@param key:��ֵ
	*@return ��
	*/
	void logicSchedule(const std::function<void(float)>& callback, const std::string& key);

	/**
	*�߼���ʱ��
	*@param callback:�ص�����
	*@param interval:���¼��(��
	*@param key:��ֵ
	*@return ��
	*/
	void logicSchedule(const std::function<void(float)>& callback, float interval, const std::string& key);

	/**
	*�߼���ʱ��
	*@param callback:�ص�����
	*@param interval:���¼��(��
	*@param repeat:���´���
	*@param delay:��һ�θ��µ��ӳ�ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void logicSchedule(const std::function<void(float)>& callback, float interval, int repeat, float delay, const std::string& key);

	/**
	*�߼���ʱ��,ֻ����һ��
	*@param callback:�ص�����
	*@param delay:��һ�θ��µ��ӳ�ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void logicScheduleOnce(const std::function<void(float)>& callback, float delay, const std::string& key);

	/**
	*ȡ���߼���ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void logicUnschedule(const std::string& key);
private:
	struct SchedulePack
	{
		std::function<void(float)> callBack;
		int repeat;
		float delay;
		float interval;
		float timeCount;
	};

	std::map<std::string, SchedulePack> scheduleMap;
};

/*
���̵߳Ķ�ʱ��
*/
class CocosSchedule
{
public:
	/**
	*�߼���ʱ����������
	*@param dt:������һ�θ��µ�ʱ��
	*@return ��
	*/
	void cocosScheduleUpdate(float dt);

	/**
	*�߼���ʱ��,ÿһ֡������
	*@param callback:�ص�����
	*@param key:��ֵ
	*@return ��
	*/
	void cocosSchedule(const std::function<void(float)>& callback, const std::string& key);

	/**
	*�߼���ʱ��
	*@param callback:�ص�����
	*@param interval:���¼��(��
	*@param key:��ֵ
	*@return ��
	*/
	void cocosSchedule(const std::function<void(float)>& callback, float interval, const std::string& key);

	/**
	*�߼���ʱ��
	*@param callback:�ص�����
	*@param interval:���¼��(��
	*@param repeat:���´���
	*@param delay:��һ�θ��µ��ӳ�ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void cocosSchedule(const std::function<void(float)>& callback, float interval, int repeat, float delay, const std::string& key);

	/**
	*�߼���ʱ��,ֻ����һ��
	*@param callback:�ص�����
	*@param delay:��һ�θ��µ��ӳ�ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void cocosScheduleOnce(const std::function<void(float)>& callback, float delay, const std::string& key);

	/**
	*ȡ���߼���ʱ��
	*@param key:��ֵ
	*@return ��
	*/
	void cocosUnschedule(const std::string& key);
private:
	struct SchedulePack
	{
		std::function<void(float)> callBack;
		int repeat;
		float delay;
		float interval;
		float timeCount;
	};

	std::map<std::string, SchedulePack> scheduleMap;
};


#endif // !__LOGIC_SCHEDULE_H__
