#include "StdAfx.h"
#include <algorithm>
#include "Sunac_def.h"
#include "Common/ComFun_String.h"


CString ToCstring(eRCType p_type)
{
	CString str;
	switch (p_type)
	{
	case S_WINDOW:
	case S_DOOR:
		return _T("�Ŵ�");
		break;
	case S_KITCHEN:
		return _T("����");
		break;
	case S_BATHROOM:
		return _T("������");
		break;
	case S_AIRCON:
		return _T("�յ�");
		break;
	case S_RAILING:
		return _T("����");
		break;
	default:
		break;
	}
	return str;
}

eWindowDoorPos ToEWindowDoorPos(CString type)
{
	if (type == "�Կ�")
	{
		return DUIKAI;
	}
	else if (type == "��ֱ��")
	{
		return CHUIZHIKAI;
	}
	else
	{
		ASSERT(FALSE);
		return DUIKAI;
	}
}

CString ViewDir2String(const eViewDir p_viewDir)
{
	switch (p_viewDir)
	{
	case E_VIEW_FRONT://����ͼ
		return _T("����");
	case E_VIEW_TOP:	//ƽ��ͼ
		return _T("ƽ��");
	case E_VIEW_LEFT://����ͼ
		return _T("����");
	case E_VIEW_EXTEND://չ��ͼ
		return _T("չ��");
	default:
		return _T("");
		break;
	}
}

eViewDir String2ViewDir(const CString p_sDir)
{
	eViewDir dir = E_VIEW_TOP;
	if (p_sDir== _T("����"))
		dir = E_VIEW_FRONT;
	else if (p_sDir == _T("ƽ��"))
		dir = E_VIEW_TOP;
	else if (p_sDir == _T("����"))
		dir = E_VIEW_LEFT;
	else if (p_sDir == _T("չ��"))
		dir = E_VIEW_EXTEND;
	else if (p_sDir == _T("ȫ��"))
		dir = E_VIEW_ALL;
	else
	{
		assert(false);
	}

	return dir;
}

E_DIRECTION String2Direction(CString p_sDir)
{
	if (p_sDir.Find(_T("��"))>=0 || p_sDir.Find(_T("��"))>=0 || p_sDir.CompareNoCase(_T("S"))==0)
	{
		return E_DIR_BOTTOM;
	}
	else if (p_sDir.Find(_T("��")) >= 0 || p_sDir.Find(_T("��")) >= 0 || p_sDir.CompareNoCase(_T("E")) == 0)
	{
		return E_DIR_RIGHT;
	}
	else if (p_sDir.Find(_T("��")) >= 0 || p_sDir.Find(_T("��")) >= 0 || p_sDir.CompareNoCase(_T("N")) == 0)
	{
		return E_DIR_TOP;
	}
	else if (p_sDir.Find(_T("��")) >= 0 || p_sDir.Find(_T("��")) >= 0 || p_sDir.CompareNoCase(_T("W")) == 0)
	{
		return E_DIR_LEFT;
	}
	else
	{
		return E_DIR_UNKNOWN;
	}
}

eRailingType ToERailingType(CString type)
{
	if (type == "����" || type == "��������")
	{
		return E_RAILING_TIEYI;
	}
	else if (type == "����" || type == "��������")
	{
		return E_RAILING_BOLI;
	}
	else
	{
		ASSERT(FALSE);
		return E_RAILING_TIEYI;
	}
}

CFloorInfo::CFloorInfo()
{
	m_floorHeight = 2900;
}


bool CFloorInfo::SetFloorHeight(double p_height)
{ 
	if (p_height>100 &&p_height<10000)
	{
		m_floorHeight = p_height;
		return true;
	} 

	return false;
}

bool CFloorInfo::FloorLessCmp(CString p_sFloor1, CString p_sFloor2)
{
	vector<int> floors1 = StringToIntVector(p_sFloor1);
	vector<int> floors2 = StringToIntVector(p_sFloor2);
	if (floors1.size() == 0)
		return true;
	else if (floors2.size() == 0)
		return false;
	
	return floors1[0] < floors2[0];
}
vector<int> CFloorInfo::StringToIntVector(const CString p_sFloors)
{
	vector<int> floors;
	std::vector<CString> strs = YT_SplitCString(p_sFloors, L',');
	for (UINT i = 0; i < strs.size(); i++)
	{
		int nPos = strs[i].Find(_T('-'));
		if (nPos >= 0) //����
		{
			CString str1 = strs[i].Left(nPos);
			CString str2 = strs[i].Mid(nPos + 1);
			int nStart = _ttoi(str1);
			int nEnd = _ttoi(str2);
			if (nStart > nEnd || nStart == 0)
			{
				return floors;
			}

			for (int n = nStart; n <= nEnd; n++)
			{
				floors.push_back(n);
			}
		}
		else
		{
			int nFloor = _ttoi(strs[i]);
			if (nFloor == 0)
			{
				return floors;
			}

			floors.push_back(nFloor);
		}
	}

	//ȥ���ظ�Ԫ��
	sort(floors.begin(), floors.end());
	floors.erase(unique(floors.begin(), floors.end()), floors.end());

	return floors;
}
bool CFloorInfo::SetFloors(CString p_sFloors)
{
	if (p_sFloors != p_sFloors.SpanIncluding(_T("0123456789, -")))
	{
		return false;
	}

	vector<int> floors = StringToIntVector(p_sFloors);
	if (floors.size()==0)
	{
		return false;
	}
	
	m_sFloors = p_sFloors;
	m_nAllFloors = floors;

	return true;
}

bool CFloorInfo::AddFloors(CString p_sFloors) //��ԭ���Ļ��������
{
	vector<int> floors = StringToIntVector(p_sFloors);
	if (floors.size() == 0)
	{
		return false;
	}

	if (m_sFloors.IsEmpty())
	{
		m_sFloors = p_sFloors;
	}
	else
	{
		if (m_sFloors.Find(p_sFloors)<0)
		{
			m_sFloors += _T(",") + p_sFloors;
		}
	}

	m_nAllFloors.insert(m_nAllFloors.end(), floors.begin(), floors.end());

	return true;
}

int CFloorInfo::GetFloorCountByFloor(CString p_sFloor)const
{
	vector<int> floors = StringToIntVector(p_sFloor);
	if (floors.size() == 0)
	{
		return 0;
	}

	int nCount = 0;
	for (UINT j = 0; j < m_nAllFloors.size(); j++)
	{
		for (UINT i = 0; i < floors.size(); i++)
		{
			if (m_nAllFloors[j]==floors[i])
			{
				nCount++;
			}
		}
	}

	return nCount;
}


int CFloorInfo::GetFloorCountByFloorIndex(int p_nFloor)const
{
	int nCount = 0;
	for (UINT j = 0; j < m_nAllFloors.size(); j++)
	{
		if (m_nAllFloors[j] == p_nFloor)
		{
			nCount++;
		}
	}

	return nCount;

}