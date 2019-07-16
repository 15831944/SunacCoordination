#pragma once

#include <dbents.h>


/*-----------------------------����ϵ��-------------------*/
bool vfind(AcDbObjectId &id, vAcDbObjectId &ids);
bool vfind(vAcDbObjectId &ids, AcDbObjectId &id, int &index);
bool vfind(vCString &strs, CString &str, int &index);
bool vfind(CString &str, vCString &strs);
bool vfind(AcGePoint3d &pt, vAcGePoint3d &pts, double tol = TOL);
bool vfind(double &pt, vdouble &pts, double tol = TOL);


/*-----------------------------���㼫��ϵ��-------------------*/
//��������ڸ���һϵ�е��ҵ����� ���£� ���ң����ϵ��ĸ�����ֵ��������������
void MD2010_GetExtrmePoint(vAcGePoint3d &in, AcGePoint3d &out1, AcGePoint3d &out2);

//���������һϵ�е����ߣ�������Щ���ߵ����е㣬д������
void MD2010_GetCurvePoints(vAcDbObjectId &curveIds, vAcGePoint3d &vpoints);

//���������һ��ͼ�㣬�����ͼ���������ߵ������� �� ���ϵ㣬 ���ڵ�ƺ���ʣ��Լ��������ʱ����
void MD2010_ComputeExtrmePointsForPaper(WCHAR * layername, AcGePoint3d &lb, AcGePoint3d &rt);

//����������ڼ���������������������ǵ����ĵ�
void MD2010_ComputeExtrmeCenterForPaper(WCHAR * layername, AcGePoint3d &center);

//��Ե�������Ӧ�����Ϸ������õ����ĵ�
void MD2010_ComputeExtrmeCenterForPaper_SingleCurve(WCHAR * layername, AcDbObjectId curveId, AcGePoint3d &center);


/*-----------------------------�����ཻϵ��-------------------*/

//��һ���߶Σ�Ȼ���һЩ�е����ߣ���������߶κ���Щ���ߵ����н���
void MD2010_GetLineInstersectPoints(AcDbObjectId lineid, vAcDbObjectId &ids, vAcGePoint3d &pnts);

//��һ���߶Σ���һϵ�е����ߣ��������еĽ�����ҵ�������һ���ĵ�
bool MD2010_GetLineInstersectTwoPoints(AcDbObjectId lineid, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b);

//ͬMD2010_GetLineInstersectPoints��ָ����ʽ
void MD2010_GetLineInstersectPoints(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &pnts);

//ͬ�ϣ�ָ����ʽ
bool MD2010_GetLineInstersectTwoPoints(AcDbLine *pline, vAcDbObjectId &ids, AcGePoint3d &a, AcGePoint3d &b);

//һ��ֱ�ߺ�һϵ�е����ߣ����㽻��󣬰ѽ��㰴��X����˳������
bool MD2010_GetLineInstersect_Row(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out);

//һ��ֱ�ߺ�һϵ�е����ߣ����㽻��󣬰ѽ��㰴��Y����˳������
bool MD2010_GetLineInstersect_Coloum(AcDbLine *pline, vAcDbObjectId &ids, vAcGePoint3d &out);


/*--------------------��������---------------------------*/

//��������Ӹ�һ���㣬��������Χ�� һȦ�㣬���ڶ�ý��ͼ ������
void MD2010_GetAroundPointsFromOnePoint(AcGePoint3d &pnt, vAcGePoint3d &pnts, double d, bool twosidesflag = true);

//���е㰴��X��������
void MD2010_SortX(vAcGePoint3d &indata, vAcGePoint3d&outdata);

//���е㰴��Y��������
void MD2010_SortY(vAcGePoint3d &indata, vAcGePoint3d&outdata);

//���������һЩ�еĵ㣬������ϳ�һ����ն���ߣ������������
double MD2010_GetAreaFromPoints(vAcGePoint3d &vpoints);

//��һϵ�е����ҵ�������һ���ĸ�����
bool MD2010_GetTwoDiffPoints(vAcGePoint3d &pnts, AcGePoint3d &a, AcGePoint3d &b);

//������� ��һϵ�е��ӣ��޳��ظ��ģ��õ��ɾ���Ψһ�ĵ������
void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts);
void MD2010_RemoveCopyOfPoints(vAcGePoint3d &vpnts, double tol);

//���������һ�ѵ��еõ�X��ͬ�ĵ� ������Y
void MD2010_GetDiffPointX(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol);
//���������һ�ѵ��еõ�Y��ͬ�ĵ� ������X
void MD2010_GetDiffPointY(vAcGePoint3d &vpnts, vAcGePoint3d &opts, double tol);

