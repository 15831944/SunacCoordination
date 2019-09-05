/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN)
#define ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN "ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN"
#endif

class DLLIMPEXP  AttrKitchen : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrKitchen);

	// Constructor / Destructor
	AttrKitchen();
	virtual ~AttrKitchen();

	AttrKitchen(const AttrKitchen &other);
	AttrKitchen & operator=(const AttrKitchen &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��
private:

public:
	bool m_isGuoBiao;//������
	int m_floorOption;//¥��ѡ��
	double m_paiQiDaoLen;//����������
	double m_paiQiDaoWid;//���������
	bool m_isMirror;//����
	bool m_hasPaiQiDao;//�Ƿ���������
	CString m_windowDoorPos;//�Ŵ�λ�ù�ϵ

	//��Щ����������ͼ����--���Դ�ͼ��ֱ��ȡ���� �Ͳ��ô����Լ�¼
	/*CString m_shuiPenType;//ˮ������
	CString m_bingXiangType;//��������
	CString m_zaoTaiWidth;//��̨��� 
	double len;//����
	double kuandu;//���*/
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN)