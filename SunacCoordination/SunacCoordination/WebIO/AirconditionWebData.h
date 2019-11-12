#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
	CAirConditionWebData();
public:
	static CAirConditionWebData* Instance();

	~CAirConditionWebData();

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;

	//weiZhi����ˮ��λ��
	/*std::vector<AttrAirCon> GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi);*/
	std::vector<AttrAirCon> GetAllAirCons();
	std::vector<AttrAirCon >  GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;

protected:
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//��XML�����յ���Ϣ
	vector<AttrAirCon> m_allAirconditions; //���пյ�
};

