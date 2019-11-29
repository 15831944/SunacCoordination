#include "StdAfx.h"
#include "BathroomGen.h"
#include "RCBathroom.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_DynamicBlock.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "BathroomAutoName.h"


CBathroomGen::CBathroomGen(AttrBathroom* p_att)
: m_attr(*p_att)
{
}

CBathroomGen::~CBathroomGen()
{
}

void CBathroomGen::SelectTaipen(AcDbObjectId bathroomId, CString taipen)
{
	vCString hideBlockRecordNames;

	if (taipen != L"650")
		hideBlockRecordNames.push_back(L"Sunac_̨��650");
	if (taipen != L"750")
		hideBlockRecordNames.push_back(L"Sunac_̨��750");
	if (taipen != L"800")
		hideBlockRecordNames.push_back(L"Sunac_̨��800");
	if (taipen != L"900")
		hideBlockRecordNames.push_back(L"Sunac_̨��900");
	if (taipen != L"1000")
		hideBlockRecordNames.push_back(L"Sunac_̨��1000");

	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);
}

void CBathroomGen::SelectMatong(AcDbObjectId bathroomId, CString matong)
{
	vCString hideBlockRecordNames;
	if (matong != L"750")
		hideBlockRecordNames.push_back(L"Sunac_��Ͱ750");
	if (matong != L"800")
		hideBlockRecordNames.push_back(L"Sunac_��Ͱ800");

	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);
}

void CBathroomGen::SelectGuanxiWidth(AcDbObjectId bathroomId, double width)
{
	acDocManager->lockDocument(curDoc());

	bool isG = (m_attr.GetFileName().Right(6) == _T("_g.dwg"));
	//ֻ�и�ʪ����������������ϴ��
	if (isG)
		TYCOM_SetDynamicBlockValue(bathroomId, L"��ϴ��Y", width);

	acDocManager->unlockDocument(curDoc());
}

//////////////////////////////////////////////////////////////////////////
vCString CBathroomGen::GetTaipenOptions()
{
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetTaiPenWidths();
}
CString CBathroomGen::GetTaipenDefault()
{ 
	return L"650";
}

vCString CBathroomGen::GetMatongOptions()
{
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetMaTongTypes();
}
CString CBathroomGen::GetMatongDefault()
{
	return _T("750");
}

vCString CBathroomGen::GetGuanxiquOptions()
{
	//ֻ�и�ʪ����������������ϴ��
	if (m_attr.m_prototypeCode.Find(L"_g") == -1)
		return vCString(0);
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetGuanXiWidths();

}
CString CBathroomGen::GetGuanxiquDefault()
{
	return _T("950");
}

void CBathroomGen::InitBathroomByDefault()
{
	vCString taipenOptions = GetTaipenOptions();
	vCString matongOptions = GetMatongOptions();
	CString& taipen = GetBathroomAtt()->m_taipenWidth;
	CString& matong = GetBathroomAtt()->m_matongWidth;
	double& guanxiqu = GetBathroomAtt()->m_guanXiWidth;

	if (TYCOM_vFind(taipen, taipenOptions) == -1)
		taipen = GetTaipenDefault();
	if (TYCOM_vFind(matong, matongOptions) == -1)
		GetBathroomAtt()->m_matongWidth = GetMatongDefault();
	if (m_attr.m_prototypeCode.Find(L"_g") == -1)
		guanxiqu = 0;
	else if (guanxiqu == 0)
		guanxiqu = _ttof(GetGuanxiquDefault());
	GetBathroomAtt()->m_instanceCode = CBathroomAutoName::GetInstance()->GetBathroomName(*GetBathroomAtt());
}

bool CBathroomGen::CheckParameter(CString& errMsg)
{
	//���̨���Ƿ񳬳���ϴ��
	bool isG = (m_attr.GetFileName().Right(6) == _T("_g.dwg"));
	if (!isG)
		return true;
	int taipenWidth = _ttoi(m_attr.m_taipenWidth);
	int guanXiWidth = int(m_attr.m_guanXiWidth + 0.5);
	if (taipenWidth > guanXiWidth)
	{
		errMsg = L"̨���Ȳ��ܳ�����ϴ�����";
		return false;
	}
	return true;
}

