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


typedef enum eLengNingShuiGuanPos//����ˮ��λ��
{
	AIRCON_CEBIAN,//���
	AIRCON_HOUBIAN//���
}eLingNingShuiGuanPos;


class DLLIMPEXP  AttrAirCon : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrAirCon);

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
	virtual bool IsPrototypeEqual(const AttrAirCon& p_att);

	CString AutoInstanceCode(); //�յ��Զ����ֻ����ƥ����������ͳ�ƣ������ڲ��ҿ�

public:
	double m_power;			//�յ�ƥ��
	CString m_pipePos;		//�յ�����ˮ��λ��
	bool m_hasRainPipe;		//�յ��Ƿ�����ˮ���ܴ���
	CString m_rainPipePos;	//�յ���ˮ��λ��

	int m_airW;		//��Ӧ�յ����ݱ��
	int m_airH;
	int m_airD;
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON)