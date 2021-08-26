/*********************************
描述:通知对象，热插拔产生的通知将被转发到该对象
	 不能对UI进行处理，如有对UI处理的需要需要自己发送消息
作者:zhongzhipeng
***********************************/

#ifndef __CMMNOTIFICATION_H__
#define __CMMNOTIFICATION_H__
#include <memory>
#include <string>
class HotSwappingInterface {
public:
	virtual void deviceInsert(const stringT& deviceName) {};//设备插入
	virtual void deviceUnplug(const stringT& deviceName) {};//设备拔出
	virtual void deviceDisable(const stringT& deviceName) {};//设备禁用
	virtual void decvicePreparation(const stringT& deviceName) {};//设备未准备
	virtual void deviceStateAll(const stringT& deviceName, DWORD  dwNewState) {};//设备全部状态，交由自己做处理
	virtual void deviceTypeChange(const stringT& deviceName, const stringT& type) {};//设备类型切换
	virtual	~HotSwappingInterface() {};
};

using photSwappingInterface = std::shared_ptr<HotSwappingInterface>;

#endif
