#pragma once
#include "StdAfx.h"
#include <string>
#include "WindowWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Common\ComFun_String.h"
#include "WebIO.h"


CWindowWebData::CWindowWebData()
{

}

CWindowWebData::~CWindowWebData()
{

}

CString GetFileName(const WCHAR *fullname)
{
	WCHAR filename[256]; 
	WCHAR name[256];
	wcscpy_s(name,fullname);
	WCHAR *p = wcstok(name,L"/");
	WCHAR *pre = NULL;
	while(p != NULL)
	{
		pre = p;
		p = wcstok(NULL, L"/");
	}
	wcscpy_s(filename,pre);
	return filename;
}

std::vector<AttrWindow > CWindowWebData::ParseWindowsFromXML(CMarkup xml)const
{
	std::vector<AttrWindow> windowAtts;

	xml.ResetMainPos();
	xml.FindElem();	//���ڵ�
	xml.IntoElem();
	if (xml.FindElem(_T("Windows")))
	{
		xml.IntoElem();

		if (xml.FindElem(_T("Code")))
		{
			if(_ttoi(xml.GetData()) != 100)
			{
				return windowAtts;
			}
		}

		while (xml.FindElem(_T("Window")))
		{
			xml.IntoElem();
			AttrWindow attrwindow;
			if (xml.FindElem(_T("DrawingCode")))
			{
				attrwindow.m_prototypeCode = xml.GetData();
			}

			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("Drawings")))
			{
				xml.IntoElem();
				{
					while (xml.FindElem(_T("Drawing")))
					{
						xml.IntoElem();
						{
							CString sFileType, tempString, sFileName, sFileID;
							if (xml.FindElem(_T("Id")))
							{
								sFileID = xml.GetData();
							}
							if (xml.FindElem(_T("CADPath")))
							{
								tempString = xml.GetData();
								if (tempString != "")
								{
									sFileName = GetFileName(tempString);//��ô���չ�����ļ���
								}
							}
							if (xml.FindElem(_T("CADType")))
							{
								sFileType = xml.GetData();
							}
							if (sFileType == "FrontViewFile")
							{
								attrwindow.m_frontViewFile.id = _ttoi(sFileID);
								attrwindow.m_frontViewFile.fileName = sFileName;
							}
							else if (sFileType == "LeftViewFile")
							{
								attrwindow.m_leftViewFile.id = _ttoi(sFileID);
								attrwindow.m_leftViewFile.fileName = sFileName;
							}
							else if (sFileType == "TopViewFile")
							{
								attrwindow.m_topViewFile.id = _ttoi(sFileID);
								attrwindow.m_topViewFile.fileName = sFileName;
							}
							else if (sFileType == "ExpandViewFile")
							{
								attrwindow.m_file.id = _ttoi(sFileID);
								attrwindow.m_file.fileName = sFileName;
							}
							//����ļ��Ƿ���ڣ�������������
							CString sFilePath = MD2010_GetAppPath() + L"\\support\\Sunac2019\\WebMode\\" + sFileName;
							if (!JHCom_FileExist(sFilePath))
							{
								WEBINST->DownloadFile(_ttoi(sFileID), sFilePath);
							}
						}
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
			}

			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"��")
				{
					attrwindow.m_isJiTuan = TRUE;
				}
				else attrwindow.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("AreaId")))
			{
				attrwindow.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				attrwindow.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingType")))
			{
				//attrwindow.m_type = xml.GetData();
			}
			if (xml.FindElem(_T("WindowDynamicType")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"��")
				{
					attrwindow.m_isDynamic = TRUE;
				}
				else attrwindow.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("WindowOpenTypeName")))
			{
				attrwindow.m_openType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenQtyName")))
			{
				attrwindow.m_openQty = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowHasCorner")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"��")
				{
					attrwindow.m_isZhuanJiao = TRUE;
				}
				else attrwindow.m_isZhuanJiao = FALSE;
			}

			double minVaule = 0;
			double maxValue = 100000;
			if (xml.FindElem(_T("WindowSizeMin")))
			{
				minVaule = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowSizeMax")))
			{
				maxValue = _ttof(xml.GetData());
			}

			CWindowsDimData dimDataW;
			dimDataW.sCodeName = L"W";
			if (attrwindow.m_isDynamic)
			{
				dimDataW.type = SCOPE;
				dimDataW.minValue = minVaule;
				dimDataW.maxValue = maxValue;
			}
			//else //TODO ֧�־�̬������
			//{
			//	dimDataW.type = SINGLE;
			//	dimDataW.value = _ttof(xls.GetCellValue(i, 12)); //���
			//}
			attrwindow.SetDimData(dimDataW);

			CWindowsDimData dimDataH;
			dimDataH.sCodeName = L"H";
			if (attrwindow.m_isDynamic)
			{
				dimDataH.type = UNLIMIT;
			}
			//else
			//{
			//	dimDataH.type = SINGLE;
			//	dimDataH.value = _ttof(xls.GetCellValue(i, 13)); //���
			//}
			attrwindow.SetDimData(dimDataH);

			//////////////////////////////////////////////////////////////////////////
			if (xml.FindElem(_T("WindowDesignFormula")))
			{
				attrwindow.m_tongFengFormula = xml.GetData();
			}
			if (xml.FindElem(_T("WindowVentilationQuantity")))
			{
				attrwindow.m_tongFengQty = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowPlugslotSize")))
			{
				attrwindow.m_plugslotSize = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowFunctionalArea")))
			{
				attrwindow.m_gongNengquType = xml.GetData();
			}

			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				while (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					CWindowsDimData tempData;
					if (xml.FindElem(_T("SizeNo")))
					{
						tempData.sCodeName = xml.GetData();
					}
					if (xml.FindElem(_T("ValueTypeName")))
					{
						tempData.type = ToEWindowType(xml.GetData());
					}
					if (xml.FindElem(_T("Val")))
					{
						CString value = xml.GetData();
						if (tempData.type == MULTI)
						{
							std::vector<CString> strs = YT_SplitCString(value, L',');
							for (UINT i = 0; i < strs.size(); i++)
							{
								tempData.valueOptions.push_back(_wtof(strs[i]));
							}
						}
						else if (tempData.type == CALC)
						{
							tempData.sFomula = value;
						}
						else tempData.valueOptions.push_back(_ttof(value));

					}
					if (xml.FindElem(_T("MinValue")))
					{
						tempData.minValue = _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						tempData.maxValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						tempData.defaultValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("Desc")))
					{
						tempData.prompt = xml.GetData();
					}
					attrwindow.SetDimData(tempData);

					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
			windowAtts.push_back(attrwindow);
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();
	return windowAtts;
}

std::vector<AttrWindow> CWindowWebData::ParseDoorsFromXML(CMarkup xml)const
{
	std::vector<AttrWindow> DoorAttrs;

	xml.ResetMainPos();
	xml.FindElem();	//���ڵ�
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return DoorAttrs;
		}
	}
	if (xml.FindElem(_T("Doors")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Door")))
		{
			AttrWindow Attrdoor;
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				Attrdoor.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("Drawings")))
			{
				xml.IntoElem();
				{
					while (xml.FindElem(_T("Drawing")))
					{
						while (xml.FindElem(_T("Drawing")))
						{
							xml.IntoElem();
							{
								CString sFileType, tempString, sFileName, sFileID;
								if (xml.FindElem(_T("Id")))
								{
									sFileID = xml.GetData();
								}
								if (xml.FindElem(_T("CADPath")))
								{
									tempString = xml.GetData();
									if (tempString != "")
									{
										sFileName = GetFileName(tempString);//��ô���չ�����ļ���
									}
								}
								if (xml.FindElem(_T("CADType")))
								{
									sFileType = xml.GetData();
								}
								if (sFileType == "FrontViewFile")
								{
									Attrdoor.m_frontViewFile.fileName = sFileName;
								}
								else if (sFileType == "LeftViewFile")
								{
									Attrdoor.m_leftViewFile.fileName = sFileName;
								}
								else if (sFileType == "TopViewFile")
								{
									Attrdoor.m_topViewFile.fileName = sFileName;
								}
								else if (sFileType == "ExpandViewFile")
								{
									Attrdoor.m_file.fileName = sFileName;
								}
								//����ļ��Ƿ���ڣ�������������
								CString sFilePath = MD2010_GetAppPath() + L"\\support\\Sunac2019\\WebMode\\" + sFileName;
								if (!JHCom_FileExist(sFilePath))
								{
									WEBINST->DownloadFile(_ttoi(sFileID), sFilePath);
								}
							}
							xml.OutOfElem();
						}
					}
				}
				xml.OutOfElem();
			}
			/*if (xml.FindElem(_T("DrawingPathTop")))
			{
				Attrdoor.m_topViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathFront")))
			{
				Attrdoor.m_frontViewFile.fileName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathLeft")))
			{
				Attrdoor.m_leftViewFile.fileName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathExpanded")))
			{
				Attrdoor.m_file.fileName = xml.GetData();
			}*/
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"��")
				{
					Attrdoor.m_isJiTuan = TRUE;
				}
				else Attrdoor.m_isJiTuan = FALSE;
			}
/*TODO
			if (xml.FindElem(_T("Areaid")))
			{
				Attrdoor.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				Attrdoor.m_quyuName = xml.GetData();
			}*/
			//if (xml.FindElem(_T("DrawingType")))
			//{
			//	Attrdoor.m_type = xml.GetData();
			//}
			if (xml.FindElem(_T("DynamicType")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"��")
				{
					Attrdoor.m_isDynamic = TRUE;
				}
				else Attrdoor.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("DoorTypeName")))
			{
				Attrdoor.m_openType = xml.GetData();
			}

			double minValue = 0; 
			double maxValue = 10000;
			if (xml.FindElem(_T("DoorSizeMin")))
			{
				minValue = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowSizeMax")))
			{
				maxValue = _ttof(xml.GetData());
			}

			CWindowsDimData dimDataW;
			dimDataW.sCodeName = L"W";
			if (Attrdoor.m_isDynamic)
			{
				dimDataW.type = SCOPE;
				dimDataW.minValue = minValue;
				dimDataW.maxValue = maxValue;
			}
			//else //TODO ֧�־�̬������
			//{
			//	dimDataW.type = SINGLE;
			//	dimDataW.value = _ttof(xls.GetCellValue(i, 12)); //���
			//}
			Attrdoor.SetDimData(dimDataW);

			CWindowsDimData dimDataH;
			dimDataH.sCodeName = L"H";
			if (Attrdoor.m_isDynamic)
			{
				dimDataH.type = UNLIMIT;
			}
			//else
			//{
			//	dimDataH.type = SINGLE;
			//	dimDataH.value = _ttof(xls.GetCellValue(i, 13)); //���
			//}
			Attrdoor.SetDimData(dimDataH);


			//////////////////////////////////////////////////////////////////////////
			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				while (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					CWindowsDimData tempData;
					if (xml.FindElem(_T("Code")))
					{
						tempData.sCodeName = xml.GetData();
					}
					if (xml.FindElem(_T("ValueType")))
					{
						tempData.type = ToEWindowType(xml.GetData());
					}
					if (xml.FindElem(_T("Value")))
					{
						CString value = xml.GetData();
						std::vector<CString> strs = YT_SplitCString(value, L',');
						for (UINT i = 0; i < strs.size(); i++)
						{
							tempData.valueOptions.push_back(_wtof(strs[i]));
						}
					}
					if (xml.FindElem(_T("MinValue")))
					{
						tempData.minValue = _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						tempData.maxValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						tempData.defaultValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("ValueDescription")))
					{
						tempData.prompt = xml.GetData();
					}
					Attrdoor.SetDimData(tempData);
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
			DoorAttrs.push_back(Attrdoor);
		}
		

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return DoorAttrs;
}



