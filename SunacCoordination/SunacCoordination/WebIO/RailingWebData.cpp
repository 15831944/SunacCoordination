#include "StdAfx.h"
#include "RailingWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include <string>

std::vector<AttrRailing> CRailingWebData::GetRailings(eRailingType RailingType)
{
/*
	CString opNum;
	opNum.Format(_T("%d"),openNum);

	std::wstring sOpenType = openType;
	std::wstring sOpenNum = opNum.GetString();
	std::wstring sGongnengqu = gongNengQu;

	_ns1__GetWindows ns;
	ns.width = p_width;
	ns.height = p_heigh;
	ns.openType = &sOpenType;
	ns.openNum = &sOpenNum;
	ns.gongNengQu = &sGongnengqu;*/

	//std::wstring sRailingType = RailingType;

	_ns1__GetAllHandrailByParam ns;

	ns.RailingType = 0;

	_ns1__GetAllHandrailByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllHandrailByParam(&ns, nsResponse);

	std::vector<AttrRailing> vRailingAttrs;

	//�жϷ��ؽ���Ƿ�ɹ�
	if (nsResponse.GetAllHandrailByParamResult == NULL)
	{
		return vRailingAttrs;
	}

	//�����ַ��������
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllHandrailByParamResult)).c_str());

	vRailingAttrs = ParseRailingsFromXML(xml);

	return vRailingAttrs;
}


std::vector<AttrRailing > CRailingWebData::ParseRailingsFromXML(CMarkup xml)const
{
	std::vector<AttrRailing> vRailingAttrs;
	AttrRailing RailingAttr;

	xml.ResetMainPos();
	xml.FindElem();	//���ڵ�
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vRailingAttrs;
		}
	}
	if (xml.FindElem(_T("Handrails")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Handrail")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				RailingAttr.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPath")))
			{
				RailingAttr.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "��")
				{
					RailingAttr.m_isJiTuan = TRUE;
				}
				else RailingAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				RailingAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				RailingAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("Handrailtype")))
			{
				RailingAttr.m_railingType = ToERailingType(xml.GetData());
			}
			xml.OutOfElem();
		}
		vRailingAttrs.push_back(RailingAttr);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vRailingAttrs;
}