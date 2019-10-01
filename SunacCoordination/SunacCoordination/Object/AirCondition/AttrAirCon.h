/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON)
#define ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "..\AttrObject.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON "ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON"
#endif

class DLLIMPEXP  AttrAirCon : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrAirCon);

	// Constructor / Destructor
	AttrAirCon();
	virtual ~AttrAirCon();

	AttrAirCon(const AttrAirCon &other);
	AttrAirCon & operator=(const AttrAirCon &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return AIRCON;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

private:


public:
	//double m_piShu;
	//CString m_lengNingShuiGuanPos;//����ˮ��λ��---��ȷ��
	//bool m_hasYuShuiGuan;//�Ƿ�����ˮ��


	//�ⴰ���ݱ��ڶ��ű��յ� ����
	CString m_airConId;				//�յ����
	//CString m_yxid;	//�յ�ԭ�ͱ��
	//CString m_name;	//�յ�ԭ���ļ�
	CString m_airConHorseNumber;	//�յ�ƥ��
	CString m_airConPipePos;		//�յ�����ˮ��λ��
	CString m_airConRainRiser;		//�յ��Ƿ�����ˮ���ܴ���
	CString m_airConRainRiserPos;	//�յ���ˮ��λ��
	CString m_airConInstallNetSize;	//�յ���װ���ߴ�
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON)