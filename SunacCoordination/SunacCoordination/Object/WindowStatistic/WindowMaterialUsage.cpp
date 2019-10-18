#include "StdAfx.h"
#include "WindowMaterialUsage.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Str.h"
#include "..\..\Common\TYFormula.h"

CWindowMaterialUsage::CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount)
:m_winAtt(p_winAtt)
, m_nCount(p_nCount)
{
}

CWindowMaterialUsage::~CWindowMaterialUsage()
{
}

void CWindowMaterialUsageNC::CalcMateriaUsage()
{

}


double CWindowMaterialUsage::GetVauleByFomula(CString p_sFomula)
{
	//p_sFomula.MakeUpper();
	CFormulaParser parser(p_sFomula);
	double ret;
	E_ParserStatus es;
	while ((es = parser.TryParse(ret)) == E_PS_UnknownVars)
	{
		vCString vars = parser.GetUnknownVars();
		for (UINT i = 0; i < vars.size(); i++)
		{
			CString sParaName = vars[i];
			double valueOut = 0;
			//W.. H..a  R  �Ĳ�����m_winAtt ȡ
			if (sParaName.Find(_T("W")) >= 0 || sParaName.Find(_T("H")) >= 0 || sParaName.Find(_T("a")) >= 0 || sParaName.Find(_T("R")) >= 0)
			{
				valueOut = m_winAtt.GetValue(sParaName);
			}
			else
			{
				//�����Ĵӿۼ��ߴ����ȡ
				bool bSuc = CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(m_winAtt.m_openType, m_winAtt.m_material.sAluminumSerial, sParaName, valueOut);
			}
			parser.SetVar(sParaName, m_winAtt.GetValue(sParaName, valueOut));
		}
	}
	if (es == E_PS_InvalidFormula)
	{
		CString errMsg;
		errMsg.Format(L"��ʽ%s��ʽ�����޷�����\n", p_sFomula);
		acutPrintf(errMsg);
		return 0;
	}
	else if (es == E_PS_PaserOverflow)
	{
		CString errMsg;
		errMsg.Format(L"��ʽ%s����ѭ���Ƶ����޷�����\n", p_sFomula);
		acutPrintf(errMsg);
		return 0;
	}
	else if (es == E_PS_Success)
	{
		return ret;
	}
	else
	{
		assert(false);
		return 0;
	}
}

bool CWindowMaterialUsageNC::ExportReportToExcel(CString p_sReportFile)
{
	Excel::CExcelUtil xls;
	xls.OpenExcel(p_sReportFile); //�򿪱��
	xls.SetVisible(true); 
	xls.SetActiveSheet(2); //�򿪵ڶ��ű�

	ExportReportToExcel(xls);

	return true;
}

bool CWindowMaterialUsageNC::ExportReportToExcel(Excel::CExcelUtil& p_excel)
{
	p_excel.SetActiveSheet(2); //�򿪵ڶ��ű�

	ExportWindowInfo(p_excel);
	ExprotAlInfo(p_excel);


	return true;
}

void CWindowMaterialUsage::ExportWindowInfo(Excel::CExcelUtil& p_excel)//���������Ϣ
{
	CString str;

	p_excel.SetCellValue(2, 2, m_winAtt.m_prototypeCode);	
	
	//�߶�
	str.Format(_T("%d"),(int)(m_winAtt.GetH()));
	p_excel.SetCellValue(4, 8, str);

	//��
	str.Format(_T("%d"),(int)(m_winAtt.GetW()));
	p_excel.SetCellValue(5, 8, str);

	double area = m_winAtt.GetH()* m_winAtt.GetW()/1e6;

}
void CWindowMaterialUsage::ExprotAlInfo(Excel::CExcelUtil& p_excel)//����Ͳ�����
{
	CString str;
	const vector<CAluminumFormula> alFormulas = CWindowFormula::Instance()->GetAluminumFormulas(m_winAtt.m_prototypeCode);

	E_WindowDoorType winType = GetWindowDoorType(m_winAtt.m_openType);

	int nRow = 17;
	for (UINT i=0; i<alFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 2, alFormulas[i].m_aluminumClassify);

		//�Ͳı��
		CAluminumData dataOut;
		bool bSuc = CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(winType, m_winAtt.m_material.sAluminumSerial, alFormulas[i].m_name, dataOut);
		if (bSuc)
		{
			p_excel.SetCellValue(nRow, 5, dataOut.sCode);
		}

		//����
		double length = GetVauleByFomula(alFormulas[i].m_formula);
		str.Format(_T("%d"),(int)(length));
		p_excel.SetCellValue(nRow, 6, str);



	}

}
//void CWindowMaterialUsage::ExportGlassInfo(Excel::CExcelUtil& p_excel); //�����������
//void CWindowMaterialUsage::ExportHardwareInfo(Excel::CExcelUtil& p_excel) ;//����������
//void CWindowMaterialUsage::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) ;//�����������

