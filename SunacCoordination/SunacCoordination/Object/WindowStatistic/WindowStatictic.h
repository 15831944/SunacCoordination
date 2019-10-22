#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "WindowMaterialUsage.h"

//�������͵����ԡ�����������
class CWindowUsage
{
public:
	CWindowUsage();
	virtual ~CWindowUsage();

	void InitWindMaterialUsage();

public:
	AttrWindow winAtt;
	int nCount;

	CWindowMaterialUsage* m_pMaterialUsage;
};


//�Ŵ�ͳ�Ʊ�
class CWindowStatictic
{
public:
	CWindowStatictic();
	virtual ~CWindowStatictic();
	
	void Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile); //ͳ�Ʋ�����ͳ�Ʊ����ļ�

protected:
	void WindowClassify(const vector<AttrWindow>& p_winAtts);

	void InitWindowMatrialUsage(vector<CWindowUsage>& p_winows);

	bool ExportWindowReport(CString p_sReportFile);
	bool GenerateReport(CString p_sReportFile);

protected:
	vector<CWindowUsage> m_windows;
};

