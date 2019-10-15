#include "StdAfx.h"
#include "string"
#include "afx.h"
#include "WindowLocalData.h"
#include "..\Tool\Excel\Excel.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"

using namespace Excel;

CWindowLocalData::CWindowLocalData()
{
	CString localWindowPath = TY_GetLocalFilePath();
	LoadDataFromExcel((localWindowPath + ("�ⴰ����.xlsx")));
}

CWindowLocalData::~CWindowLocalData()
{
}

CWindowsDimData CWindowLocalData::ReadDimData(Excel::CExcelUtil &xls, CString code, int p_row, int p_colum)
{
	CString valueType = xls.GetCellValue(p_row, p_colum);
	CString sValue = xls.GetCellValue(p_row, p_colum + 1);
	double minValue = _wtof(xls.GetCellValue(p_row, p_colum + 2));
	double maxValue = _wtof(xls.GetCellValue(p_row, p_colum + 3));
	double defaultValue = _wtof(xls.GetCellValue(p_row, p_colum + 4));
	CString sComment = xls.GetCellValue(p_row, p_colum + 5);   //˵��

	CWindowsDimData  data;
	data.sCodeName = code;
	data.defaultValue = defaultValue;
	data.minValue = minValue;
	data.maxValue = maxValue;
	data.prompt = sComment;

	if (valueType == "��")
	{
		data.type = NOVALUE;
	}
	else if (valueType == L"��ʽ")
	{
		data.type = CALC;
		data.sFomula = sValue;
	}
	else if (valueType == L"�̶�ֵ")
	{
		data.type = SINGLE;
		data.valueOptions.push_back(_wtof(sValue));
	}
	else if (valueType == L"ֵϵ��")
	{
		data.type = MULTI;

		std::vector<CString> strs = YT_SplitCString(sValue, L',');
		for (UINT i = 0; i < strs.size(); i++)
		{
			data.valueOptions.push_back(_wtof(strs[i]));
		}
	}
	
	return data;
}

//�Ŵ�
CWindowsDimData CWindowLocalData::ConvertStringToDimData ( CString code, CString  valueType, CString value, CString defaultValue,	CString state) const
{
	CWindowsDimData  data;
	if (valueType == "��")
	{
		data.sCodeName = code;
		data.type = NOVALUE;
	}
	else if(valueType == L"��ʽ")
	{
		data.sCodeName = code;
		data.type = CALC;
	}
	else if(valueType == L"�̶�ֵ")
	{
		data.sCodeName = code;
		data.type = SINGLE;
		data.valueOptions.push_back(_wtof(value));
		data.defaultValue = _wtof(defaultValue);
		data.prompt = state;
	}
	else if(valueType == L"ֵϵ��")
	{
		data.sCodeName = code;
		data.type = MULTI;

		std::vector<CString> strs = YT_SplitCString(value, L',');
		for (UINT i = 0; i < strs.size(); i++)
		{
			data.valueOptions.push_back(_wtof(strs[i]));
		}
		data.defaultValue = _wtof(defaultValue);
		data.prompt = state;
	}
	return data;
}

