#pragma once

#include <vector>
#include <atlconv.h>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_Convert.h"
//#include "..\WebIO\WebIO.h"


class CWindowWebData
{
	CWindowWebData();
public:
	static CWindowWebData* Instance();
	~CWindowWebData();

	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //ͨ��ԭ�ͱ�Ŵ�m_windows�л�ȡ����
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//ͨ���ļ�����m_windows�л�ȡ����

	//ע��߶�ֵ����Ϊ�������� 
	//width���ֵ��openType��������, openNum����������  gongNengQu������, tongFengLiangͨ����
	std::vector<AttrWindow >  GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetAllWindows()const;

	std::vector<AttrWindow >  GetAllDoors()const;
	std::vector<AttrWindow >  GetDoors(double p_width, double p_heigh, CString doorType, int openNum, CString gongNengQu)const;

protected:
	std::vector<AttrWindow > ParseWindowsFromXML(CMarkup xml)const;//��XML����������Ϣ
	std::vector<AttrWindow > ParseDoorsFromXML(CMarkup xml)const;//��XML��������Ϣ
};

