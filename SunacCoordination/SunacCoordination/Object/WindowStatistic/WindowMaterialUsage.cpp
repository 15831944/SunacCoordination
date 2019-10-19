#include "StdAfx.h"
#include "WindowMaterialUsage.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Str.h"
#include "..\..\Common\TYFormula.h"

CWindowMaterialUsage::CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount)
:m_winAtt(p_winAtt)
, m_nCount(p_nCount)
{
	m_alTotalQTY = 0;  
	m_broAlQTY = 0;   
	m_nonBroAlQTY = 0; 
	m_anoAlQTY = 0;	 

	m_glassTotalQTY = 0;	
	m_hardwareNumber = 0;	

	m_foamingQTY = 0;	
	m_glassSealantQTY = 0; 
	m_inSealantQTY = 0;  
	m_exSealantQTY = 0;  
	m_comGlueQTY = 0;	
	m_sealStripQTY = 0;	
	m_squSealStripQTY = 0;
	m_sealBurrsQTY = 0;	
	m_windowCost = 0;	

	m_yShuliaoTotallength = 0;
	m_yHengliaoTotallength = 0;
	m_cShuliaoTotallength = 0;
	m_cHengliaoTotallength = 0;
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
			if (sParaName.Find(_T("W")) >= 0 || sParaName.Find(_T("H")) >= 0 || sParaName.CompareNoCase(_T("a")) == 0 || sParaName.Compare(_T("R")) == 0)
			{
				valueOut = m_winAtt.GetValue(sParaName);
			}
			else
			{
				//�����Ĵӿۼ��ߴ����ȡ
				bool bSuc = CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(m_winAtt.m_openType, m_winAtt.m_material.sAluminumSerial, sParaName, valueOut);
				assert(bSuc);
			}
			parser.SetVar(sParaName, valueOut);
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
	CString reportTemplateXlsFile = TY_GetLocalFilePath() + _T("�Ŵ��������.xlsx");

	Excel::CExcelUtil xls;
	xls.OpenExcel(reportTemplateXlsFile); //�򿪱��
	xls.SetVisible(true); 
	xls.SetActiveSheet(2); //�򿪵ڶ��ű�

	ExportReportToExcel(xls);

	xls.SaveAs(p_sReportFile);

	return true;
}

bool CWindowMaterialUsageNC::ExportReportToExcel(Excel::CExcelUtil& p_excel)
{
	//p_excel.SetActiveSheet(2); //�򿪵ڶ��ű�

	ExportWindowInfo(p_excel);
	ExprotAlInfo(p_excel);
	ExportGlassInfo(p_excel);
	ExportHardwareInfo(p_excel);
	ExportFuliaoInfo(p_excel);

	return true;
}

