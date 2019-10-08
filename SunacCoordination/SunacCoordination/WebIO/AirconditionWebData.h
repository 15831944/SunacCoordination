#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
public:

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;


	std::vector<AttrAirCon> GetAirCons(double piShu,
							CString weiZhi,//����ˮ��λ��
							bool hasYuShuiGuan,
							CString yuShuiGuanWeizhi);

	std::vector<AttrAirCon> GetAllAirCons();

	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//��XML�����յ���Ϣ

protected:

};

