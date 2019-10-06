/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCLanGan.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCRailingTieyi.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"

//////////////////////////////////////////////////////////////////////////
CRCRailingTieyi::CRCRailingTieyi()
{
}

CRCRailingTieyi::~CRCRailingTieyi()
{
}


//////////////////////////////////////////////////////////////////////////

CRCRailingT1::CRCRailingT1()
{
}

//start Ϊ���˵����½�
int CRCRailingT1::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	//1. ������ֶε�ֵ
	bool bSuc = GenRailing();
	if (bSuc==false)
		return -1;

	//2 ���뵽ͼ��
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + m_railingAtt.m_height, 0); //������������Ͻǵ�
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const double centerY = leftTopPt.y - GetHandRailHeight() - railH / 2;
	m_railingAtt.m_fileName = MD2010_GetAppPath() + L"\\support\\Sunac2019\\LocalMode\\" + m_railingAtt.m_prototypeCode + L".dwg";

	AcDbObjectIdArray idsOut;
	//2.1 �Ǳ��
	AcDbObjectId id1;
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), centerY, 0); //���Ͻǵ�x�����ϼ�ȥ��ṹǽ��϶��y�����ϼ�ȥ���ֵĺ��,Ȼ���Ǿ���λ��
	id1 = InsertBlockRefFromDwg(m_railingAtt.m_fileName, _T("Railing_T1_NonStandard"), ACDB_MODEL_SPACE, pos1);

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id1, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2); //�ڲ���С��������Ϊ�Ǳ�μ�ȥ��������
	DQ_SetDynamicAttribute(id1, _T("H"), railH);
	
	idsOut.append(id1);

	//2.2 ��׼��
	AcDbObjectId id2;
	AcGePoint3d pos2 = pos1;
	pos2.x = pos1.x + GetNonstandardLen() - GetPillarWidth();
	CString sStandardBlockName = GetB() < 1380 ? _T("Railing_T1_1260") : _T("Railing_T1_1380");
	for (int i = 0; i < GetN(); i++)
	{
		id2 = InsertBlockRefFromDwg(m_railingAtt.m_fileName, sStandardBlockName, ACDB_MODEL_SPACE, pos2);
		//MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_fileName, sStandardBlockName, id2, pos2, 0, AcGeScale3d(1));
		DQ_SetDynamicAttribute(id2, _T("H"), railH);

		idsOut.append(id2);

		pos2.x += GetB() - GetPillarWidth(); //��ȥ������
	}


	//2.3 �Ǳ��
	AcDbObjectId id3;
	AcGePoint3d pos3 = pos2;
	id3 = InsertBlockRefFromDwg(m_railingAtt.m_fileName, _T("Railing_T1_NonStandard"), ACDB_MODEL_SPACE, pos3);
	//MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_fileName, _T("Railing_T1_NonStandard"), id3, pos3, 0, AcGeScale3d(1));
	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id3, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id3, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2); //�ڲ���С��������Ϊ�Ǳ�μ�ȥ��������
	DQ_SetDynamicAttribute(id3, _T("H"), railH);
	idsOut.append(id3);

	//2.4 ����
	AcDbObjectId id4;
	id4 = InsertBlockRefFromDwg(m_railingAtt.m_fileName, _T("Railing_T1_Handrail"), ACDB_MODEL_SPACE, leftTopPt);
	//MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_fileName, _T("Railing_T1_Handrail"), id4, leftTopPt, 0, AcGeScale3d(1));
	//���÷��ֳ���
	DQ_SetDynamicAttribute(id4, _T("L"), GetLength());
	DQ_SetDynamicAttribute(id4, _T("H"), railH);
	idsOut.append(id4);

	//////////////////////////////////////////////////////////////////////////
	//3 ���Ϊһ���飬���������������ǰ��  TODO

	acDocManager->unlockDocument(curDoc());

	return 0;
}


bool CRCRailingT1::GenRailing()  //�������ܳ������жϣ���������ܳ�С��1550������false
{
	if (GetLength() < 1550)
	{
		return false;    //������㰴ť�����judge()����false,���û��ڶԻ���������ܳߴ粻����Ҫ��Ӧ�����Ի�����ʾ�û���������
	}

	//�ȼ���1260��׼�ε�����
	int N_1 = GenStandardSegCount(GetLength(), 1260);
	int n_1 = GenNonstandardUnitCount(GetLength(), 1260, N_1);
	double k1 = GenK(GetLength(), 1260, N_1, n_1);

	//����1380��׼�ε�����
	int N_2 = GenStandardSegCount(GetLength(), 1380);
	int n_2 = GenNonstandardUnitCount(GetLength(), 1380, N_2);
	double k2 = GenK(GetLength(), 1380, N_2, n_2);

	//��k(���˲�����ռ�϶)�����жϣ��������105����ô��׼���˳ߴ��Ϊ1380�������Ϊ1260
	if (k1 <= 105)
	{
		m_B = 1260;
		m_N = N_1;
		m_n = n_1;
		m_K = k1;
	}
	else
	{
		m_B = 1380;
		m_N = N_2;
		m_n = n_2;
		m_K = k2;
	}
	return true;
}

int CRCRailingT1::GenStandardSegCount(double p_lenth, double p_segLength)const		//�����׼����������p_lenthΪ���˳���p_segLengthΪ���˵ı�׼�γ�
{
	int nCount = 0;
	if ((int)(p_lenth / p_segLength == 1))
	{
		nCount = 1;
	}
	else if ((int)(p_lenth / p_segLength) - 1 < 0)
	{
		nCount = 0;
	}
	else
	{
		nCount = (int)(p_lenth / p_segLength - 1);
	}

	return nCount;
}

//��ȡ��׼�������൥Ԫ��������
int CRCRailingT1::GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)	const
{
	//��׼�������൥Ԫ�������� = �������ܳߴ� - ��׼���˳ߴ� - H(�����ռ�ߴ�)��/ ��Ԫ����ߴ�
	int n = (int)((GetLength() - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - GetH()) / Getb());

	//��������Ҫ��2�ı�����������ǣ�����ȡ��ӽ���2�ı��� (��-1)
	if ((((int)(n)) % 2) != 0)
	{
		n--;
	}

	return n;
}

double CRCRailingT1::GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const
{
	//ͨ����׼���˳ߴ�������������׼�����ܳߴ�,��ʽ����׼�����ܳߴ� = ��׼���˳ߴ�*��׼��������- 40*(��������-1)
	return p_segLength *p_standardSegCount - GetPillarWidth() * (p_standardSegCount - 1);
}

double CRCRailingT1::GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount) const//������ռ�϶ = ���������ͽṹǽ��ߴ�/2
{
	double nonStandardLen = p_nonStandardUnitCount * Getb(); //�Ǳ�׼�εĵ�Ԫ�����ܳ���
	double k = (p_lenth - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - nonStandardLen - GetH()) / 2;
	return k;
}

//////////////////////////////////////////////////////////////////////////

double CRCRailingT1::GetStandardRailingTotalLen()const
{
	return GenStandardRailingTotalLen(m_B, m_N);
}
double CRCRailingT1::GetNonstandardLen()const   //��ȡ�Ǳ�����˳��ȣ�����������
{
	//�����ܳߴ� = ��������/2*����ߴ�   /2��ֻ����һ��ĳ���
	double nonStandardLen = (m_n/2) * Getb() + (Getb()-GetSmallPillarWidth()) + GetPillarWidth() * 2; //����һ�������϶����������
	return nonStandardLen;
}


//////////////////////////////////////////////////////////////////////////

