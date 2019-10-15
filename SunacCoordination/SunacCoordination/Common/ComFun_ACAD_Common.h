
#pragma once

#include "gepnt3d.h"
#include "dbents.h"
#include "dbapserv.h"
#include "dbmain.h"
#include <vector>
#include <afxdb.h>

typedef enum ACDBOBJECTTYPE 
{
    ACDB_CURVE = 0, ACDB_POINT, ACDB_LINE,ACDB_CYCLE,ACDB_ARC,ACDB_0PLINE, ACDB_2dPLINE,ACDB_MLINES,ACDB_SPLINE,
	ACDB_ELLIPSE,ACDB_TEXT,ACDB_MTEXT,ACDB_HATCH
} eACDBOBJECTTYPE, *peACDBOBJECTTYPE;


//ʵ�����
AcDbObjectId MD2010_PostModalToBlockTable(const ACHAR* entryName, AcDbEntity *pent);


AcDbObjectId MD2010_CreateLine(const AcGePoint3d &ptStart, const AcGePoint3d &ptEnd,
							   const ACHAR* entryName = ACDB_MODEL_SPACE, Adesk::UInt8 color = 3);
AcDbObjectId MD2010_CreateLine(double startx, double starty, double startz,
							   double endx, double endy, double endz,
							   const ACHAR* entryName = ACDB_MODEL_SPACE, Adesk::UInt8 color = 3);
AcDbObjectId MD2010_CreateLine(double startx, double starty,
							   double endx, double endy,
							   const ACHAR* entryName = ACDB_MODEL_SPACE, Adesk::UInt8 color = 3);
AcDbObjectId MD2010_CreateArc(const AcGePoint3d &center, double radius, double startangle, double endangle,
							  const ACHAR* entryName = ACDB_MODEL_SPACE);
AcDbObjectId MD2010_CreateArc(double centerx, double centery, double centerz,
							  double radius, double startangle, double endangle,
							  const ACHAR* entryName = ACDB_MODEL_SPACE);
AcDbObjectId MD2010_CreateArc(double centerx, double centery,
							  double radius, double startangle, double endangle,
							  const ACHAR* entryName = ACDB_MODEL_SPACE);
AcDbObjectId MD2010_CreateCycle(AcGePoint3d center, double radius,AcGeVector3d dir = AcGeVector3d(0,0,1),
								const ACHAR* entryName = ACDB_MODEL_SPACE);

AcDbObjectId MD2010_CreateCycle(double centerx, double centery, double centerz, double radius,
								const ACHAR* entryName = ACDB_MODEL_SPACE);

AcDbObjectId MD2010_CreatePolyLine(vAcGePoint3d &pnts,
                                   const ACHAR* entryName = ACDB_MODEL_SPACE, Adesk::UInt8 color = 3);


int              MD2010_SetEntityColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);
Adesk::UInt16    MD2010_GetEntityColor(AcDbObjectId entId);
int              MD2010_SetEntityLayer(AcDbObjectId objID, CString strLayerName);
CString          MD2010_GetEntityLayer(AcDbObjectId objID);
AcDbObjectId     MD2010_GetEntityLayerID(AcDbObjectId objID);
int              MD2010_SetLineType(AcDbObjectId objID, CString strLinetype);
CString          MD2010_GetLineType(AcDbObjectId objID);
AcDbObjectId     MD2010_GetLineTypeID(AcDbObjectId objID);
int              MD2010_SetLineWeight(AcDbObjectId objID, AcDb::LineWeight weight );
AcDb::LineWeight MD2010_GetLineWeight(AcDbObjectId objID);

//��ע����
AcDbObjectId MD2010_AddAlignedDimension(AcGePoint3d start,AcGePoint3d end, AcGePoint3d dimlinpnt, const WCHAR * entryname = ACDB_MODEL_SPACE,
										const ACHAR* newLayer = L"0");
//�Ŵ���ͼ
AcDbObjectId MD2010_AddAlignedDimension2(AcGePoint3d start,AcGePoint3d end, AcGePoint3d dimlinpnt, const ACHAR* newLayer = L"0", int colorIndex = 2, double textHeight = 120);


