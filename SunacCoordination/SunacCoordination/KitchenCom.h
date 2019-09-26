#pragma once
#include "Sunac_def.h"



//-------------ͨ�ú���-----//

//kitchenId��������յ�id
//shuiPen:"����600"/"����800"/˫��900/˫��1000/˫��1200
int Kitchen_SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen);

//kitchenId��������յ�id
//bingXiang:������700;�Կ���800;�Կ���1000
int Kitchen_SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang);

//kitchenId��������յ�id
//shuiPen:"800"/"900"
int Kitchen_SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai);

//kitchenId��������յ�id
//shuiPen:"����_������_700"/"����_�Կ���_800"/"����_�Կ���_1000"
int Kitchen_SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang);


//-------------KUq�Կ�����-----//
//KUq�Ŵ��Կ������ŵ�λ��
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
//KUq�Ŵ��Կ�������̨��λ�� 
//zaoTaiType "800"/"900"
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType);
//KUq�Ŵ��Կ�����ˮ���λ��
int Kitchen_KUq_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian);


//-------------KUq��ֱ������-----//
//KUq�Ŵ���ֱ�������ŵ�λ��
int Kitchen_KUq_ChuiZhiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
//KUq�Ŵ���ֱ��������̨��λ�� 
//zaoTaiType "800"/"900"
int Kitchen_KUq_ChuiZhiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian);
//KUq�Ŵ���ֱ������ˮ���λ��
int Kitchen_KUq_ChuiZhiKai_SetShuiPenPos(AcDbObjectId kitchenId, double jinshen);


//-------------KUs�Կ�����-----//
//KUs�Ŵ��Կ�����ˮ���λ��
int Kitchen_KUs_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian);

//KUs�Ŵ��Կ�������̨��λ�� 
int Kitchen_KUs_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinShen);


//-------------KL�Կ�����-----//
//KL�Ŵ��Կ�����ˮ���λ��
int Kitchen_KL_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian);

//KL�Ŵ��Կ�������̨��λ�� 
//zaoTaiType "800"/"900"
int Kitchen_KL_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinShen, CString zaoTaiType);


//-------------KI�Կ�����-----//
//KI�Ŵ��Կ�����ˮ���λ��
//shuiPen:"����600"/"����800"/˫��900/˫��1000/˫��1200
int Kitchen_KI_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double jinShen, CString shuiPenType);

//KI�Ŵ��Կ�������̨��λ�� 
//zaoTaiType "800"/"900"
int Kitchen_KI_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, CString zaoTaiType);