AcDbObjectId CBathroomGen::GenBathroom(const AcGePoint3d p_pos, int p_angle)
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

	RCBathroom oneBathroom;

	//�Ȳ��뵽ԭ�㣬��������������ת����
	AcDbObjectId id = oneBathroom.Insert(TY_GetPrototypeFilePath() + m_attr.GetFileName(), p_pos, 0, L"0", 256);
	oneBathroom.InitParameters();
	oneBathroom.SetParameter(L"X�߳�", xLen);
	oneBathroom.SetParameter(L"Y�߳�", yLen);
	//////////////////////////////////////////////////////////////////////////
	//�̵�
	if (m_attr.m_hasPaiQiDao)
	{
		double airVentW, airVentH;
		m_attr.GetAirVentSize(airVentW, airVentH);
		oneBathroom.SetParameter(L"������X�ߴ�", airVentW);
		oneBathroom.SetParameter(L"������Y�ߴ�", airVentH);
		oneBathroom.SetParameter(L"������ƫ��X", m_attr.m_airVentOffsetX);
		oneBathroom.SetParameter(L"������ƫ��Y", m_attr.m_airVentOffsetY);
	}
	//////////////////////////////////////////////////////////////////////////
	oneBathroom.RunParameters();

	SelectTaipen(id, m_attr.m_taipenWidth);
	SelectMatong(id, m_attr.m_matongWidth);
	SelectGuanxiWidth(id, m_attr.m_guanXiWidth);

	SetVantTotalSize(id);
	SetMatongPos(id);
	SetXiyijiPos(id);

	//////////////////////////////////////////////////////////////////////////
	//�Ⱦ�����
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + xLen / 2, 0, 0);
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
	AttrBathroom *pAttribute = new AttrBathroom(m_attr);
	oneBathroom.AddAttribute(pAttribute);
	pAttribute->close();

	return id;
}

double CBathroomGenKI::GetMatongPos()
{
	CString type = m_attr.m_sBathroomType;
	if (type.Left(3) == _T("TI3"))
		return GetMatongPos_I3();
	else
		return GetMatongPos_I3();
}

bool CBathroomGenKI::CheckParameter(CString& errMsg)
{
	double xLen = GetXLength();
	double yLen = GetYLength();

	double ventX, ventY;
	m_attr.GetAirVentSize(ventX, ventY);
	if ((ventX <= 0) || (ventY <= 0))
	{
		errMsg = L"��Ч���������ߴ�";
		return false;
	}
	if (ventX + 1010 > xLen + TOL)
	{
		errMsg = L"�޷�������ԡ�������С������X����ߴ�";
		return false;
	}

	double linyuWidth = max(ventY + 420, 850.0);
	double matongWidth = _ttof(m_attr.m_matongWidth);
	double taipenWidth = _ttof(m_attr.m_taipenWidth);
	double guanxiWidth = m_attr.m_guanXiWidth;

	double minYLen;
	bool isG = (m_attr.m_prototypeCode.Find(L"_g") != -1);
	bool isI4 = (m_attr.m_prototypeCode.Find(L"I4") != -1);
	if (isG)
	{
		if (guanxiWidth + TOL < taipenWidth)
		{
			errMsg = L"̨�費�ܳ�����ϴ��";
			return false;
		}
		minYLen = linyuWidth + matongWidth + guanxiWidth + 150;
	}
	else
		minYLen = linyuWidth + matongWidth + taipenWidth + 50;
	if (isI4)
		minYLen += 650;

	if (minYLen > yLen + TOL)
	{
		errMsg.Format(L"�޷����ý�ߣ����С��߻��������ߴ������\n�������䳤��Ϊ%.0lf����ǰ����Ҫ��С��%.0lf", yLen, minYLen);
		return false;
	}

	return true;
}

void CBathroomGenKI::SetVantTotalSize(AcDbObjectId bathroomId)
{
	double ventX, ventY;
	m_attr.GetAirVentSize(ventX, ventY);
	double width = max(790.0, ventY + 360);

	acDocManager->lockDocument(curDoc());
	TYCOM_SetDynamicBlockValue(bathroomId, L"����������X�ܳߴ�", ventX);
	TYCOM_SetDynamicBlockValue(bathroomId, L"����������Y�ܳߴ�", width);
	acDocManager->unlockDocument(curDoc());
}

