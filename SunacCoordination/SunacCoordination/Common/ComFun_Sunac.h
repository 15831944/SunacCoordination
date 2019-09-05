#pragma once

#define  SUNAC_DEBUG

#include "ComFun_ACAD_Common.h"
#include "TYRect.h"
#include "../Object/RCWindow.h"

#define  SUNAC_ATTRIBUTE_ENTITY L"SUNAC_ATTRIBUTE_ENTITY"
#define  SUNAC_COMMON_STR_LEN (256)
#define  FILE_VERSION ((Adesk::Int32)1)

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

void TY_GetAllWindowFiles(vCString &files);
void TY_GetAllKitchenFiles(vCString &files);
void TY_GetAllTieYiLanGanFiles(vCString &files);
void TY_GetAllBoLiLanGanFiles(vCString &files);

AcGePoint3d TY_GetPoint(CString prompt = L"��ѡ������");
TYRect TY_GetOneRect();
int TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2);

AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id);
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt);
int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt);

bool TY_IsWindow(AcDbObjectId Id);
bool TY_Iskitchen(AcDbObjectId Id);
bool TY_IsToilet(AcDbObjectId Id);
bool TY_IsAirCon(AcDbObjectId Id);
bool TY_IsDoor(AcDbObjectId Id);
bool TY_IsRailing(AcDbObjectId Id);

bool TY_IsPairsEqual(vRCPairKeyDValue &A, vRCPairKeyDValue &B);
bool TY_IsPairsEqual(vRCPairKeyIValue &A, vRCPairKeyIValue &B);
bool TY_IsPairsEqual(vRCPairKeyStrValue &A, vRCPairKeyStrValue &B);

int vFind(RCPairKeyDValue &A, vRCPairKeyDValue &B);
int vFind(RCPairKeyIValue &A, vRCPairKeyIValue &B);
int vFind(RCPairKeyStrValue &A, vRCPairKeyStrValue &B);
int vFind(RCWindow &oneWindow, vRCWindow &allWindows);




