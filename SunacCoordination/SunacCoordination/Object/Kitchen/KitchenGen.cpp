#include "StdAfx.h"
#include "KitchenGen.h"
#include "RCKitchen.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "..\..\Common/ComFun_DynamicBlock.h"


//const CString c_ShuipenBlockNames[5] = { 
//	L"ˮ��_����_600_420",
//	L"ˮ��_����_800_420",
//	L"ˮ��_˫��_900_450",
//	L"ˮ��_˫��_1000_450",
//	L"ˮ��_˫��_1200_450" };
//
//const CString c_BingxiangBlockNames[] = {
//	L"����_������_700",
//	L"����_˫����_800",
//	L"����_˫����_1000" };
//
//const CString c_ZhaotaiBlockNames[] = {
//	L"��̨_800",
//	L"��̨_1000"};

CKitchGen::CKitchGen(AttrKitchen* p_att)
: m_attr(*p_att)
{
	m_id = AcDbObjectId::kNull;

	m_doorDir = E_DIR_BOTTOM;
	m_windowDir = E_DIR_TOP;
	m_angle = 0;
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

int CKitchGen::SetAirVentSize(double p_width, double p_height )
{
	//TODO �����̵���ȵ���̬��
	return 0;
}
//////////////////////////////////////////////////////////////////////////
vCString CKitchGen::GetShuipenOptions()// ��ȡ̨��ѡ��
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetShuiPenTypes();
	return options;
}
CString CKitchGen::GetShuipenDefault()
{ 
	return L"����600";
}

vCString CKitchGen::GetBinxiangOptions()// ��ȡ����ѡ��
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetBingXiangTypes();
	return options;
}
CString CKitchGen::GetBinxiangDefault()
{
	return _T("�Կ���800");
}

vCString CKitchGen::GetZhaotaiOptions()// ��ȡ��̨ѡ��
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetZaoTaiWidths();
	return options;

}
CString CKitchGen::GetZhaotaiDefault()
{
	return _T("800");
}
double CKitchGen::GetRotateAngle() //������ת�������ϵ
{
	switch (m_doorDir)
	{
	case E_DIR_TOP:
		return PI;
		break;
	case E_DIR_LEFT:
		return -PI/2;
		break;
	case E_DIR_BOTTOM:
		return 0;
		break;
	case E_DIR_RIGHT:
		return PI / 2;
		break;
	default:
		break;
	}
	return 0;
}

void CKitchGen::InitMirror() //��Ҫ����Ŵ���ֱ��������Ŵ���ֱԭ�͵Ĵ����ŵ��Ҳ࣬��ʵ��Ϊ�������Ҫ�Գ�
{
	if (abs(m_windowDir - m_doorDir)%2 == 1) //������Ϊ�Ŵ���ֱ��
	{
		if (m_windowDir > m_doorDir)
		{
			m_attr.m_isMirror = false;
		}
		else
		{
			m_attr.m_isMirror = true;
		}
	}
}
AcDbObjectId CKitchGen::GenKitchen(const AcGePoint3d p_pos)
{
	InitMirror();
	const double angle = GetRotateAngle();

	RCKitchen oneKitchen;

	//�Ȳ��뵽ԭ�㣬��������������ת����
	AcDbObjectId id = oneKitchen.Insert(m_attr.m_filePathName, p_pos, angle, L"0", 256);
	oneKitchen.InitParameters();
	oneKitchen.SetParameter(L"����", m_attr.m_height);
	oneKitchen.SetParameter(L"����", m_attr.m_width);
	oneKitchen.RunParameters();

	SelectShuiPen(id, m_attr.m_shuiPenType);
	SelectBingXiang(id, m_attr.m_bingXiangType);
	SelectZaoTai(id, m_attr.m_zaoTaiType);

	//////////////////////////////////////////////////////////////////////////
	//�̵�
	double airVentW = 0;
	double airVentH = 0;
	if (m_attr.m_hasPaiQiDao)
	{
		if (m_attr.m_isGuoBiao) //����
		{
			airVentW = m_attr.m_airVentOffsetX + c_airVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_airVentSize[m_attr.m_floorRange];
		}
		else
		{
			airVentW = m_attr.m_airVentW;
			airVentH = m_attr.m_airVentH;
		}
		assert(airVentW > 0 && airVentH > 0);
	}
	SetAirVentSize(airVentW, airVentH);

	//////////////////////////////////////////////////////////////////////////
	SetDoorPos(id, m_attr.m_width);
	SetZaoTaiPos(id, m_attr.m_width, m_attr.m_height, m_attr.m_zaoTaiType);
	SetShuiPenPos(id, m_attr.m_width, m_attr.m_height,m_attr.m_shuiPenType);

	//////////////////////////////////////////////////////////////////////////
	//�Ⱦ�����
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + GetXLength() / 2, 0, 0);
		TYCOM_Mirror(oneKitchen.m_id, basePt, AcGeVector3d(0, 1, 0));
	}

	//�ٽǶ���ת
	if (angle!=0)
	{
		TYCOM_Rotate(oneKitchen.m_id, p_pos, angle);
	}

	
	//////////////////////////////////////////////////////////////////////////
	//��UI�����ݼ�¼��ͼ�����չ�ֵ���
	AttrKitchen *pAttribute = new AttrKitchen(m_attr);
	oneKitchen.AddAttribute(pAttribute);
	pAttribute->close();

	m_id = id;
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
		//�����2900ʱ����̨�Ҳ��ǽ��װ������棬 �ж��ֽ���
		//1.��̨�Ҳ�������װ�������1100
		//2.��̨�Ҳ�����Ҳ�װ�������1100
		TYCOM_SetDynamicBlockValue(kitchenId, L"��̨��ǽX", 1100 - ztt);
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

