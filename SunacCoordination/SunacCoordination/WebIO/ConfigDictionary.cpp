#include "StdAfx.h"
#include "../Sunac_def.h"
#include "ConfigDictionary.h"
#include <xstring>
#include <atlconv.h>  
#include "SunacCadWeb\ArgumentSettingServiceSoap.nsmap" //ֻ�ð���һ�Σ��ұ������һ��
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "../WebIO/WindowWebData.h"

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
	GetConfig(_T("actionType"));

	//�Ŵ�λ�ù�ϵ
	GetConfig(_T("doorWindowPosition"));

	//��������
	GetConfig( _T("openType"));

	//����������
	GetConfig(_T("openWindowNum"));

	//ͨ�������������ֵ
	GetConfig(_T("airVolumeAreaRation"));

	//�Ŵ�����ǽ����
	GetConfig(_T("doorWindowDistance"));

	//������
	GetConfig(_T("doorType"));

	//��������
	GetConfig(_T("kitchenType"));

	//����ˮ������
	GetConfig(_T("kitchenBasinType"));

	//��������
	GetConfig(_T("refrigeratorType"));

	//��̨���
	GetConfig(_T("hearthWidth"));

	//����������
	GetConfig(_T("toiletType"));

	//������̨����
	GetConfig(_T("toiletBasinWidth"));

	//��Ͱ���
	GetConfig(_T("closesToolWidth"));

	//��ϴ�����
	GetConfig(_T("bathroomWidth"));

	//�յ�ƥ��
	GetConfig(_T("airConditionNumber"));

	//����ˮ��λ��
	GetConfig(_T("condensatePipePosition"));

	//��ˮ��λ��
	GetConfig(_T("rainPipePosition"));

	//��������
	GetConfig(_T("handRail"));

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
	//if (m_configDict.count("area") > 0) // ������û�����key���е�
	//{
	//	return m_configDict[_T("area")];
	//}
	auto iter = m_configDict.find(_T("area"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
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
	auto iter = m_configDict.find(_T("actionType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"����");
		strs.push_back(L"������");
		strs.push_back(L"��̨");
		strs.push_back(L"����");
		strs.push_back(L"����");
		return strs;
	}
}
vCString CConfigDictionary::GetWindowDoorPoss()const
{
	auto iter = m_configDict.find(_T("doorWindowPosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"�Ŵ��Կ�");
		strs.push_back(L"�Ŵ���ֱ��");
		return strs;
	}
}
vCString CConfigDictionary::Window_GetOpenTypes()const
{
	auto iter = m_configDict.find(_T("openType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"�ڿ�");
		strs.push_back(L"�⿪");
		strs.push_back(L"����");
		strs.push_back(L"����");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetOpenAmount()const
{
	auto iter = m_configDict.find(_T("openWindowNum"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"1");
		strs.push_back(L"2");
		strs.push_back(L"3");
		strs.push_back(L"4");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetRate()const
{
	auto iter = m_configDict.find(_T("airVolumeAreaRation"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"1/4");
		strs.push_back(L"1/5");
		strs.push_back(L"1/6");
		strs.push_back(L"1/8");
		strs.push_back(L"1/10");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetWallDis()const
{
	auto iter = m_configDict.find(_T("doorWindowDistance"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"0");
		strs.push_back(L"50");
		strs.push_back(L"100");
		strs.push_back(L"150");
		strs.push_back(L"200");
		return strs;
	}
}

vCString CConfigDictionary::Door_GetTypes()const
{
	auto iter = m_configDict.find(_T("doorType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"����");
		strs.push_back(L"�⿪");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	auto iter = m_configDict.find(_T("kitchenType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"U��");
		strs.push_back(L"L��");
		strs.push_back(L"I��");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetShuiPenTypes()const
{
	auto iter = m_configDict.find(_T("kitchenBasinType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����600");
		strs.push_back(L"����800");
		strs.push_back(L"˫��900");
		strs.push_back(L"˫��1000");
		strs.push_back(L"˫��1200");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetBingXiangTypes()const
{
	auto iter = m_configDict.find(_T("refrigeratorType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"������700");
		strs.push_back(L"�Կ���800");
		strs.push_back(L"�Կ���1000");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetZaoTaiWidths()const
{
	auto iter = m_configDict.find(_T("hearthWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"800");
		strs.push_back(L"900");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetTypes()const
{
	auto iter = m_configDict.find(_T("toiletType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"U��");
		strs.push_back(L"L��");
		strs.push_back(L"I��");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetTaiPenWidths()const
{
	auto iter = m_configDict.find(_T("toiletBasinWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"650");
		strs.push_back(L"750");
		strs.push_back(L"800");
		strs.push_back(L"900");
		strs.push_back(L"1000");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetMaTongTypes()const
{
	auto iter = m_configDict.find(_T("closesToolWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"750");
		strs.push_back(L"800");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetGuanXiWidths()const
{
	auto iter = m_configDict.find(_T("bathroomWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"950");
		strs.push_back(L"1050");
		return strs;
	}
}
vCString CConfigDictionary::Air_GetPiShus()const
{
	auto iter = m_configDict.find(_T("airConditionNumber"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"1.0P");
		strs.push_back(L"1.5P");
		strs.push_back(L"2.0P");
		strs.push_back(L"3.0P");
		return strs;
	}
}
vCString CConfigDictionary::Air_GetLengNingShuiGuanPos()const//�������ˮ��λ��
{
	auto iter = m_configDict.find(_T("condensatePipePosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"����");
		return strs;
	}
}

vCString CConfigDictionary::Air_GetYuShuiGuanPos()const//�����ˮ��λ��
{
	auto iter = m_configDict.find(_T("rainPipePosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"����");
		strs.push_back(L"����");
		return strs;
	}
}

vCString CConfigDictionary::Railing_GetTypes()
{
	auto iter = m_configDict.find(_T("handRail"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"��������");
		strs.push_back(L"��������");
		return strs;
	}
}
