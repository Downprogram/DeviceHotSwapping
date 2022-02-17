/*********************************
描述: 监听设备热插拔 可以在main函数创建单例，进行全局监听
作者:zhongzhipeng
***********************************/
#pragma once
#include "HotSwappingInterface.h"
#include "CMMNotificationClient.h"
#include <memory>

class ListeningStatus
{
private:
	ListeningStatus();
	~ListeningStatus();
public:
	static ListeningStatus* GettInstance();
	static void Release();
	void Add(const photSwappingInterface&);
	void Remove(const photSwappingInterface&);
private:
	IMMDeviceEnumerator* pEnumerator = nullptr;
	CMMNotificationClient pNotify;
	static ListeningStatus* m_pListen;
};