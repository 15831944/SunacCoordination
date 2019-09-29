#pragma once
#include <vector>
#include "AttrKitchen.h"
#include "..\PrototypeCommonDef.h"

using namespace std;

class CKitchGen
{
public:
	CKitchGen(AttrKitchen* p_att);
	virtual ~CKitchGen();

	virtual AcDbObjectId GenKitchen(const AcGePoint3d p_pos); //���ɲ����뵽ָ����

	//////////////////////////////////////////////////////////////////////////
	//����ֵ������ѡ��
	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	virtual CString GetZhaotaiDefault();


	//////////////////////////////////////////////////////////////////////////
	//���û�������
	void SetDoorDir(E_DIRECTION p_doorDir){ m_doorDir = p_doorDir; }
	void SetWindowDir(E_DIRECTION p_winDir){ m_windowDir = p_winDir; }

	//���������ֵ����ͨ��ֱ������AttrKitchen����ı���ʵ��
	AttrKitchen* GetKitchenAtt(){ return &m_attr; }

protected:
	//kitchenId��������յ�id
	//shuiPen:"����600"/"����800"/˫��900/˫��1000/˫��1200
	virtual int SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen);

	//kitchenId��������յ�id shuiPen:"800"/"900"
	virtual int SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai);

	//kitchenId��������յ�id
	//bingXiang:"����_������_700"/"����_�Կ���_800"/"����_�Կ���_1000"
	virtual int SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang);

	//������λ�ù�ϵ
	virtual int SetDoorPos(AcDbObjectId kitchenId, double kaiJian) { return 0; }
	//������̨��λ�� 
	virtual int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType){ return 0; }
	//����ˮ���λ��
	virtual int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType){ return 0; }

	virtual void GetRotateAngle(double &angle, AcGeVector3d &offsetV); //������ת��ϵ
	//virtual void InitMirror(); //��Ҫ����Ŵ���ֱ��������Ŵ���ֱԭ�͵Ĵ����ŵ��Ҳ࣬��ʵ��Ϊ�������Ҫ�Գ� //��Ϊ�Ի������ж�

	virtual double GetXLength(){ return m_attr.m_width; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(){ return m_attr.m_height; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	AttrKitchen m_attr;

	AcDbObjectId m_id; //���ɲ����뵽ͼ�ϵĳ�����ID

	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;

	double m_angle;
};
//////////////////////////////////////////////////////////////////////////
class CKitchGenKUQ : public CKitchGen	//ǳU���Ŵ��Կ�
{
public:
	CKitchGenKUQ(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	//�������̨��Ĭ�ϵĴ���
	//virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	//virtual CString GetBinxiangDefault();
	//virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	//virtual CString GetZhaotaiDefault();

protected:
	int SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
};

class CKitchGenKUQ_C : public CKitchGen//ǳU���Ŵ���ֱ��
{
public:
	CKitchGenKUQ_C(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	//�������̨��Ĭ�ϵĴ���
	//virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	//virtual CString GetBinxiangDefault();
	//virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	//virtual CString GetZhaotaiDefault();


protected:
	int SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);

};

class CKitchGenKUS : public CKitchGen////��U���Ŵ��Կ�
{
public:
	CKitchGenKUS(AttrKitchen* p_att);

	//����Ĭ�ϴ���
	//virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	//virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	//virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	//virtual CString GetBinxiangDefault();

	//virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	//virtual CString GetZhaotaiDefault();

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};

class CKitchGenKL : public CKitchGen////��U���Ŵ��Կ�
{
public:
	CKitchGenKL(AttrKitchen* p_att);

	//virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	//virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	virtual CString GetBinxiangDefault();

	//virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	//virtual CString GetZhaotaiDefault();
	virtual double GetXLength(){ return m_attr.m_height; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(){ return m_attr.m_width; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};

class CKitchGenKI : public CKitchGen////��U���Ŵ��Կ�
{
public:
	CKitchGenKI(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	//virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	//virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	virtual CString GetZhaotaiDefault();

	virtual double GetXLength(){ return m_attr.m_height; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(){ return m_attr.m_width; } //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);

	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};
class CKitchGenSTATIC: public CKitchGen////��̬����
{
public:
	CKitchGenSTATIC(AttrKitchen* p_att);



};


//////////////////////////////////////////////////////////////////////////
class CKitchMrg
{
	CKitchMrg();
public:
	static CKitchMrg *GetInstance();
	~CKitchMrg();


	//����������������ԭ��
	vector<AttrKitchen*> FilterKitch(EKitchType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);

	//����ԭ�ʹ����������ɶ���
	CKitchGen* CreateKitchGenByKitchType(AttrKitchen* p_attKitchen);

protected:
	vector<AttrKitchen*> FilterKitch_Internal(EKitchType p_type, int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterStatic(int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUq(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUqc(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUs(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKL(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKI(int p_width, int p_height, bool p_bHasAirVent);

};