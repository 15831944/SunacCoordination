#include "StdAfx.h"
#include "../Sunac_def.h"
#include "ConfigDictionary.h"
#include <xstring>
#include <atlconv.h>  
#include "SunacCadWeb\ArgumentSettingServiceSoap.nsmap"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"


#include "..\Tool\MarkupXml\Markup.h"

using namespace std;

CConfigDictionary::CConfigDictionary()
{
	//InitFromWeb();
	//m_quyus = GetQuyus();//����
	//m_gongNengQus = GetGongNengQus();//������
	//m_windowDoorPoss = GetWindowDoorPoss();//�Ŵ�λ�ù�ϵ
	//m_windowOpenTypes = Window_GetOpenTypes();//��������
	//m_doorTypes = Door_GetTypes();//������
	//m_kitchenTypes = Kitchen_GetTypes();//��������
	//m_kitchenShuiPenTypes = Kitchen_GetShuiPenTypes();//ˮ������
	//m_kitchenBingXiangTypes = Kitchen_GetBingXiangTypes();//��������
	//m_kitchenZaoTaiWidths = Kitchen_GetZaoTaiWidths();//��̨���
	//m_toiletTypes = Toilet_GetTypes();//����������
	//m_toiletTaiPenWidths = Toilet_GetTaiPenWidths();//����������
	//m_toiletMaTongTypes = Toilet_GetMaTongTypes();//����������
	//m_toiletGuanXiWidths = Toilet_GetGuanXiWidths();//��ϴ�����---�Ƿ�֧���ֶ����룿��
	//m_airPiShus = Air_GetPiShus();//���е�ƥ��
	//m_airLengNingShuiGuanPos = Air_GetLengNingShuiGuanPos();//����ˮ��λ��
	//m_windowOpenAmount = Window_GetOpenAmount();//����������
	//m_rate = Window_GetRate();//��ֵ
	//m_windowWallDis = Window_GetWallDis();//��������ǽ����
}

CConfigDictionary::~CConfigDictionary()
{

}

//�ӿ��ĵ� https://docs.qq.com/doc/DUUpaanJLZmlSQ2d4
bool CConfigDictionary::InitFromWeb()
{
	vector<wstring> p_paraOut;
	bool bSuc1 = GetConfigFromWeb(_T("area"), p_paraOut);
	if (bSuc1)
	{
		//m_configDict[_T("area")] = p_paraOut;
	}



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

	//UINT  len = (attResult.StandardDesignAttributeResult)->length();
	//MessageBox(NULL, attResult.StandardDesignAttributeResult->c_str(), _T("���ؽ��"), 0);


	//�����ַ��������
	CMarkup xml;
	xml.SetDoc(*(attResult.StandardDesignAttributeResult));
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
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"��������");
	strs.push_back(L"��������");
	strs.push_back(L"�Ϻ�����");
	strs.push_back(L"��������");
	strs.push_back(L"��������");
	strs.push_back(L"��������");
	strs.push_back(L"��������");
	strs.push_back(L"��������");
#else

#endif
	return strs;
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
	strs.push_back(L"100");
	strs.push_back(L"120");
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
#else

#endif
	return strs;
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"��U��");
	strs.push_back(L"ǳU��");
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
	strs.push_back(L"����700");
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
	strs.push_back(L"����600");
	strs.push_back(L"����700");
	strs.push_back(L"˫��900");
	strs.push_back(L"˫��1000");
	strs.push_back(L"˫��1200");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetMaTongTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"800");
	strs.push_back(L"900");
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
	strs.push_back(L"1.0");
	strs.push_back(L"1.5");
	strs.push_back(L"2.0");
	strs.push_back(L"3.0");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Air_GetLengNingShuiGuanPos()const//�������ˮ��λ��
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"���");
	strs.push_back(L"���");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Air_GetYuShuiGuanPos() //�����ˮ��λ��
{
	vCString strs;
#ifdef WORK_LOCAL//����ģʽ
	strs.push_back(L"���");
	strs.push_back(L"���");
#else

#endif
	return strs;
}
