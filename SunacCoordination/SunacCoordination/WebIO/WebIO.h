#pragma once
#include "../Sunac_def.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ConfigDictionary.h"
#include "WindowLocalData.h"
#include "WindowWebData.h"
#include "AirConditionLocalData.h"
#include "KitchenBathroomLocalData.h"


class WebIO
{
	WebIO();
public:
	~WebIO();

	static WebIO *GetInstance()
	{
		static WebIO instance;
		return &instance;
	}

	static const CConfigDictionary* GetConfigDict() { return &(GetInstance()->m_configDic); }
	//static const CWindowLocalData* GetWindowLocalData() { return &(GetInstance()->m_windowLocalData); }
	static const CAirConditionLocalData* GetAirConditionLocalData() { return &(GetInstance()->m_airConLocalData); }

public:
	////-----------------ԭ����������--------------//
	//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//openType, //��������
	//openNum,//����������
	//gongNengQu,//������
	//tongFengLiang//ͨ����
	//��ȡ�źʹ�
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows(); //��ȡ���д���
	vector<AttrWindow> GetAllDoors();  //��ȡ������

	//////////////////////////////////////////////////////////////////////////

	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen> GetKitchens (EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao);
	
	static std::vector<AttrKitchen> GetAllKitchens();
	//��ȡ������
	static std::vector<AttrBathroom> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	static std::vector<AttrBathroom> GetAllBathrooms();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrAirCon *> GetAirCons(double piShu,
												CString weiZhi,//����ˮ��λ��
												bool hasYuShuiGuan,
												CString yuShuiGuanWeizhi);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrAirCon *> GetAllAirCons();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrRailing *> GetRailings(eRailingType type);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrRailing *> GetAllRailings();

	//------����---------//
	//��web����ԭ���ļ��� ok����0 ���򷵻�-1
	static bool DownLoadFile(const int fileId, const CString filePathName);

protected:
	CConfigDictionary m_configDic;

	CWindowLocalData m_windowLocalData;
	CAirConditionLocalData m_airConLocalData;
	CKitchenBathroomLocalData m_kitchenBathroomLocalData;

	CWindowWebData m_windowWebData;
}; 

#define WEBINST (WebIO::GetInstance())

