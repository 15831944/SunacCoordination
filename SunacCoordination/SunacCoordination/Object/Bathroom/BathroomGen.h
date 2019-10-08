#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"
#include "../PrototypeInfo.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos, double p_angle); //���ɲ����뵽ָ����

	//////////////////////////////////////////////////////////////////////////
	//����ֵ������ѡ��
	virtual vCString GetTaipenOptions();// ��ȡ̨��ѡ��
	virtual CString GetTaipenDefault(); //̨��Ĭ��ֵ

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();

	virtual bool CheckParameter() { return true; } //����ǰ�������Ϸ���

	//���������ֵ����ͨ��ֱ������AttrBathroom����ı���ʵ��
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual int SelectTaipen(AcDbObjectId bathroomId, CString taipen);
	virtual int SelectMatong(AcDbObjectId bathroomId, CString matong);
	virtual int SelectGuanxiWidth(AcDbObjectId bathroomId, double width);

	virtual int SetMatongPos(AcDbObjectId bathroomId, double jinShen) { return 0; } //�Զ����㲢������Ͱλ��

	virtual double GetXLength(){ return m_attr.m_width; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(){ return m_attr.m_height; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	AttrBathroom m_attr;

	AcDbObjectId m_id; //���ɲ����뵽ͼ�ϵĳ�����ID
};

class CBathroomGenKI : public CBathroomGen
{
public:
	CBathroomGenKI(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	virtual int SetMatongPos(AcDbObjectId bathroomId, double yLen);

protected:
	int SetMatongPos_I3(AcDbObjectId bathroomId, double yLen);
	int SetMatongPos_I4(AcDbObjectId bathroomId, double yLen);
};

class CBathroomGenKU : public CBathroomGen
{
public:
	CBathroomGenKU(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomGenKL : public CBathroomGen
{
public:
	CBathroomGenKL(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomMrg
{
public:
	static CBathroomGen* CreateBathroomByAttribute(AttrBathroom* p_attr);
};
