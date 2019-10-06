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
	CString localWindowPath = TY_GetLocalFilePath();
	LoadDataFromExcel((localWindowPath + ("�ⴰ����.xlsx")));
}

CAirConditionLocalData::~CAirConditionLocalData()
{
}


void CAirConditionLocalData::LoadDataFromExcel(CString p_file) 
{
	Excel::CExcelUtil xls; 
	xls.OpenExcel(p_file); //�򿪱��
	xls.SetVisible(false); 

	//��ȡ�յ����ݱ��Ŀյ�����
	xls.SetActiveSheet(2); //�򿪵ڶ��ű�

	for (int i = 2; i <= 1000000; i++)  //ѭ��excel���  �յ����ݱ�����
	{
		AttrAirCon attrAirCon;

		attrAirCon.m_airConId = xls.GetCellValue(i, 1); //ͨ���к��л�ȡ��Ԫ���ֵ������ֵ��������attrAirCon
		attrAirCon.m_prototypeCode = xls.GetCellValue(i, 2);
		if (attrAirCon.m_prototypeCode.GetLength() == 0)
			break;

		attrAirCon.m_fileName = xls.GetCellValue(i, 3);
		attrAirCon.m_airConHorseNumber = xls.GetCellValue(i, 4);
		attrAirCon.m_airConPipePos = xls.GetCellValue(i, 5);
		attrAirCon.m_airConRainRiser = xls.GetCellValue(i, 6);
		attrAirCon.m_airConRainRiserPos = xls.GetCellValue(i, 7);
		attrAirCon.m_airConInstallNetSize = xls.GetCellValue(i, 8);
		
		m_aircon.push_back(attrAirCon); //��������ӵ�vector����m_aircon
	}

	xls.CloseExcel();//�رձ��
}


bool CAirConditionLocalData::GetAirConById(CString p_sId, AttrAirCon& value)const   //ͨ��ԭ�ͱ�Ŵ�m_aircon�л�ȡ�յ�
{
	for (UINT i = 0; i < m_aircon.size(); i++)
	{
		if (m_aircon[i].m_prototypeCode == p_sId)
		{
			value = m_aircon[i];
			return true;
		}
	}

	return false;
}

bool CAirConditionLocalData::GetAirConByFileName(CString p_sFileName, AttrAirCon&value)  const //ͨ���ļ�����m_aircon�л�ȡ�յ�
{
	for (UINT i = 0; i < m_aircon.size(); i++)
	{
		if (m_aircon[i].m_fileName == p_sFileName)
		{
			value = m_aircon[i];
			return true;
		}
	}
	return false;
}

vector<AttrAirCon> CAirConditionLocalData::GetAllAirCon() const  //��ȡ���пյ�
{
	return m_aircon;
}

//��ȡ����ɸѡ�����Ŀյ�
std::vector<AttrAirCon >  CAirConditionLocalData::GetAirCon(CString p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const
{
	std::vector<AttrAirCon> data;

	for (UINT i =0; i < m_aircon.size(); i++)
	{
		if (p_airConHorseNumber != m_aircon[i].m_airConHorseNumber)
		{
			continue;
		}

		if (p_airConPipePos != m_aircon[i].m_airConPipePos)
		{
			continue;
		}

		if (p_airConRainRiser != L"��")
		{
			if (p_airConRainRiserPos !=  m_aircon[i].m_airConRainRiserPos)
			{
				continue;
			}
		}
		
		data.push_back(m_aircon[i]);
	}

	return data;

}