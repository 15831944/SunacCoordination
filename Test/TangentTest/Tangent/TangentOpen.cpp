

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
#include <dbapserv.h>


//#include "Tangent\TangentCom\tch10_com19.tlh"
#include "TangentCom\tch10_com19_opening.tlh"

//////////////////////////////////////////////////////////////////////////
#include "TangentOpen.h"

//////////////////////////////////////////////////////////////////////////

AcDbObjectId AppendEntity(AcDbEntity *pEnt, const WCHAR * entry)
{
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBt;
	pDb->getBlockTable(pBt, AcDb::kForRead);
	AcDbBlockTableRecord *pBtr;
	pBt->getAt(entry, pBtr, AcDb::kForWrite);
	AcDbObjectId entId;
	pBtr->appendAcDbEntity(entId, pEnt);
	pBtr->close();
	pBt->close();
	pEnt->close();
	return entId;
}

std::vector<AcDbObjectId> YT_Explode(AcDbObjectId entId, const WCHAR * entry)
{
	// Add your code for command ahlzlARX._test here
	std::vector<AcDbObjectId> ids;
	AcDbEntity *pEnt = NULL;
	acdbOpenObject(pEnt, entId, AcDb::kForWrite);
	if(pEnt == 0)
		return ids;

	AcDbVoidPtrArray pExps;
	if (pEnt->explode(pExps) == Acad::eOk)

	{
		for (int i = 0; i < pExps.length(); i++)
		{
			AcDbEntity *pExpEnt = (AcDbEntity*)pExps[i];
			ids.push_back(AppendEntity(pExpEnt,entry));
		}
		pEnt->erase(true);
	}
	else
	{
		acutPrintf(_T("\n�ö����ܱ��ֽ⣡"));
	}
	pEnt->close();

	return ids;
}

HRESULT CTangentOpen::InsertWinOpenning(AcGePoint3d p_centerPt, CTOpenData p_winData)
{
	AcDbObjectId blockid = AcDbObjectId::kNull;
	CString sPath = _T("D:\\test\\Sunac_test\\T�Ŷ�.dwg");
	CString sBlockDefName = _T("T�Ŷ�");
	//int nRet = MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, sPath, sBlockDefName, blockid, p_centerPt, 0, AcGeScale3d(1, 1, 1));
	if (blockid==AcDbObjectId::kNull)
	{
		return E_FAIL;
	}

	YT_Explode(blockid, ACDB_MODEL_SPACE);

	//////////////////////////////////////////////////////////////////////////
	SetTangentOpenProp(blockid, p_winData);
}

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
	else
	{
		hr = E_FAIL;
	}

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
	catch (HRESULT eHr)
	{
		acutPrintf(_T("\n Error SetTangentOpenProp_TCH10_COM9_T20V5X64."));
		return eHr;
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