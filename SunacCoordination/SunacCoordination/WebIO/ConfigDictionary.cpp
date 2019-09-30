#include "StdAfx.h"
#include "../Sunac_def.h"
#include "ConfigDictionary.h"
#include <xstring>
#include <atlconv.h>  
#include "SunacCadWeb\ArgumentSettingServiceSoap.nsmap" //ֻ�ð���һ�Σ��ұ������һ��
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"

#include "..\Tool\MarkupXml\Markup.h"

using namespace std;

CConfigDictionary::CConfigDictionary()
{
	InitFromWeb();
}

CConfigDictionary::~CConfigDictionary()
{

}

//�ӿ��ĵ� https://docs.qq.com/doc/DUUpaanJLZmlSQ2d4

void CConfigDictionary::GetConfig(wstring Term)
{
	vector<wstring> paraOut;
	bool bSuc1 = GetConfigFromWeb(Term, paraOut);
	if (bSuc1 && paraOut.size()>0)
	{
		vCString allPara;
		for (vector<wstring>::iterator it = paraOut.begin(); it != paraOut.end(); it++)
		{
			CString str = it->c_str();
			allPara.push_back(str);
		}

		CString cstr = Term.c_str();
		m_configDict[cstr] = allPara;
	}
}

bool CConfigDictionary::InitFromWeb()
{	
	//����
	GetConfig(_T("area"));

	//������
	wstring wstr = _T("actionType");
	GetConfig(wstr);

	//�Ŵ�λ�ù�ϵ
	wstr = _T("actionType");
	GetConfig(wstr);

	//��������
	wstr = _T("actionType");
	GetConfig(wstr);

	//����������
	wstr = _T("openWindowNum");
	GetConfig(wstr);

	//ͨ�������������ֵ
	wstr = _T("airVolumeAreaRation");
	GetConfig(wstr);

	//�Ŵ�����ǽ����
	wstr = _T("doorWindowDistance");
	GetConfig(wstr);

	//������
	wstr = _T("doorType");
	GetConfig(wstr);

	//��������
	wstr = _T("kitchenType");
	GetConfig(wstr);

	//����ˮ������
	wstr = _T("kitchenBasinType");
	GetConfig(wstr);

	//��������
	wstr = _T("refrigeratorType");
	GetConfig(wstr);

	//��̨���
	wstr = _T("hearthWidth");
	GetConfig(wstr);

	//����������
	wstr = _T("toiletType");
	GetConfig(wstr);

	//������̨����
	wstr = _T("toiletBasinWidth");
	GetConfig(wstr);

	//��Ͱ���
	wstr = _T("closesToolWidth");
	GetConfig(wstr);

	//��ϴ�����
	wstr = _T("bathroomWidth");
	GetConfig(wstr);

	//�յ�ƥ��
	wstr = _T("airConditionNumber");
	GetConfig(wstr);

	//����ˮ��λ��
	wstr = _T("condensatePipePosition");
	GetConfig(wstr);

	//��ˮ��λ��
	wstr = _T("rainPipePosition");
	GetConfig(wstr);

	//��������
	wstr = _T("handRail");
	GetConfig(wstr);

	return true;
}

