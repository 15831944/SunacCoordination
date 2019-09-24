#pragma once
#include <map>
using namespace std;



class CConfigDictionary
{
public:
//	static CConfigDictionary *GetInstance();

	CConfigDictionary();
	~CConfigDictionary();

protected:
	bool InitFromWeb();
	bool GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut);

protected:
	map<CString, vCString> m_configDict;

	//vCString m_quyus;//����
	//vCString  m_gongNengQus;//������
	//vCString  m_windowDoorPoss;//�Ŵ�λ�ù�ϵ--����������
	//vCString m_windowOpenTypes;//�ⴰ��������
	//vCString m_windowOpenAmount;//����������
	//vCString m_windowWallDis;//��������ǽ����
	//vCString m_rate;//��ֵ�����ڼ���ͨ����
	//vCString m_doorTypes;//������
	//vCString m_kitchenTypes;//��������
	//vCString m_kitchenShuiPenTypes;//����ˮ������
	//vCString m_kitchenBingXiangTypes;//������������
	//vCString m_kitchenZaoTaiWidths;//������̨���
	//vCString m_toiletTypes;//����������
	//vCString m_toiletTaiPenWidths;//����������
	//vCString m_toiletMaTongTypes;//����������
	//vCString m_toiletGuanXiWidths;//��������ϴ�����---�Ƿ�֧���ֶ����룿��
	//vCString m_airPiShus;//�յ����е�ƥ��
	//vCString m_airLengNingShuiGuanPos;//�յ�����ˮ��λ��

public:
	//---------------ͨ�ò��ֵ�-----------------------//
	vCString GetQuyus() const ;//����
	vCString GetGongNengQus()const;//������
	vCString GetWindowDoorPoss()const;//�Ŵ�λ�ù�ϵ

	vCString Window_GetOpenTypes()const;//��������
	vCString Window_GetOpenAmount()const;//����������
	vCString Window_GetRate()const;//��ֵ�����ڼ���ͨ����
	vCString Window_GetWallDis()const;//��������ǽ����

	vCString Door_GetTypes()const;//������

	vCString Kitchen_GetTypes()const;//��������
	vCString Kitchen_GetShuiPenTypes()const;//ˮ������
	vCString Kitchen_GetBingXiangTypes()const;//��������
	vCString Kitchen_GetZaoTaiWidths()const;//��̨���

	vCString Toilet_GetTypes()const;//����������
	vCString Toilet_GetTaiPenWidths()const;//����������
	vCString Toilet_GetMaTongTypes()const;//����������
	vCString Toilet_GetGuanXiWidths()const;//��ϴ�����---�Ƿ�֧���ֶ����룿��

	vCString Air_GetPiShus()const;//���е�ƥ��
	vCString Air_GetLengNingShuiGuanPos()const;//����ˮ��λ��
	vCString Air_GetYuShuiGuanPos();
};