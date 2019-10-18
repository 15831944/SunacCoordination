#include "StdAfx.h"
#include "KitchenGen.h"
#include "RCKitchen.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "..\..\Common/ComFun_DynamicBlock.h"


CKitchGen::CKitchGen(AttrKitchen* p_att)
: m_attr(*p_att)
{
}

CKitchGen::~CKitchGen()
{
}

int CKitchGen::SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen)
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

int CKitchGen::SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai)
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

int CKitchGen::SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang)
{
	vCString hideBlockRecordNames;
	if (bingXiang == L"������700")
	{
		hideBlockRecordNames.push_back(L"����_�Կ���_800");
		hideBlockRecordNames.push_back(L"����_�Կ���_1000");
	}
	else if (bingXiang == L"�Կ���800")
	{
		hideBlockRecordNames.push_back(L"����_������_700");
		hideBlockRecordNames.push_back(L"����_�Կ���_1000");
	}
	else if (bingXiang == L"�Կ���1000")
	{
		hideBlockRecordNames.push_back(L"����_������_700");
		hideBlockRecordNames.push_back(L"����_�Կ���_800");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
}

double CKitchGen::GetXLength()
{
	CKitchenBathroomProp* pProKitchen = &m_attr.m_prop;
	if (pProKitchen->m_doorPos == E_DIR_BOTTOM || pProKitchen->m_doorPos == E_DIR_TOP)
		return m_attr.m_width;
	else
		return m_attr.m_height;
}

double CKitchGen::GetYLength()
{
	if (m_attr.m_width == GetXLength())
		return m_attr.m_height;
	else
		return m_attr.m_width;
}

//////////////////////////////////////////////////////////////////////////
vCString CKitchGen::GetShuipenOptions()// ��ȡ̨��ѡ��
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetShuiPenTypes();
	return options;
}

CString CKitchGen::GetShuipenDefault()
{ 
	return L"����600";
}

vCString CKitchGen::GetBinxiangOptions()// ��ȡ����ѡ��
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetBingXiangTypes();
	return options;
}

CString CKitchGen::GetBinxiangDefault()
{
	return _T("�Կ���800");
}

vCString CKitchGen::GetZhaotaiOptions()// ��ȡ��̨ѡ��
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetZaoTaiWidths();
	return options;

}

CString CKitchGen::GetZhaotaiDefault()
{
	return _T("800");
}

