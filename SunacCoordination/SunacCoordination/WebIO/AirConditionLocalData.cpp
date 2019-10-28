#include "StdAfx.h"
#include "string"
#include "afx.h"
#include "AirConditionLocalData.h"
#include "..\Tool\Excel\Excel.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"

using namespace Excel;

CAirConditionLocalData::CAirConditionLocalData()
{
	CString localWindowPath = TY_GetDataFilePath();
	LoadDataFromExcel((localWindowPath + ("ԭ������.xlsx")));
}

CAirConditionLocalData::~CAirConditionLocalData()
{
}


void CAirConditionLocalData::LoadDataFromExcel(CString p_file) 
{
	Excel::CExcelUtil xls; 
	bool bSuc = xls.OpenExcel(p_file); //�򿪱��
	if (bSuc == false)
	{
		AfxMessageBox(_T("�޷����ļ���") + p_file);
		return ;
	}

	xls.SetVisible(false); 

	//��ȡ�յ����ݱ��Ŀյ�����
	xls.SetActiveSheet(2); //�򿪵ڶ��ű�

	for (int i = 2; i <= 1000000; i++)  //ѭ��excel���  �յ����ݱ�����
	{
		AttrAirCon attrAirCon;

		attrAirCon.m_prototypeCode = xls.GetCellValue(i, 2);
		if (attrAirCon.m_prototypeCode.GetLength() == 0)
			break;

		attrAirCon.m_file.fileName = xls.GetCellValue(i, 3);
		attrAirCon.m_power = _ttof(xls.GetCellValue(i, 4));
		attrAirCon.m_pipePos = xls.GetCellValue(i, 5);
		CString str = xls.GetCellValue(i, 6);
		attrAirCon.m_hasRainPipe = (str==_T("��"))? true : false;
		attrAirCon.m_rainPipePos = xls.GetCellValue(i, 7);
		attrAirCon.m_airW = _ttoi(xls.GetCellValue(i, 8));
		attrAirCon.m_airH = _ttoi(xls.GetCellValue(i, 9));
		attrAirCon.m_airD = _ttoi(xls.GetCellValue(i, 10));
		
		m_allAirconditions.push_back(attrAirCon); //��������ӵ�vector����m_allAirconditions
	}

	xls.CloseExcel();//�رձ��
}


bool CAirConditionLocalData::GetAirConById(CString p_sId, AttrAirCon& value)const   //ͨ��ԭ�ͱ�Ŵ�m_allAirconditions�л�ȡ�յ�
{
	for (UINT i = 0; i < m_allAirconditions.size(); i++)
	{
		if (m_allAirconditions[i].m_prototypeCode == p_sId)
		{
			value = m_allAirconditions[i];
			return true;
		}
	}

	return false;
}

bool CAirConditionLocalData::GetAirConByFileName(CString p_sFileName, AttrAirCon&value)  const //ͨ���ļ�����m_allAirconditions�л�ȡ�յ�
{
	for (UINT i = 0; i < m_allAirconditions.size(); i++)
	{
		if (m_allAirconditions[i].GetFileName() == p_sFileName)
		{
			value = m_allAirconditions[i];
			return true;
		}
	}
	return false;
}

vector<AttrAirCon> CAirConditionLocalData::GetAllAirCons() const  //��ȡ���пյ�
{
	return m_allAirconditions;
}

//��ȡ����ɸѡ�����Ŀյ�
std::vector<AttrAirCon >  CAirConditionLocalData::GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const
{
	std::vector<AttrAirCon> data;

	for (UINT i =0; i < m_allAirconditions.size(); i++)
	{
		if (p_airConHorseNumber != m_allAirconditions[i].m_power)
		{
			continue;
		}

		if (p_airConPipePos != m_allAirconditions[i].m_pipePos)
		{
			continue;
		}

		if (p_airConRainRiser != L"��")
		{
			if (p_airConRainRiserPos !=  m_allAirconditions[i].m_rainPipePos)
			{
				continue;
			}
		}
		
		data.push_back(m_allAirconditions[i]);
	}

	return data;

}