//���ֲ���
int              MD2010_SetModelSpaceCurrentLayout();
int              MD2010_SetCurrentLayout(const ACHAR * layoutname);
const ACHAR *    MD2010_GetCurrentLayout();
void             MD2010_UpdateLayout();
                 //should call MD2010_UpdateLayout after this.
int              MD2010_CopyLayoutWithSameConfig(const WCHAR * tname, const WCHAR* newname);
bool             MD2010_CheckLayoutExist(const WCHAR * tname);
                 //should call MD2010_UpdateLayout after this.
int              MD2010_DeleteLayout(const WCHAR * tname);

//ͼ�����
AcDbObjectId     MD2010_CreateNewLayer(CString newname);
int              MD2010_DeleteLayer(CString layerName);
AcDbObjectId     MD2010_GetLayerID(CString layerName);
CString          MD2010_GetLayerName(AcDbObjectId id);
int              MD2010_GetAllLayers(vCString &vstr, vAcDbObjectId &vids);
int              MD2010_SetCurrentLayer(CString name);
AcDbObjectId     MD2010_GetCurrentLayer(CString &name);
int              MD2010_SetLayerProperty(CString layerName,
							bool bFlagColor,AcCmColor newcolor, 
							bool bFlagLT,AcDbObjectId lintypeid,
							bool bFlagLW,AcDb::LineWeight linw,
							bool bFlagFroze,bool froze, 
							bool bFlagOF,bool onoff, 
							bool bVPFroze,bool vpfroze, 
							bool bLock ,bool lock);
int              MD2010_GetLayerCount();

//��������
int              MD2010_CycleAllTypedObjectsInLayer(CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids);
int              MD2010_CycleAllTypedObjectsInAllLayer(eACDBOBJECTTYPE type, vAcDbObjectId &vids);
int              MD2010_CycleAllBlockReferencesInLayer(CString layname,  vAcDbObjectId &vids);
int              MD2010_CycleAllBlockReferencesInAllLayer(vAcDbObjectId &vids);
int              MD2010_CycleAllEntitesInLayer(CString layname, vAcDbObjectId &vids);
int              MD2010_CycleAllEntites(vAcDbObjectId &vids);
int              MD2010_CycleAllBlockReferencesInRect(vAcDbObjectId &vids, AcGePoint3d lb, AcGePoint3d rt);


//��ͨ���㺯��
int              MD2010_LoopCount1(vAcDbObjectId vids);
int              MD2010_LoopCountCurve1(vAcDbObjectId vids);
int              MD2010_LoopCountCurve(AcDbObjectId *ids, int num, AcGePoint3d st,vAcGePoint3d &rvec);

bool             MD2010_CheckLoopPoints(vAcGePoint3d v3d);
bool             MD2010_IsOneLoop(vAcDbObjectId vids);
int              MD2010_CheckRect(const WCHAR * layername);
bool             MD2010_CheckRect_2(AcDbObjectId curveid, Adesk::UInt16 colorIndex);




//ͼ��ѯ��
int              MD2010_GetLinePoints(AcDbObjectId lineid, AcGePoint3d& start, AcGePoint3d& end);

//
int              MD2010_GetCurrentMode();//1ģ�Ϳռ�



//�����

/*˵�� 
01.  ����GetAt������������ʽ����һ���Ǹ�һ�����ֵõ�����¼��ID
    �ڶ����Ǹ�һ�����֣�ֱ�ӵõ�����ָ�룬�൱�ڼ���һ��acdbOpenObject���������Ժ���Ҫ�رյġ�
02. Ĭ�Ͽ��������=����¼��*Model_Space��*Paper_Space��*Paper_Space0�ֱ��
    ��ģ�Ϳռ䡢ͼֽ�ռ��еġ�����1���͡�����2��
03. ��Ķ���,����һ������¼,����¼ �� ���� ID�� ������һ�µ�. ����������ģ�Ϳռ��Ҳ����
04. ��������ĺܶຯ������ģ�Ϳռ䶼������,
05. ����ı�������������Ϊ����һ���ռ�(��)�ı�׼����
06. AcDbBlockTableRecord == �鶨��*/

