#pragma once
#include "../Sunac_def.h"
#include "../Object/AttrAirCon.h"
#include "../Object/AttrDoor.h"
#include "../Object/AttrKitchen.h"
#include "../Object/AttrRailing.h"
#include "../Object/AttrToilet.h"
#include "../Object/AttrWindow.h"
#include "ConfigDictionary.h"


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

	static const CConfigDictionary* GetConfigDict() {	return &(GetInstance()->m_configDic);	}

public:
	//-----------------ԭ����������--------------//
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrWindow *>  GetWindows
	(
		double width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
		double height,
		CString openType, //��������
		int openNum,//����������
		CString gongNengQu,//������
		double tongFengLiang//ͨ����
	);

	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrWindow *>  GetAllWindows();

	//�Ŵ���ȡ�ߴ�ֵ�� yuanXingBianHaoԭ�ͱ�ţ� dimStr�ַ�����(��"H2")��fileName�ļ���,����״̬�����ļ�������ߴ����Ͷ�ȡ�ߴ�ȡֵ
	SRCDimData Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName);



	//////////////////////////////////////////////////////////////////////////
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrDoor *> GetDoors
	(
		double minWid, //���ֵ��ע��߶�ֵ����Ϊ�������� 
		CString doorType //������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrDoor *> GetAllDoors();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetKitchens
	(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ �Ŵ��Կ�/�Ŵ���ֱ��
		CString type,//�������� I��
		bool hasPaiQiDao//�Ƿ���������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrKitchen *> GetAllKitchens();
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrToilet *> GetToilets
	(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
		CString type,//����������
		bool hasPaiQiDao//�Ƿ���������
	);
	//ע���ⲿ��Ҫ�ͷŷ��ص��ڴ�
	static std::vector<AttrToilet *> GetAllToilets();
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

}; 

#define WEBINST (WebIO::GetInstance())

