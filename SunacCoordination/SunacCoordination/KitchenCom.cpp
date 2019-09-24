#include "StdAfx.h"
#include "KitchenCom.h"
#include "Common/ComFun_Sunac.h"
#include "Common/ComFun_DynamicBlock.h"

int Kitchen_SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen)
{
	vCString hideBlockRecordNames;
	if (shuiPen == L"����600")
	{
		hideBlockRecordNames.push_back(L"ˮ��_����_800_420");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_900_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1000_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1200_450");
	}
	else if (shuiPen == L"����700")
	{
		hideBlockRecordNames.push_back(L"ˮ��_����_600_420");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_900_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1000_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1200_450");
	}
	else if (shuiPen == L"˫��900")
	{
		hideBlockRecordNames.push_back(L"ˮ��_����_600_420");
		hideBlockRecordNames.push_back(L"ˮ��_����_800_420");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1000_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1200_450");
	}
	else if (shuiPen == L"˫��1000")
	{
		hideBlockRecordNames.push_back(L"ˮ��_����_600_420");
		hideBlockRecordNames.push_back(L"ˮ��_����_800_420");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_900_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1200_450");
	}
	else if (shuiPen == L"˫��1200")
	{
		hideBlockRecordNames.push_back(L"ˮ��_����_600_420");
		hideBlockRecordNames.push_back(L"ˮ��_����_800_420");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_900_450");
		hideBlockRecordNames.push_back(L"ˮ��_˫��_1000_450");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);
}

int Kitchen_SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai)
{
	vCString hideBlockRecordNames;
	if (zaoTai == L"800")
	{
		hideBlockRecordNames.push_back(L"��̨_900");
	}
	else if (zaoTai == L"900")
	{
		hideBlockRecordNames.push_back(L"��̨_800");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);
}

//kuq �Կ� �Զ������ŵ�λ��
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", 700.0);
	}
	else
	{
		//1600���ſ�
		double value = (kaiJian - 1600)/2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", value);
	}

	return 0;
}

//kuq �Կ� �Զ�������̨��λ��
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType)
{
	if (jinshen < 2000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽY", 200.0);
	}
	else
	{
		double pqdY = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"������Y�ߴ�", pqdY);

		double qt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

		double ztt = 800;
		if (zaoTaiType == L"900")
			ztt = 900;

		double value = (jinshen - zxt * 2 - qt - pqdY - ztt/2)/2;

		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽY", value);
	}

	return 0;
}