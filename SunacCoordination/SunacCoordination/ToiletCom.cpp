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

int TI3_SetMaTongPos(AcDbObjectId toiletId, double yLen)
{
	if (yLen > 2600)
		TYCOM_SetDynamicBlockValue(toiletId, L"��Ͱ��ǽY", 450.0);
	else
		TYCOM_SetDynamicBlockValue(toiletId, L"��Ͱ��ǽY", 400.0);

	return 0;
}

vCString TI3_GetTaiPens(AcDbObjectId toiletId, double yLen, double maTongWidth)
{
	if (maTongWidth <= 0)
		maTongWidth = 750;

	double y1 = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"����������Y�ܳߴ�", y1);

	double y2 = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"������ǽ��", y2);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"װ�޺��", zxt);

	double lastY = yLen - y1 - y2 - zxt * 2 - maTongWidth;


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_̨��_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_̨��_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_̨��_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_̨��_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_̨��_1000");
	
	return taiPens;
}


int TI3_G_SetHuanXiQuY(AcDbObjectId toiletId, double yLen)
{
	if (fabs(yLen - 950) > TOL && fabs(yLen - 1050) > TOL )
		return -1;
	TYCOM_SetDynamicBlockValue(toiletId, L"��ϴ��Y", yLen);
	return 0;
}

//yLen y�����ܳ������߳ߴ磩
int TI4_SetMaTongPos(AcDbObjectId toiletId, double yLen)
{
	if (yLen > 3200)
		TYCOM_SetDynamicBlockValue(toiletId, L"��Ͱ��ǽY", 450.0);
	else
		TYCOM_SetDynamicBlockValue(toiletId, L"��Ͱ��ǽY", 400.0);

	return 0;
}

vCString TI4_GetTaiPens(AcDbObjectId toiletId, double yLen, double maTongWidth)
{
	if (maTongWidth <= 0)
		maTongWidth = 750;

	double y1 = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"����������Y�ܳߴ�", y1);

	double y2 = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"������ǽ��", y2);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"װ�޺��", zxt);

	double lastY = yLen - y1 - y2 - zxt * 2 - maTongWidth - 650;//650Ϊϴ�»����


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_̨��_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_̨��_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_̨��_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_̨��_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_̨��_1000");

	return taiPens;
}

int TI4_G_SetHuanXiQuY(AcDbObjectId toiletId, double yLen)
{
	if (fabs(yLen - 950) > TOL && fabs(yLen - 1050) > TOL )
		return -1;
	TYCOM_SetDynamicBlockValue(toiletId, L"��ϴ��Y", yLen);
	return 0;
}

vCString TL4_GetTaiPens(AcDbObjectId toiletId, double yLen, double linYuFangY)
{
	if (linYuFangY <= 0)
		linYuFangY = 900;

	double zxt = 0;
	TYCOM_GetDynamicBlockData(toiletId, L"װ�޺��", zxt);

	double lastY = yLen - zxt * 2 - 650;//650Ϊϴ�»����


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_̨��_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_̨��_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_̨��_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_̨��_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_̨��_1000");

	return taiPens;
}