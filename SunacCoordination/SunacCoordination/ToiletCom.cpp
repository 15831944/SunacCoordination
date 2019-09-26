#include "StdAfx.h"
#include "ToiletCom.h"
#include "Common/ComFun_Sunac.h"
#include "Common/ComFun_DynamicBlock.h"

int Toilet_SelectTaiPen(AcDbObjectId toiletId, CString taiPen)
{
	vCString hideBlockRecordNames;
	if (taiPen == L"Sunac_̨��_650")
	{
		hideBlockRecordNames.push_back(L"Sunac_̨��_750");
		hideBlockRecordNames.push_back(L"Sunac_̨��_800");
		hideBlockRecordNames.push_back(L"Sunac_̨��_900");
		hideBlockRecordNames.push_back(L"Sunac_̨��_1000");
	}
	else if (taiPen == L"Sunac_̨��_750")
	{
		hideBlockRecordNames.push_back(L"Sunac_̨��_650");
		hideBlockRecordNames.push_back(L"Sunac_̨��_800");
		hideBlockRecordNames.push_back(L"Sunac_̨��_900");
		hideBlockRecordNames.push_back(L"Sunac_̨��_1000");
	}
	else if (taiPen == L"Sunac_̨��_800")
	{
		hideBlockRecordNames.push_back(L"Sunac_̨��_650");
		hideBlockRecordNames.push_back(L"Sunac_̨��_750");
		hideBlockRecordNames.push_back(L"Sunac_̨��_900");
		hideBlockRecordNames.push_back(L"Sunac_̨��_1000");
	}
	else if (taiPen == L"Sunac_̨��_900")
	{
		hideBlockRecordNames.push_back(L"Sunac_̨��_650");
		hideBlockRecordNames.push_back(L"Sunac_̨��_750");
		hideBlockRecordNames.push_back(L"Sunac_̨��_800");
		hideBlockRecordNames.push_back(L"Sunac_̨��_1000");
	}
	else if (taiPen == L"Sunac_̨��_1000")
	{
		hideBlockRecordNames.push_back(L"Sunac_̨��_650");
		hideBlockRecordNames.push_back(L"Sunac_̨��_750");
		hideBlockRecordNames.push_back(L"Sunac_̨��_800");
		hideBlockRecordNames.push_back(L"Sunac_̨��_900");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(toiletId, hideBlockRecordNames);

	return 0;
}

int Toilet_SelectBingXiang(AcDbObjectId toiletId, CString maTong)
{
	vCString hideBlockRecordNames;
	if (maTong == L"Sunac_��Ͱ800")
	{
		hideBlockRecordNames.push_back(L"Sunac_��Ͱ750");
	}
	else if (maTong == L"Sunac_��Ͱ750")
	{
		hideBlockRecordNames.push_back(L"Sunac_��Ͱ800");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(toiletId, hideBlockRecordNames);

	return 0;
}

