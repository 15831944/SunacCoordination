/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../Sunac_def.h"
#include "PrototypeInfo.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT "ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT"
#endif

//��Ҫ�洢�Ķ�Ҫ�����ϵ����
class DLLIMPEXP  AttrObject : public AcDbObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrObject);

	AttrObject();
	virtual ~AttrObject();

	AttrObject(const AttrObject &other);
	virtual AttrObject & operator=(const AttrObject &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	
	virtual eRCType GetType(){return TYPENUM;}
	virtual bool isEqualTo(AttrObject*other);//��������һ��
	
	//�õ��ӷ��������ص����ص�һ���ļ�����·����
	//�����Ǳ��ر���ʹ��ڵ�һ���ļ�·��
	//�ɹ�����0 ���򷵻�-1
	int GetFile(CString &filePathName);

	void SetInstanceCode(CString  bianHao) { m_instanceCode = bianHao; }
	CString& GetInstanceCode() { return m_instanceCode; }

	virtual CProBase* GetPrototype();
	
public:
	Adesk::Int32 m_version;//�ļ��汾 ��ʱ�洢

	CString m_prototypeCode;//ԭ�ͱ��
	//CString m_name;			//ԭ�����ƣ� YUAN 1006ע�ͣ�����ԭ��������ԭ�ͱ��
	CString m_type;		//ԭ������
	bool m_isDynamic;	//�Ƿ�̬��

	bool m_isJiTuan;//�Ƿ���
	CString m_quyuId;//����ID
	CString m_quyuName;//��������

	CString m_fileName;//ԭ���ļ���,����.dwg����������·��
	
	CString m_instanceCode;//ʵ�����
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)