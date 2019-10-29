#pragma once
#include <vector>

using namespace std;


class CGlassData
{
public:
	CGlassData();
	~CGlassData();

	CString m_sArea;	//����
	CString m_sSubArea;//��������

	double m_heatInsulationCoefficient; //����ϵ��
	double m_shadingCoefficient; //����ϵ��

	CString m_sSerailName;  //����ϵ������

	int m_glassCount;		//��������

	vector<CString> m_alSerials; //ƥ����Ͳ�
};

class CGlassSeries
{
	CGlassSeries();
public:
	static CGlassSeries* Instance();
	virtual ~CGlassSeries();

	vector<CString> GetAllGlassSerials();

	//ͨ���Ͳ�ϵ�л�ȡ����ѡ��
	vector<CString> GetGlassSerialsByAlSerial(CString p_sAlSerial);

	//ͨ������ϵ����ȡ����ѡ��
	vector<CString> GetGlassSerialsByHeatInsulationCoeff(double p_heatCoeff);

};