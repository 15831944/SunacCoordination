#pragma once

typedef enum eWindowDimType
{
	SINGLE,//����ֵ
	MULTI,//���ֵ
	SCOPE,//��Χֵ
	UNLIMIT//����ֵ
}eWindowDimType;

typedef enum eViewDir
{
	LIMINAN,//����ͼ
	PINGMIAN,//ƽ��ͼ
	CESHI,//����ͼ
}eViewDir;

typedef enum eRailingType
{
	TIEYI,//����
	BOLI,//����
}eRailingType;


typedef enum eWindowDoorPos//�Ŵ�λ�ù�ϵ
{
    DUIKAI,//�Կ�
	CHUIZHIKAI//��ֱ��
} eWindowDoorPos;


typedef enum eLingNingShuiGuanPos//����ˮ��λ��
{
	CEBIAN,//���
	HOUBIAN//���
}eLingNingShuiGuanPos;

typedef struct RCDimData
{
	eWindowDimType type;//ֵ���ͣ�����ֵ
	//����ǵ���ֵ ��ȡ��һ����
	//����Ƿ�Χֵ ��Сֵ�ǵ�һ�� ���ֵ�ǵڶ���
	//�����ϵ��ֵ �����ȡ
	//���� û��
	vdouble values;
}SRCDimData;
typedef std::vector<SRCDimData> vSRCDimData;
