#include <afxwin.h>

// ATL includes for sqareCmd.h
#include <atlbase.h>
#include <atlcom.h>
//CComModule _Module;	//COM������ģ�飬����һ���ͻ��˷��ʵ�ǰģ������

// Acad COM and Automation interfaces
#include <acadi.h>
#include <axtempl.h>
#include <axpnt3d.h>


#include <acdocman.h>
#include <aced.h>
#include <acdb.h>
#include <adslib.h>
#include <rxmfcapi.h>

//////////////////////////////////////////////////////////////////////////
#include <opmext.h>


//#include "Tangent\TangentCom\tch10_com19.tlh"
#include "TangentCom\tch10_com19_opening.tlh"

//////////////////////////////////////////////////////////////////////////
#include "TangentOpen.h"

//////////////////////////////////////////////////////////////////////////

HRESULT CTangentOpen::SetTangentOpenProp(AcDbObjectId p_winId, CTOpenData p_winData)
{
	HRESULT hr = S_OK;

	if (p_winId==AcDbObjectId::kNull)
	{
		return E_FAIL;
	}

	AcDbEntity *pent;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pent, p_winId, AcDb::kForRead);
	if (es!=Acad::eOk)
	{
		return E_FAIL;
	}

	CLSID entClsid = {0};
	es = pent->getClassID(&entClsid);	//�ɸ���entClsid������ʹ�ú��ְ汾�����������ʼ�������ص���ComOpening�ӿ�

	pent->close();

	if ( IsEqualCLSID(entClsid, __uuidof(TCH10_COM9_T20V5X64::ComOpening)) )
	{
		return SetTangentOpenProp_TCH10_COM9_T20V5X64(p_winId, p_winData);
	}
	//else if (IsEqualCLSID(entClsid, __uuidof(TCH10_COM9_T20V4X64::ComOpening)))	//TODO ��������������汾��
	//{
	//}
	//else
	//{
	//	hr = E_FAIL;
	//}

	return hr;
}


HRESULT CTangentOpen::SetTangentOpenProp_TCH10_COM9_T20V5X64( AcDbObjectId p_winId, CTOpenData p_winData)
{
	HRESULT hr = S_OK;

	CComPtr<TCH10_COM9_T20V5X64::IComOpening> pWin;
	CComQIPtr<IAcadBaseObject> pSquareBase;
	//CComQIPtr<IAcadBaseObject2> pSquareBase2;

	try
	{
		if (FAILED(hr = pWin.CoCreateInstance(__uuidof(TCH10_COM9_T20V5X64::ComOpening))))
			throw hr;

		pSquareBase = pWin;
		//pSquareBase2 = pWin;

		if (pSquareBase==NULL)
			throw E_POINTER;

		pSquareBase->SetObjectId(p_winId);

		pWin->PutWidth(p_winData.width);
		pWin->PutHeight(p_winData.height);
	}
	catch (HRESULT)
	{

	}


	return hr;

}

//  CLSIDFromProgID()��CLSIDFromProgIDEx()	�� ProgID �õ� CLSID��ûʲô��˵�ģ����Լ�������д����ע���
//	ProgIDFromCLSID()	�� CLSID �õ� ProgID��������ʹ����ɺ�Ҫ�ͷ� ProgID ���ڴ�(ע5)
//	CoCreateGuid()	�������һ�� GUID
//	IsEqualGUID()��IsEqualCLSID()��IsEqualIID()	�Ƚ�2��ID�Ƿ����
//	StringFromCLSID()��StringFromGUID2()��StringFromIID()	�� CLSID,IID �õ�ע�����CLSID��ʽ���ַ�����ע���ͷ��ڴ�

EXTERN_C const CLSID IID_IOPMPropertyExpander;
//extern const IID IID_IOPMPropertyExpander;

//MIDL_DEFINE_GUID(CLSID, IID_IOPMPropertyExpander_TY,0x5d535710, 0xd49a, 0x11d1, 0xb6, 0x16, 0x0, 0x60, 0xb0, 0x87, 0xe2, 0x35);

//DEFINE_GUID(IID_IOPMPropertyExpander_TY, 
//	0x5d535710, 0xd49a, 0x11d1, 0xb6, 0x16, 0x0, 0x60, 0xb0, 0x87, 0xe2, 0x35);

//void GuidTest()
//{
//	//CLSID clsid;  
//	//IID iid;  
//	//HRESULT hr = CLSIDFromString(OLESTR("{92e83a37-399a-47b1-bf6c-0ab052650cfe}"),&clsid); 
//
//}