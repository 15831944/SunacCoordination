#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
	CAirConditionWebData(){}
public:
	static CAirConditionWebData* Instance();

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;

	//weiZhi����ˮ��λ��
	std::vector<AttrAirCon> GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi);
	std::vector<AttrAirCon> GetAllAirCons();

protected:
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//��XML�����յ���Ϣ
};

