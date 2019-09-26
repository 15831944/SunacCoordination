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
	else if (shuiPen == L"����800")
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

	return 0;
}

int Kitchen_SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang)
{
	vCString hideBlockRecordNames;
	if (bingXiang == L"������700")
	{
		//hideBlockRecordNames.push_back(L"��̨_900");
		//hideBlockRecordNames.push_back(L"��̨_900");
	}
	else if (bingXiang == L"�Կ���800")
	{
		//hideBlockRecordNames.push_back(L"��̨_800");
		//hideBlockRecordNames.push_back(L"��̨_900");
	}
	else if (bingXiang == L"�Կ���1000")
	{
		//hideBlockRecordNames.push_back(L"��̨_800");
		//hideBlockRecordNames.push_back(L"��̨_900");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
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

	return 0;
}

//kuq �Կ� �Զ������ŵ�λ��
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

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

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq �Կ� �Զ�������̨��λ��
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

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

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq �Կ� �Զ������ŵ�λ��
int Kitchen_KUq_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����X�ߴ�", lgx);

	double value  = (kaiJian - 350)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", value-qt-zxt-lgx);

	acDocManager->unlockDocument(curDoc());

	return 0;
}



//kuq ��ֱ�� �Զ������ŵ�λ��
int Kitchen_KUq_ChuiZhiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

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

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq ��ֱ�� �Զ�������̨��λ��
int Kitchen_KUq_ChuiZhiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 2900)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", 750.0);
	}
	else
	{
		double pqdX = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"������X�ߴ�", pqdX);

		double qt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

		double value  = (kaiJian - 350)/2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", value-qt-zxt-pqdX);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq ��ֱ�� �Զ�����ˮ���λ��
int Kitchen_KUq_ChuiZhiKai_SetShuiPenPos(AcDbObjectId kitchenId, double jinshen)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����Y�ߴ�", lgy);


	double value  = (jinshen - qt - zxt*2 - lgy)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽY", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}