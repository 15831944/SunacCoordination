#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "WindowMaterialUsage.h"

class CWindowAttCount
{
public:
	AttrWindow winAtt;
	int nCount;

	CWindowAttCount()
	{
		nCount = 1;
	}
};


//�Ŵ�ͳ�Ʊ�
class CWindowStatictic
{
public:
	CWindowStatictic();
	virtual ~CWindowStatictic();
	
	void Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile); //ͳ�Ʋ�����ͳ�Ʊ����ļ�

protected:
	vector<CWindowAttCount> WindowClassify(const vector<AttrWindow>& p_winAtts);

	void InitWindowMatrialUsage(const vector<CWindowAttCount>& p_winows);

	bool GenerateReport(CString p_sReportFile);

protected:
	vector<CWindowMaterialUsage*> m_allWindowMatUsage; //�������͵Ĵ��͵���������
};

