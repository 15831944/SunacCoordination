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
	if (GetBathroomAtt()->m_taipenWidth.IsEmpty())
		GetBathroomAtt()->m_taipenWidth = GetTaipenDefault();
	if (GetBathroomAtt()->m_matongWidth.IsEmpty())
		GetBathroomAtt()->m_matongWidth = GetMatongDefault();
	if (m_attr.m_prototypeCode.Find(L"_g") == -1)
		GetBathroomAtt()->m_guanXiWidth = 0;
	else if (GetBathroomAtt()->m_guanXiWidth == 0)
		GetBathroomAtt()->m_guanXiWidth = _ttof(GetGuanxiquDefault());
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
		double airVentW = 0;
		double airVentH = 0;
		if (m_attr.m_isGuoBiao) //����
		{
			airVentW = m_attr.m_airVentOffsetX + c_bathroomAirVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_bathroomAirVentSize[m_attr.m_floorRange];
		}
		else
		{
			airVentW = m_attr.m_airVentW;
			airVentH = m_attr.m_airVentH;
		}
		assert(airVentW > 0 && airVentH > 0);
		oneBathroom.SetParameter(L"������X�ߴ�", airVentW);
		oneBathroom.SetParameter(L"������Y�ߴ�", airVentH);
	}
	//////////////////////////////////////////////////////////////////////////
	oneBathroom.RunParameters();

	SelectTaipen(id, m_attr.m_taipenWidth);
	SelectMatong(id, m_attr.m_matongWidth);
	SelectGuanxiWidth(id, m_attr.m_guanXiWidth);

	SetMatongPos(id, yLen);

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

int CBathroomGenKI::SetMatongPos(AcDbObjectId bathroomId, double yLen)
{
	CString type = m_attr.m_sBathroomType;
	if (type.Left(3) == _T("TI3"))
		return SetMatongPos_I3(bathroomId, yLen);
	else
		return SetMatongPos_I4(bathroomId, yLen);
}

int CBathroomGenKI::SetMatongPos_I3(AcDbObjectId bathroomId, double yLen)
{
	acDocManager->lockDocument(curDoc());

	if (yLen > 2600)
		TYCOM_SetDynamicBlockValue(bathroomId, L"��Ͱ��ǽY", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"��Ͱ��ǽY", 400.0);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

int CBathroomGenKI::SetMatongPos_I4(AcDbObjectId bathroomId, double yLen)
{
	acDocManager->lockDocument(curDoc());

	if (yLen > 3200)
		TYCOM_SetDynamicBlockValue(bathroomId, L"��Ͱ��ǽY", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"��Ͱ��ǽY", 400.0);

	acDocManager->unlockDocument(curDoc());

	return 0;
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
