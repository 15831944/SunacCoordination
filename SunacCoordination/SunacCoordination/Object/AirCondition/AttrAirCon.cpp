/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrAirCon.h"

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrAirCon, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEAIRCON, ZffCustomObjectDBAIRCON);

AttrAirCon::AttrAirCon()
{

}

AttrAirCon::~AttrAirCon()
{

}

AttrAirCon::AttrAirCon(const AttrAirCon &other) : AttrObject(other)
{
	//push_back����
	m_airConId = other.m_airConId;						//�յ����
	m_prototypeCode = other.m_prototypeCode;	//�յ�ԭ�ͱ��
//	m_name = other.m_name;//�յ�ԭ���ļ�
	m_airConHorseNumber = other.m_airConHorseNumber;	//�յ�ƥ��
	m_airConPipePos = other.m_airConPipePos;			//�յ�����ˮ��λ��
	m_airConRainRiser = other.m_airConRainRiser;		//�յ��Ƿ�����ˮ���ܴ���
	m_airConRainRiserPos = other.m_airConRainRiserPos;	//�յ���ˮ��λ��
	m_airConInstallNetSize = other.m_airConInstallNetSize;//�յ���װ���ߴ�


}

AttrAirCon & AttrAirCon::operator=(const AttrAirCon &rhs)
{
	AttrObject::operator=(rhs);
	return *this;
}

Acad::ErrorStatus AttrAirCon::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}
	return Acad::eOk;
}

Acad::ErrorStatus AttrAirCon::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

bool AttrAirCon::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrAirCon * pRealObj = dynamic_cast<AttrAirCon *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}