AcDbObjectId CKitchGen::GenKitchen(const AcGePoint3d p_pos, int p_angle)
{
	AcGeVector3d offsetXY;
	const double xLen = GetXLength();
	const double yLen = GetYLength();
	switch (p_angle)
	{
	case 90:
		offsetXY = AcGeVector3d(yLen, 0, 0);
		break;
	case 180:
		offsetXY = AcGeVector3d(xLen, yLen, 0);
		break;
	case 270:
		offsetXY = AcGeVector3d(0, xLen, 0);
		break;
	default:
		offsetXY = AcGeVector3d(0, 0, 0);
	}

	RCKitchen oneKitchen;

	//�Ȳ��뵽ԭ�㣬��������������ת����
	AcDbObjectId id = oneKitchen.Insert(TY_GetLocalFilePath() + m_attr.GetFileName(), p_pos, 0, L"0", 256);
	oneKitchen.InitParameters();
	oneKitchen.SetParameter(L"����", m_attr.m_width);
	oneKitchen.SetParameter(L"����", m_attr.m_height);
	//////////////////////////////////////////////////////////////////////////
	//�̵�
	if (m_attr.m_hasPaiQiDao)
	{
		double airVentW = 0;
		double airVentH = 0;
		if (m_attr.m_isGuoBiao) //����
		{
			airVentW = m_attr.m_airVentOffsetX + c_kitchenAirVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_kitchenAirVentSize[m_attr.m_floorRange];
		}
		else
		{
			airVentW = m_attr.m_airVentW;
			airVentH = m_attr.m_airVentH;
		}
		assert(airVentW > 0 && airVentH > 0);
		oneKitchen.SetParameter(L"������X�ߴ�", airVentW);
		oneKitchen.SetParameter(L"������Y�ߴ�", airVentH);
	}
	//////////////////////////////////////////////////////////////////////////
	oneKitchen.RunParameters();

	SelectShuiPen(id, m_attr.m_shuiPenType);
	SelectBingXiang(id, m_attr.m_bingXiangType);
	SelectZaoTai(id, m_attr.m_zaoTaiType);

	SetDoorPos(id, m_attr.m_width);
	SetZaoTaiPos(id, m_attr.m_width, m_attr.m_height, m_attr.m_zaoTaiType);
	SetShuiPenPos(id, m_attr.m_width, m_attr.m_height,m_attr.m_shuiPenType);

	//////////////////////////////////////////////////////////////////////////
	//�Ⱦ�����
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + GetXLength() / 2, 0, 0);
		TYCOM_Mirror(id, basePt, AcGeVector3d(0, 1, 0));
	}

	//�ٽǶ���ת
	if (p_angle!=0)
	{
		TYCOM_Rotate(id, p_pos, p_angle * PI / 180);

		//��ת����㲻�������½ǣ���Ҫƽ��
		TYCOM_Move(id, offsetXY);
	}

	
	//////////////////////////////////////////////////////////////////////////
	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrKitchen *pAttribute = new AttrKitchen(m_attr);
	oneKitchen.AddAttribute(pAttribute);
	pAttribute->close();

	return id;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenKUQ::CKitchGenKUQ(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKUQ::GetShuipenOptions()// ��ȡ̨��ѡ��
{
	vCString options;
	if (m_attr.m_width < 2900)
	{
		options.push_back(_T("����600"));
		options.push_back(_T("����800"));
	}
	else
	{
		options.push_back(_T("˫��900"));
		options.push_back(_T("˫��1000"));
		options.push_back(_T("˫��1200"));
	}
	return options;
}

CString CKitchGenKUQ::GetShuipenDefault()
{
	if (m_attr.m_width < 2900)
		return L"����600";
	else
		return L"˫��900";
}

//kuq �Կ� �Զ������ŵ�λ��
int CKitchGenKUQ::SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", 700.0);
	}
	else
	{
		//1600���ſ�
		double value = (kaiJian - 1600) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq �Կ� �Զ�������̨��λ��
int CKitchGenKUQ::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (jinShen < 2000)
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

		double value = (jinShen - zxt * 2 - qt - pqdY - ztt / 2) / 2;

		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽY", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq �Կ� �Զ������ŵ�λ��
int CKitchGenKUQ::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����X�ߴ�", lgx);

	double value = (kaiJian - 350) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", value - qt - zxt - lgx);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKUQ_C::CKitchGenKUQ_C(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKUQ_C::GetShuipenOptions()// ��ȡ̨��ѡ��
{
	vCString options;
	if (m_attr.m_height < 2150) //����С��2150ʱ����
	{
		options.push_back(_T("����600"));
		options.push_back(_T("����800"));
	}
	else
	{
		options.push_back(_T("˫��900"));
		options.push_back(_T("˫��1000"));
		options.push_back(_T("˫��1200"));
	}
	return options;
}

CString CKitchGenKUQ_C::GetShuipenDefault()
{
	if (m_attr.m_height < 2150) //����С��2150ʱ����
		return L"����600";
	else
		return L"˫��900";
}

//kuq ��ֱ�� �Զ������ŵ�λ��
int CKitchGenKUQ_C::SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", 700.0);
	}
	else
	{
		//1600���ſ�
		double value = (kaiJian - 1600) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"�Ŷ��ҳ�", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq ��ֱ�� �Զ�������̨��λ��
int CKitchGenKUQ_C::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
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

		double value = (kaiJian - 350) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", value - qt - zxt - pqdX);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq ��ֱ�� �Զ�����ˮ���λ��
int CKitchGenKUQ_C::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����Y�ߴ�", lgy);


	double value = (jinShen - qt - zxt * 2 - lgy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽY", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKUS::CKitchGenKUS(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

//kus ��ֱ�� �Զ�����ˮ���λ��
int CKitchGenKUS::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����X�ߴ�", lgx);

	double pqdx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"������X�ߴ�", pqdx);


	double value = (kaiJian - qt * 2 - zxt * 2 - lgx - pqdx) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

int CKitchGenKUS::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double pqdy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"������Y�ߴ�", pqdy);


	double value = (jinShen - qt - zxt * 2 - pqdy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽY", value);

	acDocManager->unlockDocument(curDoc());
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKL::CKitchGenKL(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKL::GetBinxiangOptions()// ��ȡ����ѡ��
{
	vCString options;
	if (m_attr.m_height < 3200) //����С��3200ʱ������
	{
		options.push_back(_T("������700"));
	}
	else
	{
		options.push_back(_T("�Կ���800"));
		options.push_back(_T("�Կ���1000"));
	}
	return options;
}

CString CKitchGenKL::GetBinxiangDefault()
{
	if (m_attr.m_height < 3200) //����С��3200ʱ������
	{
		return _T("������700");
	}
	else
	{
		return _T("�Կ���800");
	}
}

//KL�Ŵ��Կ�����ˮ���λ��
int CKitchGenKL::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"ǽ��", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"����Y�ߴ�", lgy);

	double pqdy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"������Y�ߴ�", pqdy);


	double value = (kaiJian - qt * 2 - zxt * 2 - lgy - pqdy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽY", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//KL�Ŵ��Կ�������̨��λ�� 
int CKitchGenKL::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	double ztt = 800;
	if (zaoTaiType == L"900")
		ztt = 900;

	if (jinShen < 2900)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", 900.0);
	}
	else
	{
		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"װ�޺��", zxt);


		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", jinShen - 2 * zxt - 1100 - ztt);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenKI::CKitchGenKI(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKI::GetShuipenOptions()// ��ȡ̨��ѡ��
{
	vCString options;
	if (m_attr.m_height < 3350) //����С��3350ʱ����
	{
		options.push_back(_T("����600"));
		options.push_back(_T("����800"));
	}
	else
	{
		options.push_back(_T("˫��900"));
		options.push_back(_T("˫��1000"));
		options.push_back(_T("˫��1200"));
	}
	return options;
}