void CWindowLocalData::LoadDataFromExcel(CString p_file) 
{
	Excel::CExcelUtil xls; 

	xls.OpenExcel(p_file); //�򿪱��
	xls.SetVisible(false); 


	//�����ݴӱ���ж�ȡ��m_windows�У���ȡ�ⴰ���ݱ�
	xls.SetActiveSheet(1); //�򿪵�һ�ű�

	CString str;

	for (int i = 2; i <= 1000000; i++)  //ѭ��excel���  �ⴰ���ݱ�����
	{
		AttrWindow attrwindow;
		
		//ͨ���к��л�ȡ��Ԫ���ֵ������ֵ��������attrwindow
		//attrwindow.m_id = xls.GetCellValue(i, 1); 
		attrwindow.m_prototypeCode = xls.GetCellValue(i, 2);
		if (attrwindow.m_prototypeCode.GetLength() == 0)  //��ԭ�ͱ�ŵĳ��Ƚ����жϣ���ԭ�ͱ��Ϊ�յ�ʱ�����ѭ��
			break;

		attrwindow.SetFileName(xls.GetCellValue(i, 3));
		attrwindow.m_quyuName = xls.GetCellValue(i, 4);
		if (attrwindow.m_quyuName.Find(_T("ȫ��"))>=0 ||
			attrwindow.m_quyuName.Find(_T("����")) >= 0)
		{
			attrwindow.m_isJiTuan = true;
		}
		else
		{
			attrwindow.m_isJiTuan = false;
		}

		str = xls.GetCellValue(i, 5);
		attrwindow.m_isDynamic = (str==_T("��"))? true : false;

		attrwindow.m_gongNengquType = xls.GetCellValue(i, 6);
		attrwindow.m_openType = xls.GetCellValue(i, 7);
		attrwindow.m_openQty = _ttoi(xls.GetCellValue(i, 8));
		attrwindow.m_tongFengFormula = xls.GetCellValue(i, 11);
		attrwindow.m_tongFengQty = _ttof(xls.GetCellValue(i, 14));

		//////////////////////////////////////////////////////////////////////////
		CWindowsDimData dimDataW;
		dimDataW.sCodeName = L"W";
		if (attrwindow.m_isDynamic)
		{
			dimDataW.type = SCOPE;
			dimDataW.minValue = _ttof(xls.GetCellValue(i, 9));
			dimDataW.maxValue = _ttof(xls.GetCellValue(i, 10));
		}
		else
		{
			dimDataW.type = SINGLE;
			dimDataW.value = _ttof(xls.GetCellValue(i, 12)); //���
		}
		attrwindow.SetDimData(dimDataW);

		CWindowsDimData dimDataH;
		dimDataH.sCodeName = L"H";
		if (attrwindow.m_isDynamic)
		{
			dimDataH.type = UNLIMIT;
		}
		else
		{
			dimDataH.type = SINGLE;
			dimDataH.value = _ttof(xls.GetCellValue(i, 13)); //���
		}
		attrwindow.SetDimData(dimDataH);

		//////////////////////////////////////////////////////////////////////////
		CWindowsDimData rcDimData = ReadDimData(xls, L"W1", i, 15);		
		attrwindow.SetDimData(rcDimData);

		rcDimData = ReadDimData(xls, L"W2", i, 21);
		attrwindow.SetDimData(rcDimData);

		rcDimData = ReadDimData(xls, L"W3", i, 27);
		attrwindow.SetDimData(rcDimData);

		rcDimData = ReadDimData(xls, L"H1", i, 33);
		attrwindow.SetDimData(rcDimData);

		rcDimData = ReadDimData(xls, L"H2", i, 39);
		attrwindow.SetDimData(rcDimData);

		rcDimData = ReadDimData(xls, L"H3", i, 45);
		attrwindow.SetDimData(rcDimData);

		//////////////////////////////////////////////////////////////////////////
		attrwindow.m_frontViewFile.fileName = xls.GetCellValue(i, 51);
		attrwindow.m_topViewFile.fileName = xls.GetCellValue(i, 52);
		attrwindow.m_leftViewFile.fileName = xls.GetCellValue(i, 53);

		//////////////////////////////////////////////////////////////////////////
		m_windows.push_back(attrwindow); 
	}

	xls.CloseExcel();//�رձ��
}


bool CWindowLocalData::GetWindowById(CString p_sId, AttrWindow& value) const //ͨ��ԭ�ͱ�Ż�ȡ����
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].m_prototypeCode == p_sId)
		{
			value = m_windows[i];
			return true;
		}
	}

	return false;
}

bool  CWindowLocalData::GetWindowByFileName(CString p_sFileName, AttrWindow&value)const//ͨ���ļ�����ȡ����
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].GetFileName() == p_sFileName)
		{
			value = m_windows[i];
			return true;
		}
	}
	return false;
}

vector<AttrWindow> CWindowLocalData::GetAllWindows() //��ȡ���д���
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //��"_"���
		if (strs[0] == "Window")
		{
			m_wins.push_back(m_windows[i]);
		}
	}
	return m_wins;
}

vector<AttrWindow> CWindowLocalData::GetAllDoors()  //��ȡ������
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');
		if (strs[0] == "Door")
		{
			m_doors.push_back(m_windows[i]);
		}
	}
	return m_doors;
}

std::vector<AttrWindow >  CWindowLocalData::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i =0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //��"_"���
		if (strs[0] != "Window")
		{
			continue;
		}

		const CWindowsDimData* pDiwW = m_windows[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"����")
		{
			if (openType != m_windows[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"����")
		{
			if (gongNengQu != m_windows[i].m_gongNengquType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != m_windows[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(m_windows[i]);
	}

	return data;
}

std::vector<AttrWindow >  CWindowLocalData::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //��"_"���
		if (strs[0] != "Door")
		{
			continue;
		}

		const CWindowsDimData* pDiwW = m_windows[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"����")
		{
			if (openType != m_windows[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"����")
		{
			if (gongNengQu != m_windows[i].m_gongNengquType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != m_windows[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(m_windows[i]);
	}

	return data;
}

