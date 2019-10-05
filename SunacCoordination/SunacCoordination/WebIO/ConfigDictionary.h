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
	bool GetConfig(wstring Term);
	bool GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut);

protected:
	map<CString, vCString> m_configDict;


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

	vCString Bathroom_GetTypes()const;//����������
	vCString Bathroom_GetTaiPenWidths()const;//����������
	vCString Bathroom_GetMaTongTypes()const;//����������
	vCString Bathroom_GetGuanXiWidths()const;//��ϴ�����---�Ƿ�֧���ֶ����룿��

	vCString Air_GetPiShus()const;//���е�ƥ��
	vCString Air_GetLengNingShuiGuanPos()const;//����ˮ��λ��
	vCString Air_GetYuShuiGuanPos() const;
	vCString Railing_GetTypes();

};