void CBathroomGenKI::SetMatongPos(AcDbObjectId bathroomId)
{
	double pos = GetMatongPos();
	acDocManager->lockDocument(curDoc());
	TYCOM_SetDynamicBlockValue(bathroomId, L"��Ͱ��ǽY", pos);
	acDocManager->unlockDocument(curDoc());
}

void CBathroomGenKI::SetXiyijiPos(AcDbObjectId bathroomId)
{
	double taipen = _ttof(m_attr.m_taipenWidth);
	double xiyijiPos = taipen + 350;

	acDocManager->lockDocument(curDoc());
	TYCOM_SetDynamicBlockValue(bathroomId, L"ϴ�»���ǽY", xiyijiPos);
	acDocManager->unlockDocument(curDoc());
}

double CBathroomGenKI::GetMatongPos_I3()
{
	if (m_attr.m_height > 2600 + TOL)
		return 450;
	else
		return 400;
}

double CBathroomGenKI::GetMatongPos_I4()
{
	if (m_attr.m_height > 3200 + TOL)
		return 450;
	else
		return 400;
}

CBathroomGen* CBathroomMrg::CreateBathroomByAttribute(AttrBathroom* p_attr)
{
	if (p_attr->m_prototypeCode.Left(2) == _T("TI"))
		return new CBathroomGenKI(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("TL"))
		return new CBathroomGenKL(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("TU"))
		return new CBathroomGenKU(p_attr);
	else
		return NULL;
}

bool CBathroomGenKL::CheckParameter(CString& errMsg)
{
	double xLen = GetXLength();
	double yLen = GetYLength();

	double ventX, ventY;
	m_attr.GetAirVentSize(ventX, ventY);
	if ((ventX <= 0) || (ventY <= 0))
	{
		errMsg = L"��Ч���������ߴ�";
		return false;
	}

	bool isB = (m_attr.m_prototypeCode.Find(L"_b") != -1);
	bool isL4 = (m_attr.m_prototypeCode.Find(L"L4") != -1);

	double minXLen = ventX + 470 + 900;
	if (isB)
		minXLen += 50; //��׼��ԡ��ʵ�ʿ��Ϊ950

	if (minXLen > xLen + TOL)
	{
		errMsg = L"�޷�������ԡ�������С������X����ߴ������";
		return false;
	}

	double taipenWidth = _ttof(m_attr.m_taipenWidth);

	double minYLen;
	minYLen = 730 + ventY + taipenWidth;
	if (isB)
		minYLen = 1000 + taipenWidth; //��׼��ԡ��ʵ�ʿ��Ϊ950
	if (isL4)
		minYLen += 650;

	if (minYLen > yLen + TOL)
	{
		errMsg.Format(L"�޷����ý�ߣ����С��߻��������ߴ������\n�������䳤��Ϊ%.0lf����ǰ����Ҫ��С��%.0lf", yLen, minYLen);
		return false;
	}

	return true;
}

void CBathroomGenKL::SetVantTotalSize(AcDbObjectId bathroomId)
{
	double xLen = GetXLength();
	double ventX, ventY;
	m_attr.GetAirVentSize(ventX, ventY);

	if (m_attr.m_prototypeCode.Find(L"_b") == -1)
		ventX = xLen - 1010;
	else
		ventX += 360;

	acDocManager->lockDocument(curDoc());
	TYCOM_SetDynamicBlockValue(bathroomId, L"����������X�ܳߴ�", ventX);
	TYCOM_SetDynamicBlockValue(bathroomId, L"����������Y�ܳߴ�", ventY);
	acDocManager->unlockDocument(curDoc());
}

void CBathroomGenKL::SetXiyijiPos(AcDbObjectId bathroomId)
{
	double taipen = _ttof(m_attr.m_taipenWidth);
	double xiyijiPos = taipen + 325;

	acDocManager->lockDocument(curDoc());
	TYCOM_SetDynamicBlockValue(bathroomId, L"ϴ�»���ǽY", xiyijiPos);
	acDocManager->unlockDocument(curDoc());
}
