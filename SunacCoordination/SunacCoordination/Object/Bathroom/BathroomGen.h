#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"
//#include "../PrototypeInfo.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos, int p_angle); //���ɲ����뵽ָ����

	//////////////////////////////////////////////////////////////////////////
	//����ֵ������ѡ��
	virtual vCString GetTaipenOptions();// ��ȡ̨��ѡ��
	virtual CString GetTaipenDefault(); //̨��Ĭ��ֵ

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();

	virtual void InitBathroomByDefault();

	virtual bool CheckParameter(CString& errMsg); //����ǰ�������Ϸ���

	//���������ֵ����ͨ��ֱ������AttrBathroom����ı���ʵ��
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual void SelectTaipen(AcDbObjectId bathroomId, CString taipen);
	virtual void SelectMatong(AcDbObjectId bathroomId, CString matong);
	virtual void SelectGuanxiWidth(AcDbObjectId bathroomId, double width);

	virtual void SetMatongPos(AcDbObjectId bathroomId) {} //�Զ����㲢������Ͱλ��
	virtual void SetVantTotalSize(AcDbObjectId bathroomId) {} //�Զ����㲢���������������ܳߴ�
	virtual void SetXiyijiPos(AcDbObjectId bathroomId) {} //�Զ����㲢���������������ܳߴ�

protected:
	AttrBathroom m_attr;
};

class CBathroomGenKI : public CBathroomGen
{
public:
	CBathroomGenKI(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	virtual bool CheckParameter(CString& errMsg); //����ǰ�������Ϸ���

	virtual void SetVantTotalSize(AcDbObjectId bathroomId);
	virtual void SetMatongPos(AcDbObjectId bathroomId);
	virtual void SetXiyijiPos(AcDbObjectId bathroomId);

protected:
	double GetMatongPos();
	double GetMatongPos_I3();
	double GetMatongPos_I4();
};

class CBathroomGenKU : public CBathroomGen
{
public:
	CBathroomGenKU(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	virtual vCString GetTaipenOptions() { return vCString(0); }
	virtual CString GetTaipenDefault() { return L""; }

	virtual vCString GetMatongOptions() {return vCString(0); }
	virtual CString GetMatongDefault() { return L""; }

	virtual vCString GetGuanxiquOptions() {return vCString(0); }
	virtual CString GetGuanxiquDefault() { return L""; }

	//U��Ϊ��̬����������Ҫ���
	virtual bool CheckParameter(CString& errMsg) { return true; }

	//U��Ϊ��̬��������������
	virtual void SelectTaipen(AcDbObjectId bathroomId, CString taipen) {}
	virtual void SelectMatong(AcDbObjectId bathroomId, CString matong) {}
	virtual void SelectGuanxiWidth(AcDbObjectId bathroomId, double width) {}

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomGenKL : public CBathroomGen
{
public:
	CBathroomGenKL(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	virtual bool CheckParameter(CString& errMsg); //����ǰ�������Ϸ���

	virtual void SetVantTotalSize(AcDbObjectId bathroomId);
	virtual void SetXiyijiPos(AcDbObjectId bathroomId);

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomMrg
{
public:
	static CBathroomGen* CreateBathroomByAttribute(AttrBathroom* p_attr);
};
