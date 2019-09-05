#pragma once
#include "../Sunac_def.h"
class WebIO
{
public:
	static WebIO *GetInstance()
	{
		static WebIO instance;
		return &instance;
	}
	WebIO();
	~WebIO();

public:
	//-----------------ԭ����������--------------//
	static int  GetWindows
		(
		double width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
		CString openType, //��������
		int openNum,//����������
		CString gongNengQu,//������
		double tongFengLiang//ͨ����
		);

	//�Ŵ���ȡ�ߴ�ֵ
	eWindowDimType Window_GetDim(CString yuanXingBianHao, CString dimStr);//��ȡ�ߴ�ȡֵ

	static int GetDoors
		(
		double minWid, //���ֵ��ע��߶�ֵ����Ϊ�������� 
		CString doorType //������
		);

	static int GetKitchens
		(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
		CString type,//��������
		bool hasPaiQiDao//�Ƿ���������
		);

	static int GetToilets
		(
		double kaiJian,//����
		double jinShen,//����
		CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
		CString type,//����������
		bool hasPaiQiDao//�Ƿ���������
		);
	
	static int GetAirCon
	(
		double piShu,
		CString weiZhi,//����ˮ��λ��
		bool hasYuShuiGuan
	);

	static int GetRailings();

public:
	vCString m_quyus;//����
	vCString  m_gongNengQus;//������
	vCString  m_windowDoorPoss;//�Ŵ�λ�ù�ϵ--����������
	vCString m_windowOpenTypes;//�ⴰ��������
	vCString m_doorTypes;//������
	vCString m_kitchenTypes;//��������
	vCString m_kitchenShuiPenTypes;//����ˮ������
	vCString m_kitchenBingXiangTypes;//������������
	vCString m_kitchenZaoTaiWidths;//������̨���
	vCString m_toiletTypes;//����������
	vCString m_toiletTaiPenWidths;//����������
	vCString m_toiletMaTongTypes;//����������
	vCString m_toiletGuanXiWidths;//��������ϴ�����---�Ƿ�֧���ֶ����룿��
	vCString m_airPiShus;//�յ����е�ƥ��
	vCString m_airLengNingShuiGuanPos;//�յ�����ˮ��λ��

protected:
	//---------------ͨ�ò��ֵ�-----------------------//
	vCString GetQuyus();//����
	vCString GetGongNengQus();//������
	vCString GetWindowDoorPoss();//�Ŵ�λ�ù�ϵ

	vCString Window_GetOpenTypes();//��������

	vCString Door_GetTypes();//������

	vCString Kitchen_GetTypes();//��������
	vCString Kitchen_GetShuiPenTypes();//ˮ������
	vCString Kitchen_GetBingXiangTypes();//��������
	vCString Kitchen_GetZaoTaiWidths();//��̨���

	vCString Toilet_GetTypes();//����������
	vCString Toilet_GetTaiPenWidths();//����������
	vCString Toilet_GetMaTongTypes();//����������
	vCString Toilet_GetGuanXiWidths();//��ϴ�����---�Ƿ�֧���ֶ����룿��

	vCString Air_GetPiShus();//���е�ƥ��
	vCString Air_GetLengNingShuiGuanPos();//����ˮ��λ��
}; 
