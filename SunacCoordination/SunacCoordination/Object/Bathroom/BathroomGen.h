#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos); //���ɲ����뵽ָ����

	//////////////////////////////////////////////////////////////////////////
	//����ֵ������ѡ��
	virtual vCString GetTaipenOptions();// ��ȡ̨��ѡ��
	virtual CString GetTaipenDefault(); //̨��Ĭ��ֵ

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();


	//////////////////////////////////////////////////////////////////////////
	//���û�������
	void SetDoorDir(E_DIRECTION p_doorDir){ m_doorDir = p_doorDir; }
	void SetWindowDir(E_DIRECTION p_winDir){ m_windowDir = p_winDir; }

	//���������ֵ����ͨ��ֱ������AttrBathroom����ı���ʵ��
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual int SelectTaipen(AcDbObjectId bathroomId, CString taipen);

	virtual int SelectMatong(AcDbObjectId bathroomId, CString matong);

	virtual int SelectGuanxiqu(AcDbObjectId bathroomId, CString guanxiqu);

	virtual int SetDoorPos(AcDbObjectId bathroomId, double kaiJian) = 0;

	virtual int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth) = 0;

	virtual int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType) = 0;

	virtual void GetRotateAngle(double &angle, AcGeVector3d &offsetV); //������ת��ϵ

	virtual double GetXLength(){ return 0; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(){ return 0; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	AttrBathroom m_attr;

	AcDbObjectId m_id; //���ɲ����뵽ͼ�ϵĳ�����ID

	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;

	double m_angle;
};

class CBathroomGenKI : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomGenKU : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomGenKL : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomMrg
{
public:
	static CBathroomMrg* GetInstance();

	vector<AttrBathroom*> FilterBathroom(EBathroomType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);

protected:
	//���ߴ��뷽��
	vector<AttrBathroom*> FilterBathroomTU(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTL(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTI(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
};