//��Web�˻�ȡ����
std::vector<AttrWindow> CWindowWebData::GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const
{
	CString opNum;
	opNum.Format(_T("%d"),openNum);

	if (openType == "����")
	{
		openType = "";
	}

	if (gongNengQu == "����")
	{
		gongNengQu = "";
	}

	if (openNum == 0)
	{
		opNum = "";
	}

	std::wstring sOpenType = openType;
	std::wstring sOpenNum = opNum.GetString();
	std::wstring sGongnengqu = gongNengQu;
	_ns1__GetWindows ns;
	ns.width = p_width;
	ns.height = p_heigh;
	ns.openType = &sOpenType;
	ns.openNum = &sOpenNum;
	ns.gongNengQu = &sGongnengqu;

	_ns1__GetWindowsResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetWindows(&ns, nsResponse);

	std::vector<AttrWindow> windowAtts;

	//�жϷ��ؽ���Ƿ�ɹ�
	if (nsResponse.GetWindowsResult == NULL)
	{
		return windowAtts;
	}
	
	//AttrWindow attrwindow;

	//�����ַ��������
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetWindowsResult)).c_str());

	windowAtts = ParseWindowsFromXML(xml);

	return windowAtts;

}


//��Web�˻�ȡ���д���
std::vector<AttrWindow>  CWindowWebData::GetAllWindows()const
{
	_ns1__GetAllWindows ns;
	_ns1__GetAllWindowsResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllWindows(&ns, nsResponse);

	std::vector<AttrWindow> allWindowAtts;

	//�жϷ��ؽ���Ƿ�ɹ�
	if (nsResponse.GetAllWindowsResult == NULL)
	{
		return allWindowAtts;
	}

	AttrWindow attrallwindow;

	//�����ַ��������
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllWindowsResult)).c_str());

	allWindowAtts = ParseWindowsFromXML(xml);

	return allWindowAtts;

}

std::vector<AttrWindow >  CWindowWebData::GetAllDoors()const
{
	//std::wstring sDoorType = doorType;

	_ns1__GetAllDoor ns;
/*
	ns.width = p_width;
	ns.height = p_heigh;
	ns.doorType = &sDoorType;*/

	_ns1__GetAllDoorResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllDoor(&ns, nsResponse);

	std::vector<AttrWindow> DoorAttrs;

	//�жϷ��ؽ���Ƿ�ɹ�
	if (nsResponse.GetAllDoorResult == NULL)
	{
		return DoorAttrs;
	}
	
	//�����ַ��������
	CMarkup xml;	
	xml.SetDoc((*(nsResponse.GetAllDoorResult)).c_str());

	DoorAttrs = ParseDoorsFromXML(xml);

	return DoorAttrs;
}
