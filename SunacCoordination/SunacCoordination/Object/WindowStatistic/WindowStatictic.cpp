#include "StdAfx.h"
#include "..\..\Tool\Excel\Excel.h"
#include "WindowStatictic.h"
#include "WindowMaterialUsage.h"


CWindowUsage::CWindowUsage()
{
	nCount = 1;
	m_pMaterialUsage = NULL;
}

CWindowUsage::~CWindowUsage()
{
	if (m_pMaterialUsage!=NULL)
	{
		delete m_pMaterialUsage;
		m_pMaterialUsage = NULL;
	}
}

void CWindowUsage::InitWindMaterialUsage()
{
	if (m_pMaterialUsage != NULL)
	{
		delete m_pMaterialUsage;
		m_pMaterialUsage = NULL;
	}

	E_WindowDoorType winDoorType = winAtt.GetWindowDoorType();
	switch (winDoorType)
	{
	case E_WindowDoor_NC:
		m_pMaterialUsage = new CWindowMaterialUsageNC(winAtt, nCount);
		break;
	case E_WindowDoor_WC:
		m_pMaterialUsage = new CWindowMaterialUsageWC(winAtt, nCount);
		break;
	case E_WindowDoor_TC:
		m_pMaterialUsage = new CWindowMaterialUsageTC(winAtt, nCount);
		break;
	case E_WindowDoor_WM:
		m_pMaterialUsage = new CWindowMaterialUsageWM(winAtt, nCount);
		break;
	case E_WindowDoor_TLM:
		m_pMaterialUsage = new CWindowMaterialUsageTLM(winAtt, nCount);
		break;
	case E_WindowDoor_TSTLM:
		m_pMaterialUsage = new CWindowMaterialUsageTSTLM(winAtt, nCount);
		break;
	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
CWindowStatictic::CWindowStatictic()
{
}

CWindowStatictic::~CWindowStatictic()
{
}


void CWindowStatictic::Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile) //ͳ�Ʋ�����ͳ�Ʊ����ļ�
{
	//1. ���ȶ��Ŵ����з��������
	WindowClassify(p_winAtts);	

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		m_windows[i].InitWindMaterialUsage();
	}

	//2. �Ը����Ŵ����ɱ���
	ExportWindowReport(p_sReportFile);


	//3. ���ܸ����Ŵ��õ����ܱ�
	GenerateReport(p_sReportFile);
}

void CWindowStatictic::WindowClassify(const vector<AttrWindow>& p_winAtts)
{
	m_windows.clear();
	
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //ԭ�ͱ��
		bool bFind = false;
		for (UINT n = 0; n < m_windows.size(); n++)
		{
			if (m_windows[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode)==0)
			{
				bFind = true;
				m_windows[n].nCount++;
				break;
			}
		}

		if (bFind==false)
		{
			CWindowUsage winNew;
			winNew.winAtt = p_winAtts[i];
			winNew.nCount = 1;
			m_windows.push_back(winNew);
		}
	}
}

//ÿ������ŵ���һ������
bool CWindowStatictic::ExportWindowReport(CString p_sReportFile)
{
	int nPos = p_sReportFile.ReverseFind(_T('.'));
	if (nPos<0)
		return false;

	CString sPath = p_sReportFile.Left(nPos);
	
	bool bAllSuc = true;
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		CString sOutFilePath = sPath + m_windows[i].winAtt.GetInstanceCode() + _T(".xlsx");
		bool bSuc = m_windows[i].m_pMaterialUsage->ExportReportToExcel(sOutFilePath);
		if (bSuc==false)
			bAllSuc = false;
	}

	return bAllSuc;
}

//2.���ɻ��ܱ�
bool CWindowStatictic::GenerateReport(CString p_sReportFile)
{
	//TODO



	return true;
}