#pragma once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/WindowDoor/AttrDoor.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"

class CWindowWebData
{
public:
	CWindowWebData();
	~CWindowWebData();

	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//ͨ���ļ�����m_windows�л�ȡ����

	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetAllWindows()const;

	std::vector<AttrDoor >  GetAllDoors()const;
	std::vector<AttrDoor >  GetDoors(double p_width, double p_heigh, CString doorType)const;

	std::vector<AttrBathroom> GetBathrooms(double width, double height, CString BathroomDoorWindowPosition, CString ToiletType, int Airvent)const;
	std::vector<AttrBathroom> GetAllBathrooms()const;

	std::vector<AttrKitchen> GetAllKitchens()const;
	std::vector<AttrRailing> GetRailings(CString RailingType)const;
	std::vector<AttrAirCon> GetAirConditioners(CString AirconditionerPower, CString AirconditionerPipePosition, int AirconditionerIsRainpipe, CString RainpipePosition)const;

	std::vector<AttrWindow > ParseWindowsFromXML(CMarkup xml)const;//��XML����������Ϣ
	std::vector<AttrDoor > ParseDoorsFromXML(CMarkup xml)const;//��XML��������Ϣ
	std::vector<AttrBathroom > ParseBathroomsFromXML(CMarkup xml)const;//��XML������������Ϣ
	std::vector<AttrKitchen > ParseKitchensFromXML(CMarkup xml)const;//��XML����������Ϣ
	std::vector<AttrRailing > ParseRailingsFromXML(CMarkup xml)const;//��XML����������Ϣ
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//��XML�����յ���Ϣ

protected:

};

