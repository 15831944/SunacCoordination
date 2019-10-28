#pragma once

#include <vector>
#include "..\Tool\MarkupXml\Markup.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"


class CKitchenBathroomWebData
{
public:
	CKitchenBathroomWebData(){ }
	~CKitchenBathroomWebData(){}

	bool GetKitchenById(CString p_sId, AttrKitchen& value)const;
	bool GetAttrBathroomById(CString p_sId, AttrBathroom& value)const;


	std::vector<AttrKitchen> GetKitchens( double kaiJian,//开间
												double jinShen,//进深
												CString weiZhiGuanXi,//门窗位置关系 门窗对开/门窗垂直开
												CString type,//厨房类型 I型
												bool hasPaiQiDao//是否含有排气道
												/*bool bDynatic*/)const; //是否动态

	std::vector<AttrKitchen> GetAllKitchens()const;


	std::vector<AttrBathroom> GetBathrooms(double width,//宽度，X方向
													double height,//高度，Y方向
													CString weiZhiGuanXi,//门窗位置关系
													CString type,//卫生间类型
													bool hasPaiQiDao)const;//是否含有排气道
		

	std::vector<AttrBathroom> GetAllBathrooms()const;

	std::vector<AttrKitchen > ParseKitchensFromXML(CMarkup xml)const;//从XML解析厨房信息
	std::vector<AttrBathroom > ParseBathroomsFromXML(CMarkup xml)const;//从XML解析卫生间信息

protected:

};

