#include "StdAfx.h"
#include "AirconditionWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include <string>

std::vector<AttrAirCon> CAirConditionWebData::GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi)
{
	CString AirconditionerIsRainpipe_;
	AirconditionerIsRainpipe_.Format(L"%d", hasYuShuiGuan);

/*
	std::wstring sAirconditionerPower = piShu;
	std::wstring sAirconditionerPipePosition = AirconditionerPipePosition;
	std::wstring sAirconditionerIsRainpipe = AirconditionerIsRainpipe_;
	std::wstring sRainpipePosition = RainpipePosition;*/

	_ns1__GetAllAirconditionerByParam ns;
	/*ns.AirconditionerPower = &sAirconditionerPower;
	ns.AirconditionerPipePosition = &sAirconditionerPipePosition;
	ns.AirconditionerIsRainpipe = &sAirconditionerIsRainpipe;
	ns.RainpipePosition = &sRainpipePosition;*/
	ns.AirconditionerPower = 0;
	ns.AirconditionerPipePosition = 0;
	ns.AirconditionerIsRainpipe = 0;
	ns.RainpipePosition = 0;

	_ns1__GetAllAirconditionerByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllAirconditionerByParam(&ns, nsResponse);

	std::vector<AttrAirCon> AirConAtts;

	//�жϷ��ؽ���Ƿ�ɹ�
	if (nsResponse.GetAllAirconditionerByParamResult == NULL)
	{
		return AirConAtts;
	}

	//AttrWindow attrwindow;

	//�����ַ��������
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllAirconditionerByParamResult)).c_str());

	AirConAtts = ParseAirConditionersFromXML(xml);
	return AirConAtts;
}

std::vector<AttrAirCon > CAirConditionWebData::ParseAirConditionersFromXML(CMarkup xml)const
{
	std::vector<AttrAirCon> vAirConAttrs;
	AttrAirCon AirConAttr;

	xml.ResetMainPos();
	xml.FindElem();	//���ڵ�
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vAirConAttrs;
		}
	}
	if (xml.FindElem(_T("Airconditioners")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Airconditioner")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				AirConAttr.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPath")))
			{
				AirConAttr.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "��")
				{
					AirConAttr.m_isJiTuan = TRUE;
				}
				else AirConAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				AirConAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				AirConAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerPower")))
			{
				AirConAttr.m_airConHorseNumber = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerMinWidth")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerMinLength")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerPipePosition")))
			{
				AirConAttr.m_airConPipePos = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerIsRainpipe")))
			{
				AirConAttr.m_airConRainRiser = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerRainpipePostion")))
			{
				AirConAttr.m_airConRainRiserPos = xml.GetData();
			}
			xml.OutOfElem();
		}
		vAirConAttrs.push_back(AirConAttr);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vAirConAttrs;
}