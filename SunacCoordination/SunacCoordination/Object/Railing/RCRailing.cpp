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
#include "RCRailingTieyi.h"
#include "RCRailingBoli.h"
 
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

