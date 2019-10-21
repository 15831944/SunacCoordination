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
	CString GetFileName()const{ return m_file.fileName; }
	void SetFileName(CString p_str){ m_file.fileName = p_str; }

	void SetInstanceCode(CString  bianHao) { m_instanceCode = bianHao; }
	CString GetInstanceCode()const { return m_instanceCode; }

	
public:
	Adesk::Int32 m_version;		//�ļ��汾 ���л�ʱʹ��

	//��ԭ�ͳ�ʼ��ʱ��д
	CString m_prototypeCode;	//ԭ�ͱ�� //����ԭ��������ԭ�ͱ��

	CDwgFileInfo m_file;	//ԭ���ļ���,����.dwg����������·��
	//CString m_fileName;			
	//CString m_type;			//ԭ������ //��Ϊͨ��GetType��������
	bool m_isDynamic;			//�Ƿ�̬��
	bool m_isJiTuan;			//�Ƿ���

	//��Web��ȡ���ݺ���д
	CString m_quyuId;			//����ID
	CString m_quyuName;			//��������

	//////////////////////////////////////////////////////////////////////////
	//����ʵ��������
	CString m_instanceCode;		//ʵ�����
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)