void CWindowMaterialUsage::ExportWindowInfo(Excel::CExcelUtil& p_excel)//���������Ϣ
{
	CString str;

	p_excel.SetCellValue(2, 2, m_winAtt.m_prototypeCode);

	//�Ͳ�ϵ��
	p_excel.SetCellValue(2, 5, m_winAtt.m_material.sAluminumSerial);
	
	//�߶�
	str.Format(_T("%d"),(int)(m_winAtt.GetH()));
	p_excel.SetCellValue(4, 8, str);
	p_excel.SetCellValue(2, 9, str);

	//��
	str.Format(_T("%d"),(int)(m_winAtt.GetW()));
	p_excel.SetCellValue(5, 8, str);
	p_excel.SetCellValue(2, 7, str);


	//����ߴ�
	str.Format(_T("%d"), (int)((m_winAtt.GetH()) - m_winAtt.GetA() * 2));
	p_excel.SetCellValue(4, 9, str);
	str.Format(_T("%d"), (int)((m_winAtt.GetW()) - m_winAtt.GetA() * 2));
	p_excel.SetCellValue(5, 9, str);

	//H1
	str.Format(_T("%d"),(int)(m_winAtt.GetH1()));
	p_excel.SetCellValue(6, 9, str);
	
	if (m_winAtt.HasValue(_T("H2")))
	{
		double value = m_winAtt.GetValue(_T("H2"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(7, 9, str);		
	}
	if (m_winAtt.HasValue(_T("H3")))
	{
		double value = m_winAtt.GetValue(_T("H3"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(8, 9, str);
	}

	//W1
	str.Format(_T("%d"),(int)(m_winAtt.GetW1()));
	p_excel.SetCellValue(9, 9, str);


	if (m_winAtt.HasValue(_T("W2")))
	{
		double value = m_winAtt.GetValue(_T("W2"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(10, 9, str);
	}
	if (m_winAtt.HasValue(_T("W3")))
	{
		double value = m_winAtt.GetValue(_T("W3"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(11, 9, str);
	}
	if (m_winAtt.HasValue(_T("W4")))
	{
		double value = m_winAtt.GetValue(_T("W4"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(12, 9, str);
	}

	//a
	str.Format(_T("%d"),(int)(m_winAtt.GetA()*2));
	p_excel.SetCellValue(13, 9, str);

	//�Ŵ����
	double area = m_winAtt.GetH()* m_winAtt.GetW() / 1e6; //��λ�O
	str.Format(_T("%.2f"), area);
	p_excel.SetCellValue(2, 12, str);
	p_excel.SetCellValue(14, 9, str);
	p_excel.SetCellValue(72, 9, str);
	p_excel.SetCellValue(73, 9, str);

	//�Ŵ��ܳ�
	double circumference = (m_winAtt.GetH() + m_winAtt.GetW()) * 2;
	str.Format(_T("%.2f"), circumference);
	p_excel.SetCellValue(15, 9, str);
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
		p_excel.SetCellValue(nRow, 3, alFormulas[i].m_name);
		p_excel.SetCellValue(nRow, 4, alFormulas[i].m_pos);

		CAluminumData dataOut;
		bool bSuc = CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(winType, m_winAtt.m_material.sAluminumSerial, alFormulas[i].m_name, dataOut);
		if (bSuc)
		{
			//�Ͳı��
			p_excel.SetCellValue(nRow, 5, dataOut.sCode);
		}
		
		//����
		double length = GetVauleByFomula(alFormulas[i].m_formula);
		str.Format(_T("%d"),(int)(length));
		p_excel.SetCellValue(nRow, 6, str);

		//����
		str.Format(_T("%d"),alFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 7, str);

		//����
		str.Format(_T("%.3f"),dataOut.weightPerMeter);
		p_excel.SetCellValue(nRow, 8, str);

		//����
		double totalWeight = length * alFormulas[i].m_nCount * dataOut.weightPerMeter /1000;
		str.Format(_T("%.3f"),totalWeight);
		p_excel.SetCellValue(nRow, 9, str);

		//˰ǰ����
		p_excel.SetCellValue(nRow, 10, "0.00");

		//˰ǰ�ܼ�
		p_excel.SetCellValue(nRow, 11, "0.00");

		//����
		str.Format(_T("%.3f"),dataOut.wastageRate);
		p_excel.SetCellValue(nRow, 12, str);

		//�Ͳ�����
		str.Format(_T("%.lf"),dataOut.aluminumType);
		p_excel.SetCellValue(nRow, 13, str);
		
		//������Ÿ������Ͳĵȵ�����							
		if (0 == dataOut.aluminumType)
		{
			m_broAlQTY += totalWeight;
		}
		if (1 == dataOut.aluminumType)
		{
			m_nonBroAlQTY += totalWeight;
		}
		if (2 == dataOut.aluminumType)
		{
			m_anoAlQTY += totalWeight;
		}

		//����������ļ�����
		if (_T("ѹ��") == alFormulas[i].m_name && _T("����") == alFormulas[i].m_pos)
		{
			m_yShuliaoTotallength += length * alFormulas[i].m_nCount;
		}
		if (_T("ѹ��") == alFormulas[i].m_name && _T("����") == alFormulas[i].m_pos)
		{
			m_yHengliaoTotallength += length * alFormulas[i].m_nCount;
		}

		if ((_T("����") == alFormulas[i].m_name && _T("��") == alFormulas[i].m_pos) || (_T("����") == alFormulas[i].m_name && _T("��") == alFormulas[i].m_pos))
		{
			m_comGlueQTY += alFormulas[i].m_nCount;
		}

		if (_T("����") == alFormulas[i].m_name && _T("����") == alFormulas[i].m_pos)
		{
			m_cShuliaoTotallength += length*alFormulas[i].m_nCount;
		}
		if (_T("����") == alFormulas[i].m_name && _T("����") == alFormulas[i].m_pos)
		{
			m_cHengliaoTotallength += length*alFormulas[i].m_nCount;
		}
	}

	//���Ÿ������Ͳ�����
	str.Format(_T("%.2f"),m_broAlQTY);
	p_excel.SetCellValue(44, 9, str);

	//�Ƕ��Ÿ������Ͳ�����
	str.Format(_T("%.2f"),m_nonBroAlQTY);
	p_excel.SetCellValue(45, 9, str);

	//�����������Ͳ�����
	str.Format(_T("%.2f"),m_anoAlQTY);
	p_excel.SetCellValue(46, 9, str);

	//���Ͳ��ܼ�
	m_alTotalQTY = m_broAlQTY + m_nonBroAlQTY + m_anoAlQTY;
	str.Format(_T("%.2f"),m_alTotalQTY);
	p_excel.SetCellValue(46, 9, str);
}

void CWindowMaterialUsage::ExportGlassInfo(Excel::CExcelUtil& p_excel) //�����������
{
	CString str;
	const vector<CGlassFormula> glassFormulas = CWindowFormula::Instance()->GetGlassFormulas(m_winAtt.m_prototypeCode);

	int nRow = 49;
	for (UINT i=0; i<glassFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 2, glassFormulas[i].m_glassClassify);
		p_excel.SetCellValue(nRow, 3, glassFormulas[i].m_name);

		//�������
		double width = GetVauleByFomula(glassFormulas[i].m_widthFormula);
		str.Format(_T("%.2f"),width);
		p_excel.SetCellValue(nRow, 5, str);

		//�����߶�
		double height = GetVauleByFomula(glassFormulas[i].m_heightFormula);
		str.Format(_T("%.2f"),height);
		p_excel.SetCellValue(nRow, 6, str);

		//����
		str.Format(_T("%d"),glassFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 7, str);

		//���
		double area = height* width/1e6; //��λ�O
		str.Format(_T("%.2f"),area);
		p_excel.SetCellValue(nRow, 9, str);
		m_glassTotalQTY += area;

		//˰ǰ�ܼ�
		p_excel.SetCellValue(nRow, 11, "0.00");
	}
	//�����ϼ�
	str.Format(_T("%.2f"),m_glassTotalQTY);
	p_excel.SetCellValue(55, 9, str);
}

void CWindowMaterialUsage::ExportHardwareInfo(Excel::CExcelUtil& p_excel) //����������
{
	CString str;
	const vector<CHardwareData> hardwareFormulas = CWindowFormula::Instance()->GetHardwareData(m_winAtt.m_prototypeCode);

	int nRow = 57;
	for (UINT i=0; i<hardwareFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 3, hardwareFormulas[i].m_name);
		p_excel.SetCellValue(nRow, 7, _T("��"));

		//�������
		str.Format(_T("%.2f"),hardwareFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 9, str);
		m_hardwareNumber += hardwareFormulas[i].m_nCount;

		//˰ǰ����/�ܼ�
		p_excel.SetCellValue(nRow, 10, "0.00");
		p_excel.SetCellValue(nRow, 11, "0.00");
	}
	//�����ϼ�
	str.Format(_T("%.2f"),m_hardwareNumber);
	p_excel.SetCellValue(58, 9, str);
}

void CWindowMaterialUsage::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) //�����������
{
	CString str;

	//���ݼ�
	m_foamingQTY = (m_winAtt.GetH() + m_winAtt.GetW())*2;
	str.Format(_T("%.2f"),m_foamingQTY);
	p_excel.SetCellValue(60, 9, str);
	p_excel.SetCellValue(60, 10, "0.00");
	p_excel.SetCellValue(60, 11, "0.00");

	//���Թ�ͪ�ܷ⽺(�������ͺ�)
	m_glassSealantQTY = (m_yShuliaoTotallength + m_yHengliaoTotallength)*2/1000;
	str.Format(_T("%.2f"),m_glassSealantQTY);
	p_excel.SetCellValue(61, 9, str);
	p_excel.SetCellValue(61, 10, "0.00");
	p_excel.SetCellValue(61, 11, "0.00");

	//���Թ�ͪ�ܷ⽺(��ǽ��)
	m_inSealantQTY = m_foamingQTY/10;
	str.Format(_T("%.2f"),m_inSealantQTY);
	p_excel.SetCellValue(62, 9, str);
	p_excel.SetCellValue(62, 10, "0.00");
	p_excel.SetCellValue(62, 11, "0.00");

	//���Թ�ͪ�ܷ⽺(��ǽ��)
	m_exSealantQTY = m_foamingQTY/8;
	str.Format(_T("%.2f"),m_exSealantQTY);
	p_excel.SetCellValue(63, 9, str);
	p_excel.SetCellValue(63, 10, "0.00");
	p_excel.SetCellValue(63, 11, "0.00");

	//��ǽ�
	str.Format(_T("%.2f"),m_comGlueQTY);
	p_excel.SetCellValue(64, 9, str);
	p_excel.SetCellValue(64, 10, "0.00");
	p_excel.SetCellValue(64, 11, "0.00");
	
	//�ܷ⽺��(m)
	m_sealStripQTY = (m_cShuliaoTotallength + m_cHengliaoTotallength)*2/1000;
	str.Format(_T("%.2f"),m_sealStripQTY);
	p_excel.SetCellValue(65, 9, str);
	p_excel.SetCellValue(65, 10, "0.00");
	p_excel.SetCellValue(65, 11, "0.00");

	//�ܷ⽺��(�O)
	m_squSealStripQTY = (m_cShuliaoTotallength + m_cHengliaoTotallength)/1000;
	str.Format(_T("%.2f"),m_squSealStripQTY);
	p_excel.SetCellValue(66, 9, str);
	p_excel.SetCellValue(66, 10, "0.00");
	p_excel.SetCellValue(66, 11, "0.00");

	//��������װ���ķ�
	m_windowCost = m_winAtt.GetH()* m_winAtt.GetW()/1e6;
	str.Format(_T("%.2f"),m_windowCost);
	p_excel.SetCellValue(67, 9, str);
	p_excel.SetCellValue(67, 10, "0.00");
	p_excel.SetCellValue(67, 11, "0.00");
}

