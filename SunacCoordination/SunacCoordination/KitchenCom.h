#pragma once
#include "Sunac_def.h"



//-------------ͨ�ú���-----//

//kitchenId��������յ�id
//shuiPen:"����600"/"����700"/˫��900/˫��1000/˫��1200
int Kitchen_SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen);

//kitchenId��������յ�id
//shuiPen:"800"/"900"
int Kitchen_SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai);



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