//int              MD2010_CreateBlockDefine_old(const WCHAR* blockname, AcDbObjectIdArray &vids, AcGePoint3d ori, AcDbObjectId& blkDefId);
int              MD2010_CreateBlockDefine_ExistEnts(const WCHAR* blockname, AcDbObjectIdArray &vids, AcGePoint3d ori, AcDbObjectId& blkDefId);
bool             MD2010_CheckBlockDefExist(const WCHAR * blockname);
AcDbObjectId     MD2010_GetBlockDefID(const WCHAR * blockname);
int              MD2010_InsertBlockReference_ModelSpace(const WCHAR * blockname, AcDbObjectId &id, AcGePoint3d origin = AcGePoint3d(0,0,0), double angle = 0, AcGeScale3d scale = AcGeScale3d(1,1,1));
int              MD2010_CycleBlockEntites(const WCHAR * blockname, vAcDbObjectId &vids);
HBITMAP          MD2010_BlockIconToBMP(const AcDbObjectId &blkId, HDC hdc);
int              MD2010_CycleAllBlockDefs(vAcDbObjectId &vids, vCString &vstr);
int              MD2010_GetBlockDefReferences(const WCHAR * blockname, vAcDbObjectId &vids);
bool             MD2010_CheckBlockHasAttribute(const WCHAR * blockname);

AcDbObjectId     MD2010_InsertBlockDefineFromPathName(const WCHAR *pathname,CString blockName);
                 //��׼ͼ����뺯��
int              MD2010_InsertBlockFromPathName(const WCHAR *layoutname,const WCHAR *pathname, CString blockDefineName,  AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, const WCHAR *layername = L"0", int color = -2);
int              MD2010_InsertBlockReference_Layout(const WCHAR * layoutname, const WCHAR * blockname, AcDbObjectId &entId, AcGePoint3d origin, double angle = 0, AcGeScale3d scale=AcGeScale3d(1), int color = -2);
void             MD2010_GetBlockRecordname(AcDbObjectId id, ACHAR *&pName);
int             MD2010_GetBlockReference_Record_name(AcDbObjectId id, ACHAR *&pName);
//int MD2010_GetBlock4Points();



//�ӿڲ���

/*ע���ӿڷ�Ϊ����,��һ���Ƿ��ű���ӿ�,AcDbViewportTableRecord,��һ��ר�ŵķ��ű��¼,רָģ�Ϳռ��ӿ�
�ڶ�����ͼֽ�ռ��ӿ�AcDbViewport,����һ����ͨ��acdbentity,�ͱ��ʵ��һ������ӷ���,��������רָ�ڶ���*/
AcDbObjectId     MD2010_CreateViewport(const WCHAR *layoutname, AcGePoint3d center, double w, double h);
int              MD2010_SetCurrentViewport(const WCHAR *layoutname,AcDbObjectId viewid );
int              MD2010_GetViewportIDs(const WCHAR *layoutname,vAcDbObjectId &vids );
                 //should call delete AcDbAnnotationScale after this
int              MD2010_SetViewportViewCenter(AcDbObjectId id, AcGePoint2d  p2d);
int              MD2010_SetViewportStandardScale(AcDbObjectId id, AcDbViewport::StandardScaleType scale);
int              MD2010_SetViewportCustormScale(AcDbObjectId id, double scale);
int              MD2010_AddAnnotationScale(const WCHAR *scalename, double punits, double drawunits);
AcDbAnnotationScale *MD2010_GetAnnotationScale(const WCHAR *scalename);
int              MD2010_SetViewportAnnotationScale(const WCHAR *scalename, AcDbObjectId viewportid/*, const WCHAR *layoutname = NULL*/);//����
int              MD2010_SetModelSpaceAnnotationScale(const WCHAR *scalename);
int MD2010_SetViewportUnFreeLayer(AcDbObjectId viewportid, vAcDbObjectId ids);
int MD2010_SetViewportFreeLayer(AcDbObjectId viewportid, vAcDbObjectId ids);
int MD2010_SetViewportLock(AcDbObjectId viewportid, bool lock);

/*01.ע��ʵ���಻������name û��setname����
02.record �඼��setname����*/


