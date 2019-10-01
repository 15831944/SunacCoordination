#pragma  once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/AirCondition/AttrAirCon.h"

using namespace std;

class CAirConditionLocalData
{
public:
	CAirConditionLocalData();
	~CAirConditionLocalData();

	bool GetAirConById(CString p_sId,AttrAirCon& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_aircon�л�ȡ�յ�
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value)const;//ͨ���ļ�����m_aircon�л�ȡ�յ�

	std::vector<AttrAirCon >  GetAirCon(CString p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;

	vector<AttrAirCon> GetAllAirCon()const; //��ȡ���пյ�

protected:
	void LoadDataFromExcel(CString p_file); //�ӱ���а����ݴ���m_windows��

protected:
	vector<AttrAirCon> m_aircon; //���пյ�
};