//////////////////////////////////////////////////////////////////////////
CKitchMrg* CKitchMrg::GetInstance()
{
	static CKitchMrg instance;
	return &instance;
}

CKitchMrg::CKitchMrg()
{

}

CKitchMrg::~CKitchMrg()
{

}

//////////////////////////////////////////////////////////////////////////

vector<AttrKitchen*> CKitchMrg::FilterKitch(EKitchType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	//���ȡ����
	int nWidth = int(p_width + 0.5);
	int nHeight = int(p_height + 0.5);

	//����Ϊ�ŵķ�����������ڲ�ߣ���Ҫ�������
	if (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT)
		swap(nWidth, nHeight);

	vector<AttrKitchen*> allKitchOut;
	vector<AttrKitchen*> kitchenStatic = FilterStatic(nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
	allKitchOut.insert(allKitchOut.end(), kitchenStatic.begin(), kitchenStatic.end());

	if (E_KITCH_ALL==p_type)
	{
		vector<AttrKitchen*> kitchenOut1 = FilterKitch_Internal(E_KITCH_U, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		vector<AttrKitchen*> kitchenOut2 = FilterKitch_Internal(E_KITCH_L, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		vector<AttrKitchen*> kitchenOut3 = FilterKitch_Internal(E_KITCH_I, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);

		allKitchOut.insert(allKitchOut.end(), kitchenOut1.begin(), kitchenOut1.end());
		allKitchOut.insert(allKitchOut.end(), kitchenOut2.begin(), kitchenOut2.end());
		allKitchOut.insert(allKitchOut.end(), kitchenOut3.begin(), kitchenOut3.end());
	}
	else
	{
		vector<AttrKitchen*> kitchenOut = FilterKitch_Internal(p_type, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		allKitchOut.insert(allKitchOut.end(), kitchenOut.begin(), kitchenOut.end());
	}

	//��ɸѡ��ԭ�����óߴ�
	for (UINT i = 0; i < allKitchOut.size();i++)
	{
		allKitchOut[i]->m_width = nWidth;
		allKitchOut[i]->m_height = nHeight;		
	}

	return allKitchOut;
}
vector<AttrKitchen*> CKitchMrg::FilterKitch_Internal(EKitchType p_type, int nWidth, int nHeight, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	bool bNotSupport = true;
	bool bDoorWindowOpposite = abs(p_windowDir - p_doorDir) == 2;//�Ŵ��Կ�
	if (E_KITCH_U==p_type)
	{
		if (nWidth > nHeight) //ǳU
		{
			if (bDoorWindowOpposite)
			{
				return FilterKUq(nWidth, nHeight, p_bHasAirVent);
			}
			else
			{
				return FilterKUqc(nWidth, nHeight, p_bHasAirVent);
			}
		}
		else //��U
		{
			if (bDoorWindowOpposite)
			{
				return FilterKUs(nWidth, nHeight, p_bHasAirVent);
			}
			else
			{
				bNotSupport = true;//��֧��
			}
		}
	}
	else if (E_KITCH_L == p_type)
	{
		if (bDoorWindowOpposite)
		{
			return FilterKL(nWidth, nHeight, p_bHasAirVent);
		}
		else
		{
			bNotSupport = true;//��֧��
		}
	}
	else if (E_KITCH_I == p_type)
	{
		if (bDoorWindowOpposite)
		{
			return FilterKI(nWidth, nHeight, p_bHasAirVent);
		}
		else
		{
			bNotSupport = true;//��֧��
		}
	}
	else
	{

	}

	return vector<AttrKitchen*>(); //���ؿ�
}


vector<AttrKitchen*> CKitchMrg::FilterStatic(int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent) //��̬����
{
	bool bDoorWindowOpposite = abs(p_windowDir - p_doorDir) == 2;//�Ŵ��Կ�

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, bDoorWindowOpposite ? _T("�Ŵ��Կ�") : _T("�Ŵ���ֱ"), _T(""), p_bHasAirVent, false); 
}


vector<AttrKitchen*> CKitchMrg::FilterKUq(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//�޶����������ķ�Χ
	if (p_width < 2450 || p_width > 3500)
		return attrKitchen;
	if (p_height < 1700 || p_height > 2600)
		return attrKitchen;
	if (p_width <= p_height)
		return attrKitchen;

	//��֧��2450*1700
	if (p_width == 2450 && p_height == 1700)
		return attrKitchen;

	//��150�����ߴ�
	if ((p_width - 2450) % 150 != 0)
		return attrKitchen;
	if ((p_height - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("�Ŵ��Կ�"), _T("U��"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKUqc(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//�޶����������ķ�Χ
	if (p_width < 2450 || p_width > 3500)
		return attrKitchen;
	if (p_height < 1700 || p_height > 2450)
		return attrKitchen;
	if (p_width <= p_height)
		return attrKitchen;

	//��֧��2450*1700
	if (p_width == 2450 && p_height == 1700)
		return attrKitchen;

	//��150�����ߴ�
	if ((p_width - 2450) % 150 != 0)
		return attrKitchen;
	if ((p_height - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("�Ŵ���ֱ"), _T("U��"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKUs(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//�޶����������ķ�Χ
	if (p_width < 2300 || p_width > 2450)
		return attrKitchen;
	if (p_height < 2450 || p_height > 3200)
		return attrKitchen;
	if (p_width > p_height)
		return attrKitchen;

	//��150�����ߴ�
	if ((p_width - 2300) % 150 != 0)
		return attrKitchen;
	if ((p_height - 2450) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("�Ŵ��Կ�"), _T("U��"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKL(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//�޶����������ķ�Χ
	if (p_width < 1700 || p_width > 2000)
		return attrKitchen;
	if (p_height < 2600 || p_height > 3950)
		return attrKitchen;

	//��150�����ߴ�
	if ((p_width - 1700) % 150 != 0)
		return attrKitchen;
	if ((p_height - 2600) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("�Ŵ��Կ�"), _T("L��"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKI(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//�޶����������ķ�Χ
	if (p_width != 1700)
		return attrKitchen;
	if (p_height < 3050 || p_height > 4100)
		return attrKitchen;

	//��150�����ߴ�
	if ((p_height - 3050) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("�Ŵ��Կ�"), _T("I��"), p_bHasAirVent, true);
}

CKitchGen* CKitchMrg::CreateKitchGenByKitchType(AttrKitchen* p_attKitchen)
{
	if (p_attKitchen==NULL)
	{
		assert(false);
		return NULL;
	}
	
	//ȷ�������ڴ���ǰ�����˳ߴ�
	assert(p_attKitchen->m_width > 0 && p_attKitchen->m_height > 0);


	//����ͨ���Ŵ���Ŵ�����Ӧ����
	CKitchGen* pKitchenGen = NULL;

	if (p_attKitchen->m_isDynamic)
	{
		if (p_attKitchen->m_kitchenType==_T("U��"))
		{
			if (p_attKitchen->m_windowDoorPos==_T("�Ŵ��Կ�"))
			{
				CString sName = p_attKitchen->m_name;
				sName.MakeUpper();
				if (sName.Find(_T("KUS"))>=0)
				{
					pKitchenGen = new CKitchGenKUS(p_attKitchen); //��U
				}
				else
				{
					pKitchenGen = new CKitchGenKUQ(p_attKitchen);
				}
			}
			else if (p_attKitchen->m_windowDoorPos == _T("�Ŵ���ֱ"))
			{
				pKitchenGen = new CKitchGenKUQ_C(p_attKitchen);
			}
			else
			{
				return NULL;
			}
		}
		else if (p_attKitchen->m_kitchenType == _T("L��"))
		{
			pKitchenGen = new CKitchGenKL(p_attKitchen);
		}
		else if (p_attKitchen->m_kitchenType == _T("I��"))
		{
			pKitchenGen = new CKitchGenSTATIC(p_attKitchen);
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		pKitchenGen = new CKitchGenSTATIC(p_attKitchen);
	}


	return pKitchenGen;
}