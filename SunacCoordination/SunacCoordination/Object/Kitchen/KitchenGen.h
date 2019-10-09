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

	virtual AcDbObjectId GenKitchen(const AcGePoint3d p_pos, int p_angle); //���ɲ����뵽ָ����

	//////////////////////////////////////////////////////////////////////////
	//����ֵ������ѡ��
	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	virtual CString GetZhaotaiDefault();

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

	virtual double GetXLength(); //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����
	virtual double GetYLength(); //���x����ĳ��ȣ�width�����height�ǽ������ʱ��width����x����

protected:
	AttrKitchen m_attr;
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

class CKitchGenKL : public CKitchGen////L�ͣ��Ŵ��Կ�
{
public:
	CKitchGenKL(AttrKitchen* p_att);

	//virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	//virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	virtual CString GetBinxiangDefault();

	//virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	//virtual CString GetZhaotaiDefault();

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};

class CKitchGenKI : public CKitchGen//I�ͣ��Ŵ��Կ�
{
public:
	CKitchGenKI(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// ��ȡˮ��ѡ��
	virtual CString GetShuipenDefault(); //ˮ��Ĭ��ֵ

	//virtual vCString GetBinxiangOptions();// ��ȡ����ѡ��
	//virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// ��ȡ��̨ѡ��
	virtual CString GetZhaotaiDefault();

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
public:
	//����ԭ�ʹ����������ɶ���
	static CKitchGen* CreateKitchGenByKitchType(AttrKitchen* p_attr);
};