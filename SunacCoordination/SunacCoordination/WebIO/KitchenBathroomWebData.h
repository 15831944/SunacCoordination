#pragma once

#include <vector>
#include "..\Tool\MarkupXml\Markup.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "WebCommon.h"


class CKitchenBathroomWebData
{
public:
	CKitchenBathroomWebData(){ }
	~CKitchenBathroomWebData(){}

	bool GetKitchenById(CString p_sId, AttrKitchen& value)const;
	bool GetAttrBathroomById(CString p_sId, AttrBathroom& value)const;


/*
	std::vector<AttrKitchen> GetKitchens( double kaiJian,//����
												double jinShen,//����
												CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ �Ŵ��Կ�/�Ŵ���ֱ��
												CString type,//�������� I��
												bool hasPaiQiDao//�Ƿ���������
												/ *bool bDynatic* /)const; //�Ƿ�̬*/
	std::vector<AttrKitchen> GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao);

	std::vector<AttrKitchen> GetAllKitchens();

	std::vector<AttrBathroom> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	/*std::vector<AttrBathroom> GetBathrooms(double width,//��ȣ�X����
													double height,//�߶ȣ�Y����
													CString weiZhiGuanXi,//�Ŵ�λ�ù�ϵ
													CString type//����������
													/ *bool hasPaiQiDao* /)const;//�Ƿ���������*/
		

	std::vector<AttrBathroom> GetAllBathrooms();

	std::vector<AttrKitchen > ParseKitchensFromXML(CMarkup xml);//��XML����������Ϣ
	std::vector<AttrBathroom > ParseBathroomsFromXML(CMarkup xml);//��XML������������Ϣ

	std::vector<AttrKitchen> m_allKitchens;
	std::vector<AttrBathroom> m_allBathrooms;

protected:

};

