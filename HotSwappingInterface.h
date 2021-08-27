/*********************************
描述:通知对象，热插拔产生的通知将被转发到该对象
	 不能对UI进行处理，如有对UI处理的需要需要自己发送消息
作者:zhongzhipeng
***********************************/

#ifndef __CMMNOTIFICATION_H__
#define __CMMNOTIFICATION_H__
#include <memory>
#include <string>
#include "DeviceInfo.h"
class HotSwappingInterface {
public:
	virtual void deviceInsert(const DeviceInfo& deviceInfo) {};//设备插入
	virtual void deviceUnplug(const DeviceInfo& deviceInfo) {};//设备拔出
	virtual void deviceDisable(const DeviceInfo& deviceInfo) {};//设备禁用
	virtual void decvicePreparation(const DeviceInfo& deviceInfo) {};//设备未准备
	virtual void deviceStateAll(const DeviceInfo& deviceInfo, DWORD  dwNewState) {};//设备全部状态，交由自己做处理
	virtual void deviceTypeChange(const DeviceInfo& deviceInfo, const stringT& type) {};//设备切换会触发，系统检查设备功能会触发
	virtual	~HotSwappingInterface() {};
};

using photSwappingInterface = std::shared_ptr<HotSwappingInterface>;

#endif
