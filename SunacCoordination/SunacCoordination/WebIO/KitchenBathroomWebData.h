#pragma once

#include <vector>
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
												bool hasPaiQiDao,//�Ƿ���������
												bool bDynatic); //�Ƿ�̬

	std::vector<AttrKitchen> GetAllKitchens();


	std::vector<AttrBathroom> GetBathrooms(double width,//��ȣ�X����
													double height,//�߶ȣ�Y����
													CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
													CString type,//����������
													bool hasPaiQiDao);//�Ƿ���������
		

	std::vector<AttrBathroom> GetAllBathrooms();

protected:

};