bool CConfigDictionary::GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut)
{
	p_paraOut.clear();

	_ns1__StandardDesignAttribute desingAtt;
	desingAtt.AtrributeName = &p_paraTypeName;
	_ns1__StandardDesignAttributeResponse attResult;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.StandardDesignAttribute(&desingAtt, attResult);
	//�жϵ�ָ��Ϊ�գ�δ����xmlʱ������false
	if (attResult.StandardDesignAttributeResult == NULL)
	{
		return false;
	}

	//UINT  len = (attResult.StandardDesignAttributeResult)->length();
	//MessageBox(NULL, attResult.StandardDesignAttributeResult->c_str(), _T("���ؽ��"), 0);

	//�����ַ��������
	CMarkup xml;	

	xml.SetDoc((*(attResult.StandardDesignAttributeResult)).c_str());
	
	xml.ResetMainPos();
	xml.FindElem();	//���ڵ�
	xml.IntoElem();
	{
		wstring sCode;
		if (xml.FindElem(_T("Code")))
		{
			sCode = xml.GetData();
		}
		wstring sMsg;
		if (xml.FindElem(_T("Message")))
		{
			sMsg = xml.GetData();
		}

		wstring str;
		if (false == xml.FindElem(_T("ArgumentSettings")))
		{
			return false;
		}

		xml.IntoElem();
		while (xml.FindElem(_T("ArgumentSetting")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("ArgumentText")))
			{
				str = xml.GetData();
				p_paraOut.push_back(str);
			}
			xml.OutOfElem();
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();
	
	return true;
}

vCString CConfigDictionary::GetQuyus()const
{
	//if (m_configDict.) // ������û�����key���е�
	//{
	//	return m_configDict[_T("area")];
	//}
	//else
	{
		vCString strs;
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		strs.push_back(L"�Ϻ�����");
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		return strs;
	}
}
vCString CConfigDictionary::GetGongNengQus()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����");
	strs.push_back(L"����");
	strs.push_back(L"������");
	strs.push_back(L"��̨");
	strs.push_back(L"����");
	strs.push_back(L"����");
#else

#endif
	return strs;
}
vCString CConfigDictionary::GetWindowDoorPoss()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"�Ŵ��Կ�");
	strs.push_back(L"�Ŵ���ֱ��");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Window_GetOpenTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����");
	strs.push_back(L"�ڿ�");
	strs.push_back(L"�⿪");
	strs.push_back(L"����");
	strs.push_back(L"����");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetOpenAmount()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����");
	strs.push_back(L"1");
	strs.push_back(L"2");
	strs.push_back(L"3");
	strs.push_back(L"4");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetRate()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"1/4");
	strs.push_back(L"1/5");
	strs.push_back(L"1/6");
	strs.push_back(L"1/8");
	strs.push_back(L"1/10");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetWallDis()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"0");
	strs.push_back(L"50");
	strs.push_back(L"100");
	strs.push_back(L"150");
	strs.push_back(L"200");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Door_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����");
	strs.push_back(L"����");
	strs.push_back(L"�⿪");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"U��");
	strs.push_back(L"L��");
	strs.push_back(L"I��");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Kitchen_GetShuiPenTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����600");
	strs.push_back(L"����800");
	strs.push_back(L"˫��900");
	strs.push_back(L"˫��1000");
	strs.push_back(L"˫��1200");
#else

#endif

	return strs;
}
vCString CConfigDictionary::Kitchen_GetBingXiangTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"������700");
	strs.push_back(L"�Կ���800");
	strs.push_back(L"�Կ���1000");
#else

#endif

	return strs;
}
vCString CConfigDictionary::Kitchen_GetZaoTaiWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"800");
	strs.push_back(L"900");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"U��");
	strs.push_back(L"L��");
	strs.push_back(L"I��");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetTaiPenWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"650");
	strs.push_back(L"750");
	strs.push_back(L"800");
	strs.push_back(L"900");
	strs.push_back(L"1000");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetMaTongTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"750");
	strs.push_back(L"800");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetGuanXiWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"950");
	strs.push_back(L"1050");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Air_GetPiShus()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"1.0P");
	strs.push_back(L"1.5P");
	strs.push_back(L"2.0P");
	strs.push_back(L"3.0P");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Air_GetLengNingShuiGuanPos()const//�������ˮ��λ��
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ

	strs.push_back(L"����");
	strs.push_back(L"����");


#else

#endif
	return strs;
}

vCString CConfigDictionary::Air_GetYuShuiGuanPos()const//�����ˮ��λ��
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"����");
	strs.push_back(L"����");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Railing_GetTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"��������");
	strs.push_back(L"��������");
#else

#endif
	return strs;
}
