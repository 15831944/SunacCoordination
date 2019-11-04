#include "StdAfx.h"
#include "..\..\Tool\Excel\Excel.h"
#include "WindowStatictic.h"
#include "WindowMaterialUsage.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Str.h"
#include "..\..\Common\TYFormula.h"


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

	//3. �������
	Export(p_sReportFile);
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

void CWindowStatictic::Export(CString p_sReportFile)//�������ͳ�Ʊ�ֿ����
{
	CString reportTemplateXlsFile = TY_GetDataFilePath() + _T("�Ŵ��������ģ��.xlsx");

	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(reportTemplateXlsFile); //�򿪱��
	if (bSuc == false)
	{
		AfxMessageBox(_T("�޷��� �Ŵ��������ģ��.xlsx"));
		return ;
	}

	xls.SetVisible(false);

	//2. �Ը����Ŵ����ɱ���
	ExportWindowReport(xls);

	//3. ���ܸ����Ŵ��õ����ܱ�
	GenerateReport(xls);

	//����
	xls.SaveAs(p_sReportFile);
	xls.CloseExcel();
}

bool CWindowStatictic::ExportWindowReport(Excel::CExcelUtil& xls)
{
	bool bAllSuc = true;

	//�����Ŵ����������Ŵ�������
	for (UINT i = 1; i < m_windows.size(); i++)
	{
		xls.CopySheet(_T("������"), m_windows[i].winAtt.GetInstanceCode());
	}

	if (m_windows.size()>0)
	{
		xls.SetSheetName(2, m_windows[0].winAtt.GetInstanceCode());
	}

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		xls.SetActiveSheet(i+2); //sheet��Ŵ�1��ʼ���ҵ�һ�����ǻ��ܱ�
		bool bSuc = m_windows[i].m_pMaterialUsage->ExportReportToExcel(xls);
		if (bSuc == false)
			bAllSuc = false;
	}

	return bAllSuc;
}

#if 0
void CWindowStatictic::ExportSeparate(CString p_sReportFile)//�������ͳ�Ʊ�ֿ����
{
	//2. �Ը����Ŵ����ɱ���
	ExportWindowReport(p_sReportFile);


	//3. ���ܸ����Ŵ��õ����ܱ�
	GenerateReport(p_sReportFile);
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
		CString sOutFilePath = sPath + _T("_") + m_windows[i].winAtt.GetInstanceCode() + _T(".xlsx");
		bool bSuc = m_windows[i].m_pMaterialUsage->ExportReportToExcel(sOutFilePath);
		if (bSuc==false)
			bAllSuc = false;
	}

	return bAllSuc;
}

//2.���ɻ��ܱ�
bool CWindowStatictic::GenerateReport(CString p_sReportFile)
{
	CString reportTemplateXlsFile = TY_GetDataFilePath() + _T("�Ŵ��������ģ��.xlsx");

	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(reportTemplateXlsFile); //�򿪱��
	if (bSuc==false)
	{
		AfxMessageBox(_T("�޷��� �Ŵ��������ģ��.xlsx"));
		return false;
	}

	xls.SetVisible(false); 
	xls.SetActiveSheet(1); //�򿪻��ܱ�

	GenerateReport(xls);
	
	xls.SaveAs(p_sReportFile);
	xls.CloseExcel();

	return true;
}
#endif

bool CWindowStatictic::GenerateReport(Excel::CExcelUtil& xls)
{
	CString str;
	xls.SetActiveSheet(1); //�򿪻��ܱ�

	int nRow = 6; //�������ݿ�ʼ�к�Ϊ6
	for (UINT i=0; i<m_windows.size(); i++, nRow++)
	{
		//���(һ������)
		if (m_windows[i].winAtt.m_prototypeCode.Find(_T("Window")) >= 0)
		{
			xls.SetCellValue(nRow, 4, _T("���Ͻ�"));
		}
		if (m_windows[i].winAtt.m_prototypeCode.Find(_T("Door")) >= 0)
		{
			xls.SetCellValue(nRow, 4, _T("���Ͻ���"));
		}

		//����(��������)
		xls.SetCellValue(nRow, 5, WindowDoorTypeToCSting(m_windows[i].winAtt.GetWindowDoorType()));

		//�Ŵ����
		xls.SetCellValue(nRow, 6, m_windows[i].winAtt.GetInstanceCode());

		//�Ͳ�����
		str = m_windows[i].winAtt.m_material.sAluminumSerial;
		xls.SetCellValue(nRow, 9, str);
		xls.SetCellValue(nRow, 10, _T("������"));
		if (str.Find(_T("50")) >= 0 || str.Find(_T("90")) >= 0 || str.Find(_T("120")) >= 0)
		{
			xls.SetCellValue(nRow, 10, _T("����"));
		}

		//���ڳߴ�
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetW()));
		xls.SetCellValue(nRow, 12, str);
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetH()));
		xls.SetCellValue(nRow, 13, str);

		//�ۼ��ߴ�
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetA()));
		xls.SetCellValue(nRow, 14, str);
		xls.SetCellValue(nRow, 15, str);

		//���ߴ�
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetW() - m_windows[i].winAtt.GetA() * 2));
		xls.SetCellValue(nRow, 16, str);
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetH() - m_windows[i].winAtt.GetA() * 2));
		xls.SetCellValue(nRow, 17, str);
	
		//����������
		str.Format(_T("%d"), m_windows[i].nCount);
		xls.SetCellValue(nRow, 18, str);

		//���̶������
		double holeArea = m_windows[i].m_pMaterialUsage->GetHoleArea();
		str.Format(_T("%.2f"), holeArea);
		xls.SetCellValue(nRow, 22, str);

		//���������
		str.Format(_T("%.2f"), double(m_windows[i].nCount* holeArea));
		xls.SetCellValue(nRow, 23, str);

		//����������
		double frameArea = m_windows[i].m_pMaterialUsage->GetWindowFrameArea();
		str.Format(_T("%.2f"), frameArea);
		xls.SetCellValue(nRow, 24, str);

		//��������
		str.Format(_T("%.2f"), double(m_windows[i].nCount* frameArea));
		xls.SetCellValue(nRow, 25, str);

		//�Ͳ�(kg/�O)
		double alUsageAmount = m_windows[i].m_pMaterialUsage->GetAluminumeUsageAmount();
		str.Format(_T("%.2f"), alUsageAmount/holeArea);
		xls.SetCellValue(nRow, 26, str);

		//����(�O/�O)
		double glassUsageAmount = m_windows[i].m_pMaterialUsage->GetGlassUsageAmount();
		str.Format(_T("%.2f"), glassUsageAmount/holeArea);
		xls.SetCellValue(nRow, 27, str);

		//����(��/��)
		double hardwareUsageAmount = m_windows[i].m_pMaterialUsage->GetHardwareUsageAmount();
		str.Format(_T("%.2f"), hardwareUsageAmount/holeArea);
		xls.SetCellValue(nRow, 29, str);
	}


	return true;
}