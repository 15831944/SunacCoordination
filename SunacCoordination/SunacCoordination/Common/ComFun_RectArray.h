#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

/*
    ���ļ���Ҫ�漰�������У�·�����кͻ������е�һЩ������
	�κ�һ����������ͨ��ͼ��չʾ�����ģ�һ���漰����idһ����ѡ����߱���������arrayid��
	���arrayid����ת����һ��blockreference��Ȼ�����ը����

	ͨ��arrayid�ܹ��õ�actionid��actionid�ǿ���������еĲ����ģ�ͨ�����actionid��parameter����ʵ�����еĲ����޸�

	ע�⣺��̬�鲻��actionbody --��������TYCOM_GetActionId �޷��ɹ�

	ע�⣺actionbody ��д���Լ��� explode
*/

//�ж�һ��ѡ����߱���������id�ǲ�������
bool TYCOM_IsArray(AcDbObjectId arrayid);
AcDbObjectId TYCOM_GetActionId(AcDbObjectId arrayid);
int TYCOM_ModifyRectArrayParm_DEMO(AcDbObjectId actionid);


/*
������֤�������������õ��Ľ����TYCOM_GetArrayObjects ��id��һ�µ�
TYCOM_CycleBlockReferenceEntites(eId,ids2);---����1

AcDbObjectId actionID = TYCOM_GetActionId(arrayid);---����2
TYCOM_GetArrayObjects(actionID,ids1);

*/
int TYCOM_GetArrayObjects(AcDbObjectId actionid, vAcDbObjectId &ids);

int TYCOM_CreateRectArrayDemo();
int TYCOM_CreatePolarArrayDemo();
int TYCOM_CreatePathArrayDemo();



//һ��˫������ �ڲ�һ����3*4��window�� ����һ����2*2���ڲ��
/*
ads_name ename;
ads_point pt;
if (acedEntSel(L"\nSelect a dynamic block reference: ", ename, pt) != RTNORM)
{
acutPrintf(L"\nError selecting entity.");
return;
}
AcDbObjectId eId;
acdbGetObjectId(eId, ename);

vAcDbObjectId ids1,ids2,ids3,ids4;
AcDbObjectId actionID = TYCOM_GetActionId(eId);
TYCOM_GetArrayObjects(actionID,ids1);//�õ�4������ ÿ��������һ���� ����arrayid

for(int i = 0; i < ids1.size(); i++)
{
    TYCOM_CycleBlockReferenceEntites(ids1[0],ids2);//ͨ��������� �õ���һ�����ڲ��� arrayid

    TYCOM_IsArray(ids2[0]);
    actionID = TYCOM_GetActionId(ids2[0]);
    TYCOM_GetArrayObjects(actionID,ids3);//�����12���ڲ���ͼ�� ������window

    TYCOM_CycleBlockReferenceEntites(ids3[0],ids4);//ͨ��ͼ��õ���Ӧ��window
    TY_IsWindow(ids4[0],eViewDir::E_VIEW_ALL);
}
*/