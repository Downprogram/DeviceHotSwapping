#include "stdafx.h"
#include "CMMNotificationClient.h"
#include "DeviceInfo.h"

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

CMMNotificationClient::CMMNotificationClient() 
{
	this->_cRef = 1;
	_pEnumerator = nullptr;
}



CMMNotificationClient::~CMMNotificationClient()
{
	if (_pEnumerator != nullptr)
	{
		_pEnumerator->Release();
		_pEnumerator = nullptr;
	}
}

ULONG STDMETHODCALLTYPE CMMNotificationClient::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

ULONG STDMETHODCALLTYPE CMMNotificationClient::Release()
{
	ULONG ulRef = InterlockedDecrement(&_cRef);
	if (0 == ulRef)
	{
		delete this;
	}
	return ulRef;
}

HRESULT STDMETHODCALLTYPE CMMNotificationClient::QueryInterface(REFIID riid, VOID ** ppvInterface)
{
	return S_OK;
}

/*********************************
描述:
      通知客户端，特定设备的默认音频端点设备已经改变
参数:
        flow: 设备类型枚举值
		role：系统分配给该设备端点的功能
		pwstrDefaultDeviceId：指向识别音频端点设备的端点ID字符串的指针
返回值:
        
备注:
        
***********************************/
HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
	this->m_lock.lock();
	std::deque<photSwappingInterface> vbs = this->m_vObservers;
	this->m_lock.unlock();
	tstring pszFlow;
	switch (flow)
	{
	case eRender:
		pszFlow = L"eRender";
		break;
	default:
		pszFlow = L"eCapture";
		break;
	}
	if (/*(role == eCommunications)&&*/(pwstrDeviceId != nullptr))//不进行检测将调用一次或一次以上
	{
		DeviceInfo info;
		info.deviceName = AnalysisDeviceName(pwstrDeviceId);
		info.deviceGuid = pwstrDeviceId;
		for (const auto& It : vbs)
			It->deviceTypeChange(info, pszFlow);
	}
	return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
	//新的驱动添加
	return S_OK;
}

HRESULT __stdcall CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
	//驱动移除
	return S_OK;
}

/*********************************
描述:
        向所有窗口发送消息
参数:
        pwstrDeviceId：指向识别音频端点设备的端点ID字符串的指针
		dwNewState：状态变化
返回值:
        
备注:
        
***********************************/
HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{

	this->m_lock.lock();
	std::deque<photSwappingInterface> vbs{ this->m_vObservers };
	this->m_lock.unlock();
	DeviceInfo info;
	tstring pszFlow;
	info.deviceName = AnalysisDeviceName(pwstrDeviceId);
	info.deviceGuid = pwstrDeviceId;
	for (const auto& It : vbs)
	{
		switch (dwNewState)
		{
		case DEVICE_STATE_ACTIVE://设备活动
			It->deviceInsert(info);
			break;
		case DEVICE_STATE_DISABLED://设备被禁用
			It->deviceDisable(info);
			break;
		case DEVICE_STATE_NOTPRESENT://设备未准备
			It->decvicePreparation(info);
			break;
		case DEVICE_STATE_UNPLUGGED://设备拔出
			It->deviceUnplug(info);
			break;
		default:
			It->deviceStateAll(info, dwNewState);//自己做处理
			break;
		}
	}
	return S_OK;
}

//添加通知对象
void CMMNotificationClient::Add(const photSwappingInterface& Interface)
{
	m_lock.lock();
	this->m_vObservers.push_back(Interface);
	m_lock.unlock();
}

//移除通知对象
void CMMNotificationClient::Remove(const photSwappingInterface& Interface)
{
	this->m_lock.lock();
	const auto& pos = std::find(this->m_vObservers.begin(), this->m_vObservers.end(), Interface);
	if (pos != this->m_vObservers.cend())
		this->m_vObservers.erase(pos);
	this->m_lock.unlock();
}


//解析设备名
tstring  CMMNotificationClient::AnalysisDeviceName(LPCWSTR pwstrDeviceId)
{
	HRESULT hr = S_OK;
	IMMDevice *pDevice = nullptr;
	IPropertyStore *pProps = nullptr;
	PROPVARIANT varString;
	CoInitialize(nullptr);
	PropVariantInit(&varString);

	if (_pEnumerator == nullptr)
	{
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
			NULL, CLSCTX_INPROC_SERVER,
			__uuidof(IMMDeviceEnumerator),
			(void**)&_pEnumerator);
	}
	if (hr == S_OK)
	{
		hr = _pEnumerator->GetDevice(pwstrDeviceId, &pDevice);
	}
	if (hr == S_OK)
	{
		hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
	}
	if (hr == S_OK)
	{
		hr = pProps->GetValue(PKEY_Device_FriendlyName, &varString);
	}
	tstring deviceName;
	if (hr == S_OK)
	{
		tstring str{ varString.pwszVal == NULL ? _T("") : varString.pwszVal };
		deviceName = std::move( str );
	}
	PropVariantClear(&varString);

	if (pProps)
	{
		pProps->Release();
		pProps = nullptr;
	}
	if (pDevice)
	{
		pDevice->Release();
		pDevice = nullptr;
	}
	return deviceName;
}