CString CKitchGenKI::GetShuipenDefault()
{
	if (m_attr.m_height < 3350) //����С��3350ʱ����
		return L"����600";
	else
		return L"˫��900";
}

vCString CKitchGenKI::GetZhaotaiOptions() 
{
	vCString options;
	if (m_attr.m_height < 3200) //����С��3200ʱ������
	{
		options.push_back(_T("������700"));
	}
	else
	{
		options.push_back(_T("�Կ���800"));
		options.push_back(_T("�Կ���1000"));
	}
	return options;
}

CString CKitchGenKI::GetZhaotaiDefault()
{
	if (m_attr.m_height < 3350) //����С��3350ʱ
	{
		return _T("800");
	}
	else
	{
		return _T("900");
	}
}

//KI�Ŵ��Կ�����ˮ���λ��
//shuiPen:"����600"/"����800"/˫��900/˫��1000/˫��1200
int CKitchGenKI::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	if (shuiPenType == L"����600")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", 690.0);
	}
	else if (shuiPenType == L"����800")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", 840.0);
	}
	else if (shuiPenType == L"˫��900")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", 840.0);
	}
	else if (shuiPenType == L"˫��100")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", 940.0);
	}
	else if (shuiPenType == L"˫��1200")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"ˮ���ǽX", 1040.0);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//zaoTaiType "800"/"900"
int CKitchGenKI::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (zaoTaiType == L"800")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", 600.0);
	}

	if (zaoTaiType == L"900")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", 650.0);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenSTATIC::CKitchGenSTATIC(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

CKitchGen* CKitchMrg::CreateKitchGenByKitchType(AttrKitchen* p_attr)
{
	if (p_attr->m_prototypeCode.Left(3) == _T("KUq") && p_attr->m_prototypeCode.Find(L"_c") == -1)
		return new CKitchGenKUQ(p_attr);
	else if (p_attr->m_prototypeCode.Left(3) == _T("KUq"))
		return new CKitchGenKUQ_C(p_attr);
	else if (p_attr->m_prototypeCode.Left(3) == _T("KUs"))
		return new CKitchGenKUS(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("KL"))
		return new CKitchGenKL(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("KI"))
		return new CKitchGenKI(p_attr);
	else
		return NULL;
}