//�������,
AcDbObjectId MD2010_AddTextToPaperSpace(const WCHAR *layoutname, const WCHAR *text, double height,
										Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d center);
AcDbObjectId MD2010_AddTextToPaperSpace_LEFT(const WCHAR *layoutname, const WCHAR *text, double height,
										Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d left);


AcDbObjectId MD2010_GetTextStyler(const WCHAR *stylername);
AcDbObjectId MD2010_GetDimStyler(const WCHAR *stylername);
int MD2010_GetMTextPosition(AcDbObjectId entId, AcGePoint3d &pt);
int MD2010_GetTextPosition(AcDbObjectId entId, AcGePoint3d &pt);
CString MD2010_GetTextString(AcDbObjectId entId);
int MD2010_GetTextString(AcDbObjectId entId, CString& str);
CString MD2010_GetMTextString(AcDbObjectId entId);
int MD2010_GetMTextString(AcDbObjectId entId, CString &str);

AcDbObjectId MD2010_AddMTextToPaperSpace(const WCHAR *layoutname, const WCHAR *text, double height,
										 Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d center);

AcDbObjectId MD2010_AddMTextToPaperSpace_LEFTUP(const WCHAR *layoutname, const WCHAR *text, double height,
												Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d left, double width = 0.0);

AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLEMID(const WCHAR *layoutname, const WCHAR *text, double height,
												   Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width=0.0);
AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLELEFT(const WCHAR *layoutname, const WCHAR *text, double height,
                                                   Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width=0.0);
AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLERIGHT(const WCHAR *layoutname, const WCHAR *text, double height,
													Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width=0.0);

//���
AcDbObjectId MD2010_AddHatchPoints(vAcGePoint3d &pnts, const ACHAR* entryName = ACDB_MODEL_SPACE); 
AcDbObjectId MD2010_AddHatchPoints_newwall(vAcGePoint3d &pnts, const ACHAR* entryName = ACDB_MODEL_SPACE);
AcDbObjectId MD2010_AddHatchRectangle(AcGePoint3d lb, AcGePoint3d rt, const ACHAR* entryName = ACDB_MODEL_SPACE); 
AcDbObjectId MD2010_AddHatchRectangle_newwall(AcGePoint3d lb, AcGePoint3d rt, const ACHAR* entryName= ACDB_MODEL_SPACE);
AcDbObjectId MD2010_AddHatchRectangle(AcGePoint3d a, AcGePoint3d b,AcGePoint3d c, AcGePoint3d d, const ACHAR* entryName = ACDB_MODEL_SPACE);

typedef struct SHatchLines
{
	AcGeVoidPointerArray edgeptrAry;
	AcGeIntArray edgeTypesAry;
	AcGePoint2dArray vertices;  
	AcGeDoubleArray bulges;
	int isPline;
} SHatchLines, *pSHatchLines;
typedef vector<SHatchLines> vSHatchLines;
typedef vector<vSHatchLines> vvSHatchLines;
        

//���²���
void MD2010_UpdateOneEntity(AcDbObjectId entId);

//��ע���Ͳ���
//��һ����ע��ʽ������,�õ�ID
AcDbObjectId MD2010_GetDimstylerID(CString dimname);
int MD2010_SetCurrentDimStyler(CString name);
int MD2010_SetCurrentDimStyler(AcDbObjectId id);

//���´���һ������
int MD2010_GetHatchLines(AcDbObjectId Id, vSHatchLines &outs);
int MD2010_GetHatchLinesPts(vSHatchLines &outs, vAcGePoint3d &pts);

//ѡ��
int MD2012_Select1(const ACHAR *str, vAcDbObjectId &ids);

//��������
AcDbObjectId MD2011_CopyEntity_ToLayer(AcDbObjectId id, const WCHAR *lyername);
bool MD2012_CheckIsCurve(AcDbObjectId id, AcGePoint3d *startpt = NULL, AcGePoint3d *endpt = NULL);
bool MD2012_CheckIsPolyline(AcDbObjectId id, vAcGePoint3d *pts=NULL);

void testfunc();

bool CreateThumbnailBmp(CString p_sDwgFilePath, CString p_sBmpFilePath);
