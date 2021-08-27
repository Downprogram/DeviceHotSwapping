#include "ListeningStatus.h"
#include "stdafx.h"
ListeningStatus* ListeningStatus::m_pListen = nullptr;

ListeningStatus::ListeningStatus()
{
	::CoInitialize(nullptr);
	CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	//绑定
	pEnumerator->RegisterEndpointNotificationCallback((IMMNotificationClient*)&pNotify);
}

ListeningStatus::~ListeningStatus()
{
	//注册
	pEnumerator->UnregisterEndpointNotificationCallback((IMMNotificationClient*)&pNotify);
	if (pEnumerator != nullptr)
	{
		pEnumerator->Release();
		pEnumerator = nullptr;
	}
	::CoUninitialize();
}

ListeningStatus * ListeningStatus::GettInstance()
{
	if (!m_pListen)
		m_pListen = new ListeningStatus{};
	return m_pListen;
}

void ListeningStatus::Release()
{
	if (m_pListen)
	{
		delete m_pListen;
		m_pListen = nullptr;
	}
}

//添加通知对象
void ListeningStatus::Add(const photSwappingInterface& Interface)
{
	pNotify.Add(Interface);
}

//移除通知对象
void ListeningStatus::Remove(const photSwappingInterface& Interface)
{
	pNotify.Remove(Interface);
}
