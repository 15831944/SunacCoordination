#include "StdAfx.h"
#include "WindowStatictic.h"
#include "..\..\Tool\Excel\Excel.h"


CWindowStatictic::CWindowStatictic()
{
}

CWindowStatictic::~CWindowStatictic()
{
	for (UINT i = 0; i < m_allWindowMatUsage.size(); i++)
	{
		delete m_allWindowMatUsage[i];
	}

	m_allWindowMatUsage.clear();
}

void CWindowStatictic::Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile) //ͳ�Ʋ�����ͳ�Ʊ����ļ�
{
	////1. ���ȶ��Ŵ����з��������
	//vector<CWindowAttCount> windwosAttCounts = WindowClassify(const vector<AttrWindow>& p_winAtts);


	////2. �Ը����Ŵ����ɱ���
	//InitWindowMatrialUsage(windwosAttCounts);



	////3. ���ܸ����Ŵ��õ����ܱ�
	//GenerateReport(p_sReportFile);



}