#pragma once

#include <vector>
#include "..\Tool\MarkupXml\Markup.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"


class CKitchenBathroomWebData
{
public:
	CKitchenBathroomWebData(){ }
	~CKitchenBathroomWebData(){}

	bool GetKitchenById(CString p_sId, AttrKitchen& value)const;
	bool GetAttrBathroomById(CString p_sId, AttrBathroom& value)const;


	std::vector<AttrKitchen> GetKitchens( double kaiJian,//����
												double jinShen,//����
												CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ �Ŵ��Կ�/�Ŵ���ֱ��
												CString type,//�������� I��
												bool hasPaiQiDao//�Ƿ���������
												/*bool bDynatic*/)const; //�Ƿ�̬

	std::vector<AttrKitchen> GetAllKitchens()const;


	std::vector<AttrBathroom> GetBathrooms(double width,//��ȣ�X����
													double height,//�߶ȣ�Y����
													CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
													CString type,//����������
													bool hasPaiQiDao)const;//�Ƿ���������
		

	std::vector<AttrBathroom> GetAllBathrooms()const;

	std::vector<AttrKitchen > ParseKitchensFromXML(CMarkup xml)const;//��XML����������Ϣ
	std::vector<AttrBathroom > ParseBathroomsFromXML(CMarkup xml)const;//��XML������������Ϣ

protected:

};

