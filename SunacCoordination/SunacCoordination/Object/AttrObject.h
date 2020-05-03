/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../Sunac_def.h"
#include "AcString.h"

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
	virtual AttrObject& operator=(const AttrObject &rhs);
	void Clone(const AttrObject& p_src); //��¡ֻ��¡���ݣ�����¡������ϵ����m_instanceCodeId

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgOutFileInfo(AcDbDwgFiler* pFiler, const CDwgFileInfo& pFileInfo) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	virtual Acad::ErrorStatus dwgInFileInfo(AcDbDwgFiler* pFiler, CDwgFileInfo& pFileInfo);
	//}}AFX_ARX_METHODS
	
	virtual eRCType GetType(){return S_TYPENUM;}
	virtual bool isEqualTo(AttrObject*other);//��������һ��

	CString GetFileName()const{ return m_file.fileName; }
	void SetFileName(CString p_str){ m_file.fileName = p_str; }

	void SetInstanceCode(CString  bianHao);
	void SetInstanceCodeObjectId(AcDbObjectId p_id);
	CString GetInstanceCode()const { return m_instanceCode; }
	AcDbObjectId GetInstanceCodeObjectId()const { return m_instanceCodeId; }
	
	//20200324 �汾6��¥����Ϣ��AttrWindow�Ƶ����࣬�Ա�֧�����е�����
	CFloorInfo GetFloorInfo()const { return m_floorInfo; }
	void SetFloorInfo(CFloorInfo p_info) { m_floorInfo = p_info;  close(); }

	virtual eViewDir GetViewDir()const { return m_viewDir; }
	virtual void SetViewDir(eViewDir p_view) { } //Ĭ�ϲ��ܸı���ͼ����

public:
	Adesk::Int32 m_version;		//�ļ��汾 ���л�ʱʹ��

	//ԭ������
	CString m_prototypeCode;//ԭ�ͱ�� //����ԭ��������ԭ�ͱ��
	CDwgFileInfo m_file;	//ԭ���ļ���,����.dwg����������·��
	bool m_isDynamic;		//�Ƿ�̬��
	bool m_isJiTuan;		//�Ƿ���
	CString m_quyuId;			//����ID
	CString m_quyuName;			//��������

	//////////////////////////////////////////////////////////////////////////
	//ʵ������
	CString m_instanceCode;			//ʵ�����
	AcDbObjectId m_instanceCodeId; //ʵ�������ͼ����ʾ������ID  201912


	CFloorInfo m_floorInfo; //¥����Ϣ YUAN 20200324¥����Ϣ��AttrWindow�Ƶ����࣬�Ա�֧�����е�����

	eViewDir m_viewDir;//��ͼ����ƽ��ͼ������ͼ������ͼ YUAN 20200324¥����Ϣ��AttrWindow�Ƶ����࣬�Ա�֧�����е�����
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)