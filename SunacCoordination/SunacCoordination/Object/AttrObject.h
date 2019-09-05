/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT "ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT"
#endif

class DLLIMPEXP  AttrObject : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrObject);

	// Constructor / Destructor
	AttrObject();
	AttrObject(const AttrObject &other);
	AttrObject & operator=(const AttrObject &rhs);

	virtual ~AttrObject();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��
private:

public:
	Adesk::Int32 m_version;//�ļ��汾 ��ʱ�洢

	CString m_id;//ԭ�ͱ��
	CString m_name;//ԭ������
	bool m_isJiTuan;//�Ƿ���
	CString m_quyuName;//��������
	CString m_type;//ԭ������
	bool m_isDynamic;//�Ƿ�̬��
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)