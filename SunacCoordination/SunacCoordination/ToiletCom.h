#pragma once
#include "Sunac_def.h"



//-------------ͨ�ú���-----//

//toiletId��������յ�id
//taiPen:"Sunac_̨��_650"/"Sunac_̨��_750"/Sunac_̨��_800/Sunac_̨��_900/Sunac_̨��_1000
int Toilet_SelectTaiPen(AcDbObjectId toiletId, CString taiPen);

//toiletId��������յ�id
//maTong:Sunac_��Ͱ750/Sunac_��Ͱ800
int Toilet_SelectBingXiang(AcDbObjectId toiletId, CString maTong);

//-------------------TI3---------------//
//yLen y�����ܳ������߳ߴ磩
//�Զ�ѡ����Ͱ
int TI3_SetMaTongPos(AcDbObjectId toiletId, double yLen);

//��������ĳ��߳ߴ����Ͱ�Ŀ�� �Զ�ɸѡ���ʵ�̨����
//maTongWidth 750/800
vCString TI3_GetTaiPens(AcDbObjectId toiletId, double yLen, double maTongWidth);

//TI3_G

//Y ȡֵ�� 950����1050---ע�����ѡ����950 ̨�����Ϊ900 ����Ϊ1000--
//ע�⣺��ϴ����̨���и���ϵ
int TI3_G_SetHuanXiQuY(AcDbObjectId toiletId, double yLen);


//-------------------TI4---------------//
//yLen y�����ܳ������߳ߴ磩
//�Զ�ѡ����Ͱ
int TI4_SetMaTongPos(AcDbObjectId toiletId, double yLen);
//��������ĳ��߳ߴ����Ͱ�Ŀ�� �Զ�ɸѡ���ʵ�̨����
//maTongWidth 750/800
vCString TI4_GetTaiPens(AcDbObjectId toiletId, double yLen, double maTongWidth);
//Y ȡֵ�� 950����1050---ע�����ѡ����950 ̨�����Ϊ900 ����Ϊ1000--
//ע�⣺��ϴ����̨���и���ϵ
int TI4_G_SetHuanXiQuY(AcDbObjectId toiletId, double yLen);


//-------------------TL---------------//
//linYuFangY 900/1000
//��������ĳ��߳ߴ����ԡ���Ŀ�� �Զ�ɸѡ���ʵ�̨����
vCString TL4_GetTaiPens(AcDbObjectId toiletId, double yLen, double linYuFangY);




