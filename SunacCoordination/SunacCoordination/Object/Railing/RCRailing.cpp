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
#include "RCRailing.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
 
CRCRailing::CRCRailing(void)
{
	m_B = 1260;			//��׼���˳ߴ� 1260����1380
	m_N = 1;			//��׼�������� 
	m_n = 1;			//��׼�������൥Ԫ��������
	m_K = 0;
}
 
CRCRailing::~CRCRailing(void)
{

}

//CRCRailing::CRCRailing(const CRCRailing &other):RCStandBlock(other)
//{
//	*this = other;
//}
//
//CRCRailing& CRCRailing::operator=(const CRCRailing &rhs)
//{
//	m_B = rhs.m_B;			//��׼���˳ߴ� 1260����1380
//	m_N = rhs.m_B;			//��׼���˶����� 
//	m_n = rhs.m_B;			//��׼�������൥Ԫ�������������ࣩ
//	m_K = rhs.m_B;			//���������ṹǽ��ߴ磨���ߣ�
//
//	m_railingAtt = rhs.m_railingAtt;
//
//	return *this;
//}

void CRCRailing::SetRailingAtt(const AttrRailing p_railingAtt)
{
	m_railingAtt = p_railingAtt;
}
bool CRCRailing::SetLength(double p_length) //���������ܳ��ȣ�����(�����ܳ���)������������m_length���ܳߴ磩������GenRailing();
{
	m_railingAtt.m_length = p_length;
	return GenRailing(); //����GenRailing()�����������ܳ������жϣ���������ܳ�С��1550������false
}

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
int CRCRailingT1::GenerateRailing(AcGePoint3d start, AcGePoint3d end)
{
	//1. ���ó��ȣ���������ֶε�ֵ
	double length = (end - start).length();
	SetLength(length);

	//2 ���뵽ͼ��
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + 1100, 0);

	//2.1 �Ǳ��
	AcDbObjectId id1;
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), leftTopPt.y - GetHandRailHeight(), 0); //���Ͻǵ�x�����ϼ�ȥ��ṹǽ��϶��y�����ϼ�ȥ���ֵĺ��
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_NonStandard"), id1, pos1, 0, AcGeScale3d(1));

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id1, _T("Ln"), GetNonstandardLen()-GetPillarWidth()*2); //�ڲ���С��������Ϊ�Ǳ�μ�ȥ��������

	//2.2 ��׼��
	AcDbObjectId id2;
	AcGePoint3d pos2 = pos1;
	pos2.x = pos1.x + GetNonstandardLen() - GetPillarWidth();
	CString sStandardBlockName = GetB() < 1380 ? _T("Railing_T1_1260") : _T("Railing_T1_1380");
	for (int i = 0; i < GetN(); i++)
	{
		MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, sStandardBlockName, id2, pos2, 0, AcGeScale3d(1));
		pos2.x += GetB() - GetPillarWidth(); //��ȥ������
	}


	//2.3 �Ǳ��
	AcDbObjectId id3;
	AcGePoint3d pos3 = pos2;
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_NonStandard"), id3, pos3, 0, AcGeScale3d(1));
	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id3, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id3, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2); //�ڲ���С��������Ϊ�Ǳ�μ�ȥ��������

	//2.4 ����
	AcDbObjectId id4;
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_Handrail"), id1, leftTopPt, 0, AcGeScale3d(1));
	//���÷��ֳ���
	DQ_SetDynamicAttribute(id4, _T("L"), length);


	acDocManager->unlockDocument(curDoc());

	return 0;
}


bool CRCRailingT1::GenRailing()  //�������ܳ������жϣ���������ܳ�С��1550������false
{
	if (GetLenght() < 1550)
	{
		return false;    //������㰴ť�����judge()����false,���û��ڶԻ���������ܳߴ粻����Ҫ��Ӧ�����Ի�����ʾ�û���������
	}

	//�ȼ���1260��׼�ε�����
	int N_1 = GenStandardSegCount(GetLenght(), 1260);
	int n_1 = GenNonstandardUnitCount(GetLenght(), 1260, N_1);
	double k1 = GenK(GetLenght(), 1260, N_1, n_1);

	//����1380��׼�ε�����
	int N_2 = GenStandardSegCount(GetLenght(), 1380);
	int n_2 = GenNonstandardUnitCount(GetLenght(), 1380, N_2);
	double k2 = GenK(GetLenght(), 1380, N_2, n_2);

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
	int n = (int)((GetLenght() - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - GetH()) / Getb());

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
	return p_segLength *p_standardSegCount - GetSmallPillarWidth() * (p_standardSegCount - 1);
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
CRCRailingBoli::CRCRailingBoli()
{
}

CRCRailingBoli::~CRCRailingBoli()
{
}




//////////////////////////////////////////////////////////////////////////

CRCRailing* CreateRailing(const AttrRailing p_railingAtt)
{
	CRCRailing* pRailing = NULL;
	switch (p_railingAtt.m_railingType)
	{
	case E_RAILING_TIEYI:
	{
							if (p_railingAtt.m_prototypeCode.Find(_T("T1")) >= 0)
							{
								pRailing = new CRCRailingT1();
							}
	}
		break;
	case E_RAILING_BOLI://TODO
		break;
	default:
		break;
	}

	if (pRailing!=NULL)
	{
		pRailing->SetRailingAtt(p_railingAtt);
	}

	return pRailing; 
}

