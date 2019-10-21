#include "StdAfx.h"
#include "WindowStatictic.h"
#include "..\..\Tool\Excel\Excel.h"


CWindowStatictic::CWindowStatictic()
{
}

CWindowStatictic::~CWindowStatictic()
{
	ClearAllWindowMaterialUsage();
}

void CWindowStatictic::ClearAllWindowMaterialUsage()
{
	for (UINT i = 0; i < m_allWindowMatUsage.size(); i++)
	{
		delete m_allWindowMatUsage[i];
	}

	m_allWindowMatUsage.clear();
}

void CWindowStatictic::Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile) //ͳ�Ʋ�����ͳ�Ʊ����ļ�
{
	//1. ���ȶ��Ŵ����з��������
	vector<CWindowAttCount> windwosAttCounts = WindowClassify(p_winAtts);

	//2. �Ը����Ŵ����ɱ���
	InitWindowMatrialUsage(windwosAttCounts);


	//3. ���ܸ����Ŵ��õ����ܱ�
	GenerateReport(p_sReportFile);
}

vector<CWindowAttCount> CWindowStatictic::WindowClassify(const vector<AttrWindow>& p_winAtts)
{
	vector<CWindowAttCount> windows;
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //ԭ�ͱ��
		bool bFind = false;
		for (UINT n = 0; n < windows.size(); n++)
		{
			if (windows[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode)==0)
			{
				bFind = true;
				windows[n].nCount++;
				break;
			}
		}

		if (bFind==false)
		{
			CWindowAttCount winNew;
			winNew.winAtt = p_winAtts[i];
			winNew.nCount = 1;
		}
	}

	return windows;
}

void CWindowStatictic::InitWindowMatrialUsage(const vector<CWindowAttCount>& p_winows)
{
	ClearAllWindowMaterialUsage();

	for (UINT i = 0; i < p_winows.size(); i++)
	{
		//switch (p_winows[i].winAtt.)
		//{
		//default:
		//	break;
		//}
	}

	//TODO

}

bool CWindowStatictic::GenerateReport(CString p_sReportFile)
{
	//TODO

	return true;
}