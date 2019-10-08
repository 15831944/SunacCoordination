#pragma once
#include "../Sunac_def.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "../Object/WindowDoor/AttrDoor.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ConfigDictionary.h"
#include "WindowLocalData.h"
#include "AirConditionLocalData.h"
#include "../Object/PrototypeInfo.h"


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
	static const CWindowLocalData* GetWindowLocalData() { return &(GetInstance()->m_windowLocalData); }
	static const CAirConditionLocalData* GetAirConditionLocalData() { return &(GetInstance()->m_airConLocalData); }

public:
	////-----------------ԭ����������--------------//
	////ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	//static std::vector<AttrWindow *>  GetWindows
	//(
	//	double width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
	//	double height,
	//	CString openType, //��������
	//	int openNum,//����������
	//	CString gongNengQu,//������
	//	double tongFengLiang//ͨ����
	//);

	////ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	//static std::vector<AttrWindow *>  GetAllWindows();

	////�Ŵ���ȡ�ߴ�ֵ�� yuanXingBianHaoԭ�ͱ�ţ� dimStr�ַ�����(��"H2")��fileName�ļ���,����״̬�����ļ�������ߴ����Ͷ�ȡ�ߴ�ȡֵ
	//SRCDimData Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName);



	////////////////////////////////////////////////////////////////////////////
	////ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	//static std::vector<AttrDoor *> GetDoors
	//(
	//	double minWid, //���ֵ��ע��߶�ֵ����Ϊ�������� 
	//	CString doorType //������
	//);
	////ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	//static std::vector<AttrDoor *> GetAllDoors();


	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetKitchens (
								double kaiJian,//����
								double jinShen,//����
								CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ �Ŵ��Կ�/�Ŵ���ֱ��
								CString type,//�������� I��
								bool hasPaiQiDao,//�Ƿ���������
								bool bDynatic); //�Ƿ�̬
	

	//////////////////////////////////////////////////////////////////////////
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetAllKitchens();
	//��ȡ������
	static std::vector<CPrototypeInfo> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	static std::vector<CPrototypeInfo> GetAllBathrooms();
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
	//����ԭ�ͱ�ţ����ص��̶���ַ
	//ok����0 ���򷵻�-1
	static int DownLoadFile(CString id, CString filePathName);



protected:
	CConfigDictionary m_configDic;

	CWindowLocalData m_windowLocalData;
	CAirConditionLocalData m_airConLocalData;

}; 

#define WEBINST (WebIO::GetInstance())

