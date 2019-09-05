#include "StdAfx.h"
#include "WebIO.h"
#include "float.h"
#include <acdocman.h>
#include <dbxutil.h>

WebIO::WebIO()
{
	m_quyus = GetQuyus();//����
	m_gongNengQus = GetGongNengQus();//������
	m_windowDoorPoss = GetWindowDoorPoss();//�Ŵ�λ�ù�ϵ
	m_windowOpenTypes = Window_GetOpenTypes();//��������
	m_doorTypes = Door_GetTypes();//������
	m_kitchenTypes = Kitchen_GetTypes();//��������
	m_kitchenShuiPenTypes = Kitchen_GetShuiPenTypes();//ˮ������
	m_kitchenBingXiangTypes = Kitchen_GetBingXiangTypes();//��������
	m_kitchenZaoTaiWidths = Kitchen_GetZaoTaiWidths();//��̨���
	m_toiletTypes = Toilet_GetTypes();//����������
	m_toiletTaiPenWidths = Toilet_GetTaiPenWidths();//����������
	m_toiletMaTongTypes = Toilet_GetMaTongTypes();//����������
	m_toiletGuanXiWidths =Toilet_GetGuanXiWidths();//��ϴ�����---�Ƿ�֧���ֶ����룿��
	m_airPiShus = Air_GetPiShus();//���е�ƥ��
	m_airLengNingShuiGuanPos =  Air_GetLengNingShuiGuanPos();//����ˮ��λ��
}

WebIO::~WebIO()
{

}

int WebIO::GetWindows
(
   double width,//���ֵ��ע��߶�ֵ����Ϊ�������� 
   CString openType, //��������
   int openNum,//����������
   CString gongNengQu,//������
   double tongFengLiang//ͨ����
)
{
	return 0;
}

int WebIO::GetDoors
(
    double minWid, //���ֵ��ע��߶�ֵ����Ϊ�������� 
    CString doorType //������
)
{
	return 0;
}

int WebIO::GetKitchens
(
  double kaiJian,//����
  double jinShen,//����
  CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
  CString type,//��������
  bool hasPaiQiDao//�Ƿ���������
)
{
	return 0;
}

int WebIO::GetToilets
(
    double kaiJian,//����
    double jinShen,//����
    CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
    CString type,//����������
    bool hasPaiQiDao//�Ƿ���������
)
{
	return 0;
}

int WebIO::GetAirCon
(
    double piShu,
	CString weiZhi,//����ˮ��λ��
	bool hasYuShuiGuan
)
{
	return 0;
}

int WebIO::GetRailings()//һ���������е�
{
	return 0;
}

vCString WebIO::GetQuyus()
{
	vCString strs;
	return strs;
}
vCString WebIO::GetGongNengQus()
{
	vCString strs;
	return strs;
}
vCString WebIO::GetWindowDoorPoss()
{
	vCString strs;
	return strs;
}
vCString WebIO::Window_GetOpenTypes()
{
	vCString strs;
	return strs;
}
//��ȡ�ߴ�ֵ
eWindowDimType WebIO::Window_GetDim(CString yuanXingBianHao, CString dimStr)
{
	eWindowDimType value;
	return value;
}

vCString WebIO::Door_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetShuiPenTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetBingXiangTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetZaoTaiWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetTaiPenWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetMaTongTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetGuanXiWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Air_GetPiShus()
{
	vCString strs;
	return strs;
}
vCString WebIO::Air_GetLengNingShuiGuanPos()
{
	vCString strs;
	return strs;
}