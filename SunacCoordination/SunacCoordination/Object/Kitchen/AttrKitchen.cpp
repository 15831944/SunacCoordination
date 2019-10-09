/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrKitchen.h"
#include "../../Common/ComFun_Sunac.h"

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrKitchen, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEKITCHEN, ZffCustomObjectDBKITCHEN);
AttrKitchen::AttrKitchen()
{
	m_floorRange = E_FLOOR_1_7;//¥��ѡ��
	 m_airVentW = 0;//����������
	 m_airVentH = 0;//���������
	 m_airVentOffsetX = 0;//����������
	 m_airVentOffsetY = 0;//���������
	 m_isMirror = false;//����
	 m_hasPaiQiDao = true;//�Ƿ���������
	 m_isGuoBiao = true;//������
	//m_kitchenType;//��������
	m_windowDoorPos = _T("�Ŵ���ֱ");//�Ŵ�λ�ù�ϵ

	//��Щ����������ͼ����--���Դ�ͼ��ֱ��ȡ���� �Ͳ��ô����Լ�¼
	//m_shuiPenType;//ˮ������
	//m_bingXiangType;//��������
	//m_zaoTaiType;//��̨��� 
	m_width = 0;//���� ���
	m_height = 0;//��� ����
}

AttrKitchen::AttrKitchen(double p_xLen, double p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, const CProKitchen& p_prototype)
{
	m_type = L"����";
	m_kitchenType = p_prototype.m_sType;
	m_width = p_xLen;
	m_height = p_yLen;
	if (p_doorPos == E_DIR_LEFT || p_doorPos == E_DIR_RIGHT)
		swap(m_width, m_height);

	int pos = m_kitchenType.Find(L'_');
	CString sType = m_kitchenType;
	if (pos != -1)
		sType = m_kitchenType.Left(pos);

	m_prototypeCode.Format(L"%s-%.0lf��%.0lf", sType, m_width, m_height);
	if (m_kitchenType.Find(L"_c") != -1)
		m_prototypeCode += L"/c";

	m_fileName = p_prototype.m_sFileName;
	m_isDynamic = p_prototype.m_bIsDynamic;
	m_isJiTuan = true;
}

AttrKitchen::~AttrKitchen()
{

}

Acad::ErrorStatus AttrKitchen::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	}

	filer->readItem(&m_hasPaiQiDao);
	filer->readItem(&m_isGuoBiao);
	filer->readItem((int*)&m_floorRange);

	filer->readItem(&m_airVentW);
	filer->readItem(&m_airVentH);
	filer->readItem(&m_airVentOffsetX);
	filer->readItem(&m_airVentOffsetY);

	filer->readItem(&m_isMirror);

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];

	filer->readItem(&tempStr);
	m_kitchenType = CString(tempStr);
	filer->readItem(&tempStr);
	m_windowDoorPos = CString(tempStr);
	filer->readItem(&tempStr);
	m_shuiPenType = CString(tempStr);
	filer->readItem(&tempStr);
	m_bingXiangType = CString(tempStr);
	filer->readItem(&tempStr);
	m_zaoTaiType = CString(tempStr);

	filer->readItem(&m_width);
	filer->readItem(&m_height);

	delete [] tempStr;

	return filer->filerStatus();
}

Acad::ErrorStatus AttrKitchen::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->writeItem(m_hasPaiQiDao);
	filer->writeItem(m_isGuoBiao);
	filer->writeItem(m_floorRange);

	filer->writeItem(m_airVentW);
	filer->writeItem(m_airVentH);
	filer->writeItem(m_airVentOffsetX);
	filer->writeItem(m_airVentOffsetY);

	filer->writeItem(m_isMirror);

	filer->writeItem(m_kitchenType);
	filer->writeItem(m_windowDoorPos);
	filer->writeItem(m_shuiPenType);
	filer->writeItem(m_bingXiangType);
	filer->writeItem(m_zaoTaiType);

	filer->writeItem(m_width);
	filer->writeItem(m_height);

	return filer->filerStatus();
}

bool AttrKitchen::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrKitchen * pRealObj = dynamic_cast<AttrKitchen *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return (m_isGuoBiao == pRealObj->m_isGuoBiao &&
		m_floorRange == pRealObj->m_floorRange &&
		m_airVentW == pRealObj->m_airVentW &&
		m_airVentH == pRealObj->m_airVentH &&
		m_airVentOffsetX == pRealObj->m_airVentOffsetX &&
		m_airVentOffsetY == pRealObj->m_airVentOffsetY &&
		m_isMirror == pRealObj->m_isMirror 
		);
}

CProKitchen* AttrKitchen::GetProKitchen()
{
	return CProMrg::GetInstance()->GetProKitchenByFileName(m_fileName);
}

