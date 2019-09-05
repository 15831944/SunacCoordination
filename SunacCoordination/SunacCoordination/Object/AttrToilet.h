/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_TOILET)
#define ARX__ZFFDWGSCALE_H__20140205_113541_TOILET

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
#include "../Sunac_def.h"
#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET "ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET"
#endif

class DLLIMPEXP  AttrToilet : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrToilet);

	// Constructor / Destructor
	AttrToilet();
	virtual ~AttrToilet();

	AttrToilet(const AttrToilet &other);
	AttrToilet & operator=(const AttrToilet &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

public:

	bool m_isGuoBiao;//������
	int m_floorOption;//¥��ѡ��
	double m_paiQiDaoLen;//����������
	double m_paiQiDaoWid;//���������
	bool m_isMirror;//����

	bool m_hasPaiQiDao;//�Ƿ���������
	eWindowDoorPos m_windowDoorPos;//�Ŵ�λ�ù�ϵ

	double m_guanXiWidth;//��ϴ�����
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_TOILET)