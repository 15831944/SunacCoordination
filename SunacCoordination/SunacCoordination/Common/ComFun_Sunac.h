#pragma once

#define  SUNAC_DEBUG

#include "ComFun_ACAD_Common.h"
//�ж��ǲ���һ��ֱ�ߣ��߶Σ�
bool DQ_IsALine(AcDbObjectId entId);
//�ж��ǲ���һ�������
bool DQ_IsBlockReference(AcDbObjectId id);
//���û�ѡ��������ѡ������
void DQ_SelectMany(vAcDbObjectId &ids);

//�õ�ͼ��Ĳ����
int DQ_GetBlockReferenceInsertPoint(AcDbObjectId id, AcGePoint3d &pnt);

//�õ����ߺ͵�������
int DQ_GetCurveClosedPointToOneGivenPoint(AcDbObjectId curvre, AcGePoint3d givenPnt, AcGePoint3d &closedPnt);



//�������û�� attributename����
bool DQ_HasAttribute_BlkDef(AcDbObjectId blockDefId, CString attributename);
//�������û�� attributename����
bool DQ_HasAttribute_BlkRef(AcDbObjectId blockRefId, CString tagText);

//��ȡ���������
bool DQ_GetAttributeValue( AcDbObjectId blockRefId, CString attributename, CString &value );
//��ȡ���������
bool DQ_GetAttributeValue( AcDbBlockReference *pBlkRef, const TCHAR* attributename, CString &value );

int MD2010_GetAllTypedObjectsInLayer(vAcDbObjectId &allEntites, CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids);

void YT_UpdateBlockReference(AcDbObjectId &entId);





