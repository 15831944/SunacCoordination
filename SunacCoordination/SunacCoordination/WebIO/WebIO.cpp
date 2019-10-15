#include "StdAfx.h"
#include "WebIO.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <xstring>
#include <atlconv.h> 
#include <stdlib.h>
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "cpp_base64\src\Base64Decoder.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"

using namespace std;

WebIO::WebIO()
{

}

WebIO::~WebIO()
{

}

//width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
//openType, //��������
//openNum,//����������
//gongNengQu,//������
//tongFengLiang//ͨ����
//��ȡ�źʹ�
std::vector<AttrWindow>  WebIO::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
	vAttrWindow Local,Web;
/*#ifdef WORK_LOCAL*///����ģʽ
	Local = m_windowLocalData.GetWindows(width, openType, openNum, gongNengQu);
/*#else*/
	Web = m_windowWebData.GetWindows(width, 0, openType, openNum, gongNengQu);
	return Local;
/*#endif*/
}
std::vector<AttrWindow> WebIO::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//����ģʽ
	return m_windowLocalData.GetDoors(width, openType, openNum, gongNengQu);
#else
	return m_windowWebData.GetDoors(width, openType, openNum, gongNengQu);
#endif
}

std::vector<AttrKitchen> WebIO::GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao)
{
	std::vector<AttrKitchen> ret;

#ifdef WORK_LOCAL//����ģʽ
	int xLen = int(p_xLen + 0.5);
	int yLen = int(p_yLen + 0.5);

	CString sType;

	switch (p_type)
	{
	case E_KITCH_U:
		sType = L"KU";
		break;
	case E_KITCH_L:
		sType = L"KL";
		break;
	case E_KITCH_I:
		sType = L"KI";
		break;
	}

	std::vector<CProKitchen> prototypes = CProMrg::GetInstance()->GetAllProKitchens();

	for (UINT i = 0; i < prototypes.size(); i++)
	{
		if ((p_type == E_KITCH_ALL || prototypes[i].m_sType.Left(2) == sType) && prototypes[i].MatchPrototype(xLen, yLen, p_doorDir, p_windowDir) && prototypes[i].m_bHasPaiQiDao == p_hasPaiQiDao)
		{
			AttrKitchen attr(p_xLen, p_yLen, p_doorDir, p_windowDir, prototypes[i]);
			ret.push_back(attr);
		}
	}
#else

#endif

	return ret;
}

std::vector<AttrKitchen> WebIO::GetAllKitchens()
{
	std::vector<AttrKitchen> ret;

#ifdef WORK_LOCAL//����ģʽ
	std::vector<CProKitchen> prototypes = CProMrg::GetInstance()->GetAllProKitchens();

	for (UINT i = 0; i < prototypes.size(); i++)
	{
		AttrKitchen attr;
		//ֻ��ԭ�ͣ����಻��
		attr.m_fileName = prototypes[i].m_sFileName;
		ret.push_back(attr);
	}
#else

#endif

	return ret;
}

std::vector<AttrBathroom> WebIO::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
	std::vector<AttrBathroom> ret;

	int xLen = int(p_xLen + 0.5);
	int yLen = int(p_yLen + 0.5);

	CString sType;

	switch (p_type)
	{
	case E_BATHROOM_I:
		sType = L"TI";
		break;
	case E_BATHROOM_L:
		sType = L"TL";
		break;
	case E_BATHROOM_U:
		sType = L"TU";
		break;
	}

	std::vector<CProBathroom> prototypes = CProMrg::GetInstance()->GetAllProBathrooms();

	for (UINT i = 0; i < prototypes.size(); i++)
	{
		if ((p_type == E_BATHROOM_ALL || prototypes[i].m_sType.Left(2) == sType) && prototypes[i].MatchPrototype(xLen, yLen, p_doorDir, p_windowDir))
		{
			AttrBathroom attr(p_xLen, p_yLen, p_doorDir, p_windowDir, prototypes[i]);
			ret.push_back(attr);
		}
	}

	return ret;
}

std::vector<AttrBathroom> WebIO::GetAllBathrooms()
{
	std::vector<AttrBathroom> ret;
	std::vector<CProBathroom> prototypes = CProMrg::GetInstance()->GetAllProBathrooms();

	for (UINT i = 0; i < prototypes.size(); i++)
	{
		AttrBathroom attr;
		//ֻ��ԭ�ͣ����಻��
		attr.m_fileName = prototypes[i].m_sFileName;
		ret.push_back(attr);
	}

	return ret;
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
	for (UINT i = 0; i < localFiles.size(); i++)
	{
		AttrAirCon *pAttribute = new AttrAirCon();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_prototypeCode = strid;
		pAttribute->m_fileName = localFiles[i].first;
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
	for (UINT i = 0; i < localFiles.size(); i++)
	{
		AttrRailing *pAttribute = new AttrRailing();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_prototypeCode = strid;
		pAttribute->m_fileName = localFiles[i].first;
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
	result = GetRailings(E_RAILING_TIEYI);
#else

#endif
	return result;
}


//wstring=>string
std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs((char*)NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
// string => wstring
std::wstring String2WString(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs((wchar_t*)NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

bool WebIO::DownLoadFile(const int fileId, const CString filePathName)
{
	_ns1__CadFileDownload nsCadFile;
	nsCadFile.Id = fileId;
	_ns1__CadFileDownloadResponse cadFileResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.CadFileDownload(&nsCadFile, cadFileResponse);

	wstring* swReturn = cadFileResponse.CadFileDownloadResult;
	if (swReturn == NULL)
	{
		return false;
	}

	std::string sReturn = WString2String(*swReturn);

	Base64Decoder decoder;

	wstring filenameOut = filePathName;
	ofstream ofs(filenameOut, ofstream::out | ofstream::binary);
	if (ofs)
	{
		UINT numberOfBytes = sReturn.length() + 1;
		char* decodedBuffer = new char[numberOfBytes * 2];
		memset(decodedBuffer, 0, numberOfBytes * 2);

		int numberOfBytesDecoded = decoder.decode(sReturn.c_str(), numberOfBytes, decodedBuffer);
		ofs.write(decodedBuffer, numberOfBytesDecoded);
	}
	else
	{
		wcout << L"Cannot open file: " << filenameOut << endl;
	}

	ofs.close();

	return true;
}
