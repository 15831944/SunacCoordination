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

AttrKitchen::~AttrKitchen()
{

}

AttrKitchen::AttrKitchen(const AttrKitchen &other) : AttrObject(other)
{
	*this = other;
}

AttrKitchen & AttrKitchen::operator=(const AttrKitchen &rhs)
{
	AttrObject::operator=(rhs);

	m_floorRange = rhs.m_floorRange;//¥��ѡ��
	m_airVentW = rhs.m_airVentW;//����������
	m_airVentH = rhs.m_airVentH;//���������
	m_airVentOffsetX = rhs.m_airVentOffsetX;//����������
	m_airVentOffsetY = rhs.m_airVentOffsetY;//���������
	m_isMirror = rhs.m_isMirror;//����
	m_hasPaiQiDao = rhs.m_hasPaiQiDao;//�Ƿ���������
	m_isGuoBiao = rhs.m_isGuoBiao;//������
	m_kitchenType = rhs.m_kitchenType;//��������
	m_windowDoorPos = rhs.m_windowDoorPos;//�Ŵ�λ�ù�ϵ

	//��Щ����������ͼ����--���Դ�ͼ��ֱ��ȡ���� �Ͳ��ô����Լ�¼
	m_shuiPenType = rhs.m_shuiPenType;//ˮ������
	m_bingXiangType = rhs.m_bingXiangType;//��������
	m_zaoTaiType = rhs.m_zaoTaiType;//��̨��� 
	m_width = rhs.m_width;//���� ���
	m_height = rhs.m_height;//��� ����

	return *this;
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

