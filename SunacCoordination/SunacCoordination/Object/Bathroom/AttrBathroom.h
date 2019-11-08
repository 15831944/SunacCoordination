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
#include "../KitchenBathroom/KitchenBathroomProp.h"

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
	virtual ~AttrBathroom();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return Bathroom;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��
	bool IsInstanceEqual(const AttrBathroom& p_att) const;

	CString GetPrototypeCode() const;

	void GetAirVentSize(double& ventX, double& ventY);

public:
	//ԭ��������ԣ���ʼ��ʱ��д
	CString m_sBathroomType;	//����������
	eWindowDoorPos m_windowDoorPos;	//�Ŵ�λ�ù�ϵ
	bool m_hasPaiQiDao;			//�Ƿ���������

	//ԭ�����ݣ���¼ԭ�͵�֧�ֳߴ���Ŵ���ʼλ�ã���ʼ��ʱ��д
	CKitchenBathroomProp m_prop;

	//������ԭ�ͺ���д
	double m_width;				//����
	double m_height;			//����

	//����ʱ��д
	CString m_taipenWidth;		//̨����
	CString m_matongWidth;		//��Ͱ���
	double m_guanXiWidth;		//��ϴ�����

	//������������ã�����ʱ��д
	bool m_isGuoBiao;			//�Ƿ����
	E_BATHROOM_FLOOR_RANGE m_floorRange;	//¥��ѡ��
	double m_airVentW;			//����������
	double m_airVentH;			//���������
	double m_airVentOffsetX;	//������ƫ��x
	double m_airVentOffsetY;	//������ƫ��y

	//����ʱ��д
	int m_angle;				//��ʱ����ת�Ƕ�
	bool m_isMirror;			//����
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
