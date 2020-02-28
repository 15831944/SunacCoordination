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
#include "../../Common/ComFun_Layer.h"
#include "../../Tool/DocLock.h"
#include "RCRailingTieyi.h"
#include "RCRailingBoli.h"
#include "../../Common/ComFun_Interactive.h"
 
CRCRailing::CRCRailing(void)
{
	m_B = 1260;			//��׼���˳ߴ� 1260����1380
	m_N = 1;			//��׼�������� 
	m_K = 0;

	m_bSimple = true;
}
 
CRCRailing::~CRCRailing(void)
{

}

void CRCRailing::SetRailingAtt(const AttrRailing p_railingAtt)
{
	m_railingAtt = p_railingAtt;
}
bool CRCRailing::SetLength(double p_length) //���������ܳ��ȣ�����(�����ܳ���)������������m_length���ܳߴ磩������GenRailing();
{
	m_railingAtt.m_length = p_length;
	return GenRailing(); //����GenRailing()�����������ܳ������жϣ���������ܳ�С��1550������false
}
CString CRCRailing::GetPrototypeFilePath()const
{
	return TY_GetPrototypeFilePath_Local() + m_railingAtt.m_prototypeCode + L".dwg";
}
bool CRCRailing::CheckLength()
{
	return (m_railingAtt.m_length >= GetMinWidth());
}
bool CRCRailing::CheckHeight()
{
	return (m_railingAtt.m_height >= GetMinHeight());
}
//start Ϊ���˵����½�
int CRCRailing::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	if (CheckLength()==false || CheckHeight()==false)
	{
		return -1;
	}

	//1. ������ֶε�ֵ
	bool bSuc = GenRailing();
	if (bSuc == false)
		return -1;

	CDocLock docLock; //�����ĵ��ã���ɾ

	CString sRailingDefName = m_railingAtt.GetInstanceCode();
	if (sRailingDefName.IsEmpty())
	{
		assert(false);
		sRailingDefName.Format(_T("%s_%d_%d"), m_railingAtt.m_prototypeCode, (int)(m_railingAtt.m_length), (int)(m_railingAtt.m_height));
	}

	CString oldLayerName;
	MD2010_GetCurrentLayer(oldLayerName);
	CString sRalingLayerName = _T("Sunac_Railing");

	if (!IsSampleDraw())
	{
		sRalingLayerName = _T("Sunac_Railing_Detail");
		sRailingDefName.Format(_T("%s_%d_%d_%s"), m_railingAtt.m_prototypeCode, (int)(m_railingAtt.m_length), (int)(m_railingAtt.m_height), _T("Detail"));
	}
	
	if (JHCOM_GetLayerID(sRalingLayerName)==AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sRalingLayerName);
	}
	MD2010_SetCurrentLayer(sRalingLayerName);

	//��֮ǰ�У���ֱ�Ӳ���
	AcDbObjectId railingBlockDef = MD2010_GetBlockDefID(sRailingDefName);
	if (railingBlockDef == AcDbObjectId::kNull)
	{
		AcDbObjectId blkDefId = CreateRailingBlockDefine(sRailingDefName);
	}

	MD2010_InsertBlockReference_ModelSpace(sRailingDefName, p_railingIdOut, start);
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	TY_AddAttributeData(p_railingIdOut, pAttRailing);
	pAttRailing->close();
	MD2010_SetCurrentLayer(oldLayerName);

	//���USC���괦��
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(p_railingIdOut, mat);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CRCRailing* CreateRailing(const AttrRailing p_railingAtt)
{
	CRCRailing* pRailing = NULL;
	switch (p_railingAtt.m_railingType)
	{
	case E_RAILING_TIEYI:
	{
		if (p_railingAtt.m_prototypeCode.Find(_T("T1")) >= 0 ||
			p_railingAtt.m_prototypeCode.Find(_T("T2")) >= 0)
		{
			pRailing = new CRCRailingT1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T3")) >= 0)
		{
			pRailing = new CRCRailingT3();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T4")) >= 0)
		{
			pRailing = new CRCRailingT4();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T5")) >= 0)
		{
			pRailing = new CRCRailingT5();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T6")) >= 0)
		{
			pRailing = new CRCRailingT6();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T7")) >= 0)
		{
			pRailing = new CRCRailingT7();
		}
	}
		break;
	case E_RAILING_BOLI:

		if (p_railingAtt.m_prototypeCode.Find(_T("B1")) >= 0)
		{
			pRailing = new CRCRailingB1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B2")) >= 0 )
		{
			pRailing = new CRCRailingB2();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B3_1")) >= 0)
		{
			pRailing = new CRCRailingB3_1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B3_2")) >= 0)
		{
			pRailing = new CRCRailingB3_2();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B4")) >= 0)
		{
			pRailing = new CRCRailingB4();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B5")) >= 0 )
		{
			pRailing = new CRCRailingB5();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B6")) >= 0)
		{
			pRailing = new CRCRailingB6();
		}

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

void CRCRailing::CreateRailingTop(AcGePoint3d p_pnt1, AcGePoint3d p_pnt2)
{
	CDocLock lockEnt;

	CString sRailingDefName = m_railingAtt.GetInstanceCode();

	int nRailingTopLength = 0;
	bool bSul = GenerateInsertPt(p_pnt1, p_pnt2, nRailingTopLength);
	if (bSul == false)
		return;

	sRailingDefName.Format(_T("%s_%d_%d_P"), m_railingAtt.m_prototypeCode, nRailingTopLength, (int)(m_railingAtt.m_height));

	CString oldLayerName;
	MD2010_GetCurrentLayer(oldLayerName);
	CString sRalingLayerName = _T("Sunac_Railing");

	if (JHCOM_GetLayerID(sRalingLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sRalingLayerName);
	}
	MD2010_SetCurrentLayer(sRalingLayerName);

	AcDbObjectId railingTopId = GenerateRailingTop(sRailingDefName, p_pnt1, p_pnt2);

	MD2010_SetCurrentLayer(oldLayerName);

	// ���������ӵ�ģ�Ϳռ�
	MD2010_InsertBlockReference_ModelSpace(sRailingDefName, railingTopId, p_pnt1);
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	TY_AddAttributeData(railingTopId, pAttRailing);
	pAttRailing->close();
}

AcDbObjectId CRCRailing::GenerateRailingTop(CString p_blockName, AcGePoint3d p_pnt1, AcGePoint3d p_pnt2)
{
	// ��õ�ǰͼ�����ݿ�Ŀ��
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);

	// �����µĿ���¼
	AcDbBlockTableRecord *pBlkTblRcd = new AcDbBlockTableRecord();

	if (pBlkTbl->has(p_blockName))
	{
		pBlkTblRcd->setName(p_blockName + L"_1");
	}
	else
	{
		pBlkTblRcd->setName(p_blockName);
	}

	// ������¼��ӵ������
	AcDbObjectId blkDefId;
	pBlkTbl->add(blkDefId, pBlkTblRcd);
	pBlkTbl->close();

	AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;
	
	if (p_pnt1.y == p_pnt2.y)
	{
		ptLeftBottom.set(p_pnt1.x, p_pnt1.y - 50);
		ptRightBottom.set(p_pnt2.x, p_pnt2.y - 50);
		ptRightTop.set(p_pnt2.x, p_pnt2.y + 50);
		ptLeftTop.set(p_pnt1.x, p_pnt1.y + 50);
	}
	else if (p_pnt1.x == p_pnt2.x)
	{
		ptLeftBottom.set(p_pnt1.x - 50, p_pnt1.y);
		ptRightBottom.set(p_pnt1.x + 50, p_pnt1.y);
		ptRightTop.set(p_pnt2.x + 50, p_pnt2.y);
		ptLeftTop.set(p_pnt2.x - 50, p_pnt2.y);
	}
	else
		return false;

	// ������Ӧ�Ķ����
	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(1, ptRightBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(2, ptRightTop, 0, 0.1, 0.1);
	pPoly->addVertexAt(3, ptLeftTop, 0, 0.1, 0.1);
	pPoly->setClosed(Adesk::kTrue);

	pPoly->setColorIndex(1);

	AcDbObjectId entId;
	pBlkTblRcd->appendAcDbEntity(entId, pPoly);
	pBlkTblRcd->setOrigin(p_pnt1);

	pPoly->close();
	pBlkTblRcd->close();

	return entId;
}

bool CRCRailing::GenerateInsertPt(AcGePoint3d& p_pnt1, AcGePoint3d& p_pnt2, int& p_width)
{
	int nError = 20; //ѡ��������ļ�����
	bool bSul = true;

	if (fabs(p_pnt2.x - p_pnt1.x) > nError && fabs(p_pnt2.y - p_pnt1.y) > nError)
	{
		AfxMessageBox(_T("ѡ��������㾡��������ͬһˮƽ�߻�ֱ����\n"));
		bSul = false;
	}
	else if (fabs(p_pnt2.y - p_pnt1.y) <= nError && fabs(p_pnt2.x - p_pnt1.x) > nError)
	{
		p_pnt2.y = p_pnt1.y;
		p_width = (int)fabs(p_pnt2.x - p_pnt1.x);
		GenerateRailingLength(p_width);
		if (p_pnt2.x - p_pnt1.x > 0)
			p_pnt2.x = p_pnt1.x + p_width;
		else
			p_pnt2.x = p_pnt1.x - p_width;
	}
	else if (fabs(p_pnt2.x - p_pnt1.x) <= nError && fabs(p_pnt2.y - p_pnt1.y) > nError)
	{
		p_pnt2.x = p_pnt1.x;
		p_width = (int)fabs(p_pnt2.y - p_pnt1.y);
		GenerateRailingLength(p_width);
		if (p_pnt2.y - p_pnt1.y > 0)
			p_pnt2.y = p_pnt1.y + p_width;
		else
			p_pnt2.y = p_pnt1.y - p_width;
	}
	else
	{
		AfxMessageBox(_T("ѡ������������С��������ѡ��\n"));
		bSul = false;
	}

	return bSul;
}

void CRCRailing::GenerateRailingLength(int& p_width)
{
	int nRemainder = p_width % 10;
	if (nRemainder % 10 < 5)
	{
		p_width -= nRemainder;
	}
	else
	{
		p_width += 10 - nRemainder;
	}
}

//////////////////////////////////////////////////////////////////////////
//������ͼ
void CRCRailing::DrawRailingDetail()
{
	CDocLock lockEnt;

	//��һ����ѡ����Ҫͳ�Ƶ�����
	vAcDbObjectId RailingIds;
	JHCOM_SelectEnts(RailingIds);
	if (RailingIds.size() == 0)
		return;

	//�ڶ���  ѡ��������ͼ�����
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"��ѡ��������ͼ�����");
	if (bSuc == false)
		return;

	//����������ȡ�������ݲ��ҷ������
	CRailingCountArray railCountArray;
	railCountArray.InitByRailingIds(RailingIds);

	////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < railCountArray.GetCount(); i++)
	{
		const AttrRailing& railAtt = railCountArray.GetRailing(i).railAtt;
		CRCRailing* pRailing = CreateRailing(railAtt);
		pRailing->SetSimpleDraw(false);
		pRailing->GenerateRailing(pnt, RailingIds[i]);
		pnt.set(pnt.x, pnt.y+ 1800, 0);
	}

	return;
}

