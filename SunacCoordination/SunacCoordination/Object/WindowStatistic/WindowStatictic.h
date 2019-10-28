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

	void Export(CString p_sReportFile);
	bool ExportWindowReport(Excel::CExcelUtil& xls);
	bool GenerateReport(Excel::CExcelUtil& xls);

#if 0
	void ExportSeparate(CString p_sReportFile); //�������ͳ�Ʊ�ֿ����
	bool ExportWindowReport(CString p_sReportFile);
	bool GenerateReport(CString p_sReportFile);

#endif

protected:
	vector<CWindowUsage> m_windows;
};

