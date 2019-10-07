#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"


class CAirConditionWebData
{
public:

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;


	std::vector<AttrAirCon> GetAirCons(double piShu,
							CString weiZhi,//����ˮ��λ��
							bool hasYuShuiGuan,
							CString yuShuiGuanWeizhi);

	std::vector<AttrAirCon> GetAllAirCons();

protected:

};

