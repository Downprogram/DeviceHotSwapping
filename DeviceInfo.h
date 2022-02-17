/*********************************
描述:用于设备热插拔处理，同时对录音过程中，查找对应设备并进行相应的操作;对应更多设置中麦克风选项
作者:zhongzhipeng
***********************************/
#pragma once
#include "stdafx.h"
typedef struct deviceInfo {
	
	tstring deviceName;//设备名
	tstring deviceGuid;//设备GUID
	size_t	index;	   //对应索引
	bool	select;	   //是否被选中，后面热插拔通知时，依据这个判断是不是对应的设备
}DeviceInfo;
