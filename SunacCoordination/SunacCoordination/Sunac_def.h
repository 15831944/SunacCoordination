#pragma once

#define  SUNAC_DEBUG
#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"
#define  SUNAC_COMMON_STR_LEN (256)
#define  FILE_VERSION ((Adesk::Int32)1)

#define  WORK_LOCAL//�Ƿ񱾵ع���ģʽ
#define CHECK_USE//�Ƿ���Ȩ��

//ģ������
typedef enum eRCType
{
    WINDOW,//�ⴰ
	DOOR,//��
	KITCHEN,//����
	Bathroom,//������
	AIRCON,//�յ�
	RAILING,//����
	TYPENUM
}eRCType;

//�Ŵ�ȡֵ��������
typedef enum eWindowDimType
{
	SINGLE,//�̶�ֵ
	MULTI,//ֵϵ��
	SCOPE,//��Χ
	UNLIMIT,//����
	CALC,//��ʽ
	NOVALUE //��
}eWindowDimType;

typedef enum eViewDir
{
	E_VIEW_FRONT,//����ͼ
	E_VIEW_TOP,	//ƽ��ͼ
	E_VIEW_LEFT,//����ͼ
	E_VIEW_EXTEND,//չ��ͼ
}eViewDir;

typedef enum eRailingType
{
	E_RAILING_TIEYI,//����
	E_RAILING_BOLI,//����
}eRailingType;

enum E_Prototype_Type
{
	E_DYNAMIC_PROTOTYPE,	//��̬ԭ��
	E_STATIC_PROTOTYPE,		//��̬ԭ��
};

typedef enum eWindowDoorPos//�Ŵ�λ�ù�ϵ
{
    DUIKAI,//�Կ�
	CHUIZHIKAI//��ֱ��
} eWindowDoorPos;


typedef enum eLengNingShuiGuanPos//����ˮ��λ��
{
	AIRCON_CEBIAN,//���
	AIRCON_HOUBIAN//���
}eLingNingShuiGuanPos;

typedef enum eWindowCalcType//�Ŵ���������
{
	CALC_XINGCAI,//�Ͳ�
	CALC_BOLI,//����
	CALC_WUJIN,//���
	CALC_FUCAI//����
}eWindowCalcType;

typedef struct SCalcData//�Ŵ���������
{
    eWindowCalcType calcType;
	CString formula;
}SCalcData;

typedef std::vector<eWindowCalcType> vSCalcData;

typedef struct RCDimData
{
	CString sCodeName; //���� W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;//ֵ����
	vdouble values;  //����ֵ���ͣ��ɴ洢����ֵ��ϵ�С�ȡֵ��Сֵ�����ֵ
	double minValue;
	double maxValue;
	CString sFomula;	//��ʽ
	double defaultValue;
	CString prompt;  //˵��
}SRCDimData;
typedef std::vector<SRCDimData> vSRCDimData;
