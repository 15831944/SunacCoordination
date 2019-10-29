#pragma once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CAirConditionLocalDataFromDB
{
public:
	CAirConditionLocalDataFromDB();
	~CAirConditionLocalDataFromDB();
	static CAirConditionLocalDataFromDB* Instance();

	bool GetAirConById(CString p_sId,AttrAirCon& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_allAirconditions�л�ȡ�յ�
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value)const;//ͨ���ļ�����m_allAirconditions�л�ȡ�յ�

	std::vector<AttrAirCon >  GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;
	vector<AttrAirCon> GetAllAirCons()const; //��ȡ���пյ�
	static int OutputAirConData(void *NotUsed, int nCol, char **value, char **ColName);

protected:
	void LoadDataFromDataBase(); //�ӱ���а����ݴ���m_allAirconditions��

protected:
	vector<AttrAirCon> m_allAirconditions; //���пյ�
	sqlite3* m_pDB;
};

