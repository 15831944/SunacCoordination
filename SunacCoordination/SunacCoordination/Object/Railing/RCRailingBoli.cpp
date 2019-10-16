#include "StdAfx.h"
#include "RCRailingBoli.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"


bool CRCRailingBoli::GenRailing()
{
	if (GetLength() < 300)
	{
		return false;    //�������ܳ������жϣ���������ܳ�С��1550������false
	}

	m_B = GenStandardRailingLen(GetLength());
	m_N = GenStandardSegCount(GetLength());
	//m_H = 150 * (m_N - 1);

	return true;
}

AcDbObjectId CRCRailingBoli::CreateRailingBlockDefine(CString sRailingDefName)
{
	AcGePoint3d start = AcGePoint3d::kOrigin;
	//2 ���뵽ͼ��
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + m_railingAtt.m_height, 0); //������������Ͻǵ�
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const double centerY = leftTopPt.y - GetHandRailHeight() - railH / 2;


	AcDbObjectIdArray idsOut;
	//2.1 ����
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), centerY, 0); //���Ͻǵ�x�����ϼ�ȥ��ṹǽ��϶��y�����ϼ�ȥ���ֵĺ��,Ȼ���Ǿ���λ��


	//2.2 ��׼��
	AcDbObjectId id2;
	AcDbObjectIdArray ids = GenerateRailing_Standard(pos1);
	idsOut.append(ids);


	//2.4 ����
	AcDbObjectId id4 = GenerateRailing_HandRail(leftTopPt);
	idsOut.append(id4);

	//////////////////////////////////////////////////////////////////////////
	//3 ���Ϊһ����
	AcDbObjectId blkDefId;
	int nRet = MD2010_CreateBlockDefine_ExistEnts(sRailingDefName, idsOut, start, blkDefId);

	//ɾ��ԭ���Ŀ�
	JHCOM_DeleteCadObjectArray(idsOut);

	//4 ���������Ը�ֵ��������ʵ��
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	pAttRailing->SetInstanceCode(sRailingDefName);
	TY_AddAttributeData(blkDefId, pAttRailing);

	acDocManager->unlockDocument(curDoc());

	return blkDefId;
}
CString CRCRailingBoli::GetStandardBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Standard");
}

CString CRCRailingBoli::GetLeftBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Left");
}
CString CRCRailingBoli::GetRightBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Right");
}
CString CRCRailingBoli::GetHandRailBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Handrail");
}

AcDbObjectIdArray CRCRailingBoli::GenerateRailing_Standard(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sStandardBlockName = GetStandardBlockName();

	AcDbObjectIdArray idsOut;
	AcDbObjectId id2;
	for (int i = 0; i < GetN(); i++)
	{
		id2 = InsertBlockRefFromDwg(fileName, sStandardBlockName, ACDB_MODEL_SPACE, pos);
		assert(id2 != AcDbObjectId::kNull);

		DQ_SetDynamicAttribute(id2, _T("H"), railH);
		DQ_SetDynamicAttribute(id2, _T("L"), GetB());

		if (i==0 && (i==GetN()-1))
		{
			DQ_SetDynamicAttribute(id2, _T("�ɼ���"), _T("����"));
		}
		else if (i == 0)
		{
			DQ_SetDynamicAttribute(id2, _T("�ɼ���"), _T("���"));
		}
		else if ((i == GetN() - 1))
		{
			DQ_SetDynamicAttribute(id2, _T("�ɼ���"), _T("�Ҳ�"));
		}
		else
		{
			DQ_SetDynamicAttribute(id2, _T("�ɼ���"), _T("����"));
		}


		idsOut.append(id2);

		pos.x += GetB() + GetH(); 
	}

	return idsOut;
}

AcDbObjectId CRCRailingBoli::GenerateRailing_HandRail(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sHandrailBlockName = GetHandRailBlockName();

	AcDbObjectId id4;
	id4 = InsertBlockRefFromDwg(fileName, sHandrailBlockName, ACDB_MODEL_SPACE, pos);
	assert(id4 != AcDbObjectId::kNull);
	//���÷��ֳ���
	DQ_SetDynamicAttribute(id4, _T("L"), GetLength());
	DQ_SetDynamicAttribute(id4, _T("H"), railH);

	return id4;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingB1::CRCRailingB1()
{
	m_K = 137.5;
}

CRCRailingB1::~CRCRailingB1()
{
}



int CRCRailingB1::GenStandardSegCount(double p_lenth)const	//�����׼��������
{
	return int((p_lenth - 125) / 1650) + 1;
}

double CRCRailingB1::GenStandardRailingLen(double p_lenth)const				//�����׼���˳ߴ�
{
	int count = GenStandardSegCount(p_lenth);
	return (p_lenth - 275 - (150 * (count - 1))) / (double)count;
}


////////////////////////////////////////////////////////////////////////

CRCRailingB2::CRCRailingB2()
{
}
CRCRailingB2::~CRCRailingB2()
{
}

int CRCRailingB2::GenStandardSegCount(double p_lenth)const	//�����׼��������
{
	return int((p_lenth - 275) / 1500) + 1;
}

double CRCRailingB2::GenStandardRailingLen(double p_lenth)const				//�����׼���˳ߴ�
{
	int count = GenStandardSegCount(p_lenth);
	return (double)(p_lenth - 275) / count;
}

////////////////////////////////////////////////////////////////////////

CRCRailingB4::CRCRailingB4()
{
}
CRCRailingB4::~CRCRailingB4()
{
}

int CRCRailingB4::GenStandardSegCount(double p_lenth)const	//�����׼��������
{
	return int((p_lenth - 150) / 1500) + 1;
}

double CRCRailingB4::GenStandardRailingLen(double p_lenth)const				//�����׼���˳ߴ�
{
	int count = GenStandardSegCount(p_lenth);
	return (double)(p_lenth - 150) / count;
}