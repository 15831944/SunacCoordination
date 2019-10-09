/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
#define ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../AttrObject.h"
#include "../../Sunac_def.h"
#include "../PrototypeCommonDef.h"
#include "../PrototypeInfo.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom "ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom"
#endif

class DLLIMPEXP  AttrBathroom : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrBathroom);

	// Constructor / Destructor
	AttrBathroom();
	AttrBathroom(double p_xLen, double p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, const CProBathroom& p_prototype);
	virtual ~AttrBathroom();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return Bathroom;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

	CProBathroom* GetProBathroom();

public:
	bool m_hasPaiQiDao; //�Ƿ���������
	bool m_isGuoBiao; //������
	E_FLOOR_RANGE m_floorRange; //¥��ѡ��
	double m_airVentW; //����������
	double m_airVentH; //���������
	double m_airVentOffsetX; //������ƫ��x
	double m_airVentOffsetY; //������ƫ��y

	bool m_isMirror; //����

	CString m_sBathroomType; //����������
	eWindowDoorPos m_windowDoorPos; //�Ŵ�λ�ù�ϵ

	CString m_taipenWidth;//̨����
	CString m_matongWidth;//��Ͱ���
	double m_guanXiWidth; //��ϴ�����
	double m_width;//����
	double m_height;//����
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
