/*********************************
描述:用于监听设备热插拔
	新的设备驱动加入时会触发 OnDeviceAdded()
	设备驱动被移除时会触发 OnDeviceRemoved()
	设备的热插拔会触发 OnDeviceStateChanged()
	每次热插拔都会触发 OnDefaultDeviceChanged() 可以获取设备类型

	文档：https://docs.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immnotificationclient
作者:zhongzhipeng
***********************************/
#pragma once
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <mutex>
#include <string>
#include <deque>
#include "HotSwappingInterface.h"

//监听热插拔主要对象，对应的事件将会转发给通知对象 HotSwappingInterface
class CMMNotificationClient : public IMMNotificationClient
{
private:
	
	LONG _cRef;
	IMMDeviceEnumerator *_pEnumerator;
	std::deque<photSwappingInterface> m_vObservers;
	std::mutex m_lock;//热插拔监听是多线程，需要加锁
public:

	CMMNotificationClient();
	~CMMNotificationClient();

	ULONG STDMETHODCALLTYPE AddRef();

	ULONG STDMETHODCALLTYPE Release();


	HRESULT STDMETHODCALLTYPE QueryInterface(
		REFIID riid, VOID **ppvInterface);

	
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId);

	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);;

	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);

	//设备状态改变时触发
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState);


	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId,const PROPERTYKEY key)
	{
		return S_OK;
	}

public:
	void Add(const photSwappingInterface& Interface);
	void Remove(const photSwappingInterface&);
private:
	tstring AnalysisDeviceName(LPCWSTR);
};