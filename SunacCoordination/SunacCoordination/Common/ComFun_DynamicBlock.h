#pragma once
#include <dbdynblk.h>
#include <dbobjptr.h>

bool TYCOM_IsDyanamicBlock(AcDbObjectId id);

CString TYCOM_GetDynaBlkName(AcDbObjectId eId);
int TYCOM_GetDynamicBlockName(AcDbBlockReference *pBlkRef, CString &name);
int TYCOM_GetDynamicBlockName(AcDbObjectId entId, CString &name);


//----------------���ö�̬������--------------//
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, double value);
//����int��bool���͵�
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, Adesk::Int32 value);
//����ö�����͵Ķ�̬������
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, short index);
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, CString value);

//----------------��ȡ��̬������--------------//
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, double &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, int &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, bool &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, CString &value);

//����
int TYCOM_CycleAllDynamicBlockReferencesInRect(vAcDbObjectId &vids, AcGePoint3d lb, AcGePoint3d rt);
int TYCOM_CycleAllDynamicBlockReferencesInLayer(vAcDbObjectId &vids, CString layer);





