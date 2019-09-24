#include "StdAfx.h"
#include "WebIO.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <utility>
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"

using namespace std;

WebIO::WebIO()
{

}

WebIO::~WebIO()
{

}

std::vector<AttrWindow *>  WebIO::GetWindows
(
   double width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
   double height,
   CString openType, //��������
   int openNum,//����������
   CString gongNengQu,//������
   double tongFengLiang//ͨ����
)
{
	std::vector<AttrWindow *> result;

#ifdef WORK_LOCAL//����ģʽ
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*_Window.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrWindow *pAttribute = new AttrWindow();
		CString strid;
		strid.Format(L"%s%d_", L"C",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_filePathName = localFiles[i].second;
		int len = pAttribute->m_name.GetLength();
		pAttribute->m_name.Delete(len - 4, 4);
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"�ⴰ";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrWindow *>  WebIO::GetAllWindows()
{
	std::vector<AttrWindow *> result;

#ifdef WORK_LOCAL//����ģʽ
	result = GetWindows(0,0,L"",0,L"",0);
#else

#endif

	return result;
}

std::vector<AttrDoor *> WebIO::GetDoors
(
    double minWid, //���ֵ��ע��߶�ֵ����Ϊ�������� 
    CString doorType //������
)
{
	std::vector<AttrDoor *> result;
#ifdef WORK_LOCAL//����ģʽ
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrDoor *pAttribute = new AttrDoor();
		CString strid;
		strid.Format(L"%s_%d", L"L_DOOR_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name = pAttribute->m_name.Left(pAttribute->m_name.GetLength() - 4);
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"��";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrDoor *> WebIO::GetAllDoors()
{
	std::vector<AttrDoor *> result;
#ifdef WORK_LOCAL//����ģʽ
	result = GetDoors(0,L"");
#else

#endif
	return result;
}

std::vector<AttrKitchen *> WebIO::GetKitchens
(
  double kaiJian,//����
  double jinShen,//����
  CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
  CString type,//��������
  bool hasPaiQiDao//�Ƿ���������
)
{
	std::vector<AttrKitchen *> result;
#ifdef WORK_LOCAL//����ģʽ
	CString localKitchenPath = TY_GetLocalFilePath();
	CString localFile;

	if (type == L"Uq")
	{
		if (weiZhiGuanXi == L"�Կ�")
		{
			if (jinShen < 2000)
				localFile = L"KUq_Dui.dwg";
			else
				localFile = L"KUq_Dui_ZhuanJIao.dwg";
		}
		else
		{
			if (jinShen < 2000)
				localFile = L"KUq_Chui.dwg";
			else
				localFile = L"KUq_Chui_ZhuanJIao.dwg";
		}
	}
	else if (type == L"Us")
		localFile = L"KUs.dwg";
	else if (type == L"L")
		localFile = L"KL.dwg";
	else
		localFile = L"KI.dwg";
	if (hasPaiQiDao)
		localFile.Replace(_T(".dwg"), _T("_P.dwg"));

	AttrKitchen *pAttribute = new AttrKitchen();
	pAttribute->m_yxid.Format(L"K%s_%.0lf��%.0lf", type, kaiJian, jinShen);
	if (weiZhiGuanXi == L"��ֱ��")
		pAttribute->m_yxid += L"_c";
	pAttribute->m_name = localFile;
	pAttribute->m_isJiTuan = true;
	pAttribute->m_isDynamic = true;
	pAttribute->m_type = L"����";
	pAttribute->m_filePathName = localKitchenPath;
	pAttribute->m_kitchenType = type;
	pAttribute->m_windowDoorPos = weiZhiGuanXi;

	result.push_back(pAttribute);
	pAttribute->close();
#else

#endif
	return result;
}

std::vector<AttrKitchen *> WebIO::GetAllKitchens()
{
	std::vector<AttrKitchen *> result;
#ifdef WORK_LOCAL//����ģʽ
	result = GetKitchens(0,0,L"",L"",0);
#else

#endif
	return result;
}

std::vector<AttrToilet *>  WebIO::GetToilets
(
    double kaiJian,//����
    double jinShen,//����
    CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
    CString type,//����������
    bool hasPaiQiDao//�Ƿ���������
)
{
	std::vector<AttrToilet *> result;
#ifdef WORK_LOCAL//����ģʽ
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*_Toilet.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrToilet *pAttribute = new AttrToilet();
		CString strid;
		strid.Format(L"%s_%d", L"L_TOILETN_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"������";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrToilet *> WebIO::GetAllToilets()
{
	std::vector<AttrToilet *> result;
#ifdef WORK_LOCAL//����ģʽ
	result = GetToilets(0,0,L"",L"",0);
#else

#endif
	return result;
}

std::vector<AttrAirCon *> WebIO::GetAirCons
(
    double piShu,
	CString weiZhi,//����ˮ��λ��
	bool hasYuShuiGuan,
	CString yuShuiGuanWeizhi
)
{
	std::vector<AttrAirCon *> result;
#ifdef WORK_LOCAL//����ģʽ
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrAirCon *pAttribute = new AttrAirCon();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"�յ�";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrAirCon *> WebIO::GetAllAirCons()
{
	std::vector<AttrAirCon *> result;
#ifdef WORK_LOCAL//����ģʽ
	result = GetAirCons(0,L"",0,L"");
#else

#endif
	return result;
}

std::vector<AttrRailing *> WebIO::GetRailings(eRailingType type)//һ���������е�
{
	std::vector<AttrRailing *> result;
#ifdef WORK_LOCAL//����ģʽ
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"_Railing1.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrRailing *pAttribute = new AttrRailing();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"����";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrRailing *> WebIO::GetAllRailings()
{
	std::vector<AttrRailing *> result;
#ifdef WORK_LOCAL//����ģʽ
	result = GetRailings(TIEYI);
#else

#endif
	return result;
}


int WebIO::DownLoadFile(CString id, CString filePathName)
{
	return 0;
}

//��ȡ�ߴ�ֵ
SRCDimData WebIO::Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName)
{
	SRCDimData value;
#ifdef WORK_LOCAL//����ģʽ
	if (fileName == L"Window_N_7_0.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1200);
			value.values.push_back(1500);
		}
		else if (dimStr == L"W1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(400);
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"����͹���Ƽ�ֵ950;���ҷ�͹���Ƽ�ֵ500;�����������Ƽ�ֵ400;";
		}
	}

	if (fileName == L"Window_N_7_1.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1600);
			value.values.push_back(2000);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(750);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(400);
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"����͹���Ƽ�ֵ950;���ҷ�͹���Ƽ�ֵ500;�����������Ƽ�ֵ400;";
		}
	}
	if (fileName == L"Window_N_9_0.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SINGLE;
			value.values.push_back(1800);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(550);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"����͹���Ƽ�ֵ950;���ҷ�͹���Ƽ�ֵ500;�����������Ƽ�ֵ400;";
		}
	}
	if (fileName == L"Window_N_9_1.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1900);
			value.values.push_back(2300);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(600);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"����͹���Ƽ�ֵ950;���ҷ�͹���Ƽ�ֵ500;�����������Ƽ�ֵ400;";
		}
	}
	if (fileName == L"Window_N_9_2.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(2400);
			value.values.push_back(3000);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(750);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"����͹���Ƽ�ֵ950;���ҷ�͹���Ƽ�ֵ500;�����������Ƽ�ֵ400;";
		}
	}
#else

#endif

	return value;
}