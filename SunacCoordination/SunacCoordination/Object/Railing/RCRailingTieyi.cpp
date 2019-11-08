
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
	m_B1 = 0;
	m_B2 = 0;

	m_n = 1;			//��׼�������൥Ԫ��������
}

CRCRailingTieyi::~CRCRailingTieyi()
{
}

CString CRCRailingTieyi::GetStandardBlockName() const
{
	CString sName;
	sName.Format(_T("%s_%d"), m_railingAtt.m_prototypeCode, (int)GetB());
	if (IsSampleDraw())
		sName += _T("_simple");
	return sName;
}

CString CRCRailingTieyi::GetNonStandardBlockName() const
{
	CString sName = m_railingAtt.m_prototypeCode + _T("_NonStandard");
	if (IsSampleDraw())
		sName += _T("_simple");

	return sName;
}
CString CRCRailingTieyi::GetHandRailBlockName() const
{
	CString sName = m_railingAtt.m_prototypeCode + _T("_Handrail");
	if (IsSampleDraw())
		sName += _T("_simple");

	return sName;
}


AcDbObjectId CRCRailingTieyi::CreateRailingBlockDefine(CString sRailingDefName)
{
	AcGePoint3d start = AcGePoint3d::kOrigin;

	//2 ���뵽ͼ��
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + m_railingAtt.m_height, 0); //������������Ͻǵ�
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const double centerY = leftTopPt.y - GetHandRailHeight() - railH / 2;


	AcDbObjectIdArray idsOut;
	//2.1 �Ǳ��
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), centerY, 0); //���Ͻǵ�x�����ϼ�ȥ��ṹǽ��϶��y�����ϼ�ȥ���ֵĺ��,Ȼ���Ǿ���λ��
	AcDbObjectId id1 = GenerateRailing_NonStandard(pos1);
	idsOut.append(id1);

	//2.2 ��׼��
	AcDbObjectId id2;
	AcGePoint3d pos2 = pos1;
	pos2.x = pos1.x + GetNonstandardLen() - GetPillarWidth();
	AcDbObjectIdArray ids = GenerateRailing_Standard(pos2);
	idsOut.append(ids);


	//2.3 �Ǳ��
	AcGePoint3d pos3 = pos2;
	pos3.x = pos2.x + GetStandardRailingTotalLen() - GetPillarWidth();
	AcDbObjectId id3 = GenerateRailing_NonStandard(pos3);
	idsOut.append(id3);

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
	pAttRailing->close();

	acDocManager->unlockDocument(curDoc());

	return blkDefId;
}

AcDbObjectId CRCRailingTieyi::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id1 = AcDbObjectId::kNull;

	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sNonStandardBlockName = GetNonStandardBlockName();


	if (MD2010_CheckBlockDefExist(sNonStandardBlockName))
	{
		MD2010_InsertBlockReference_ModelSpace(sNonStandardBlockName, id1, p_pos);
	}
	else
	{
		id1 = InsertBlockRefFromDwg(fileName, sNonStandardBlockName, ACDB_MODEL_SPACE, p_pos);
	}

	assert(id1 != AcDbObjectId::kNull);

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id1, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2);
	DQ_SetDynamicAttribute(id1, _T("H"), railH);

	return id1;
}


AcDbObjectIdArray CRCRailingTieyi::GenerateRailing_Standard(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sStandardBlockName = GetStandardBlockName();

	AcDbObjectIdArray idsOut;
	AcDbObjectId id2;
	for (int i = 0; i < GetN(); i++)
	{
		if (i == 0)
			id2 = InsertBlockRefFromDwg(fileName, sStandardBlockName, ACDB_MODEL_SPACE, pos);
		else
			MD2010_InsertBlockReference_ModelSpace(sStandardBlockName, id2, pos);
		assert(id2 != AcDbObjectId::kNull);

		DQ_SetDynamicAttribute(id2, _T("H"), railH);

		idsOut.append(id2);

		pos.x += GetB() - GetPillarWidth(); //��ȥ������
	}

	return idsOut;
}

AcDbObjectId CRCRailingTieyi::GenerateRailing_HandRail(AcGePoint3d pos)
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


bool CRCRailingTieyi::GenRailing()  //�������ܳ������жϣ���������ܳ�С��1550������false
{
	if (GetLength() < 1550)
	{
		return false;    //������㰴ť�����judge()����false,���û��ڶԻ���������ܳߴ粻����Ҫ��Ӧ�����Ի�����ʾ�û���������
	}

	//�ȼ���1260��׼�ε�����
	int N_1 = GenStandardSegCount(GetLength(), B1());
	int n_1 = GenNonstandardUnitCount(GetLength(), B1(), N_1);
	double k1 = GenK(GetLength(), B1(), N_1, n_1);

	//����1380��׼�ε�����
	int N_2 = GenStandardSegCount(GetLength(), B2());
	int n_2 = GenNonstandardUnitCount(GetLength(), B2(), N_2);
	double k2 = GenK(GetLength(), B2(), N_2, n_2);

	//��k(���˲�����ռ�϶)�����жϣ��������105����ô��׼���˳ߴ��Ϊ1380�������Ϊ1260
	if (k1 <= 105 && k1 >= 0)
	{
		m_B = m_B1;
		m_N = N_1;

		if (n_1 < 0)
		{
			//AfxMessageBox(_T("�����˳����޷�����"));
			return false;
		}

		m_n = n_1;
		m_K = k1;
	}
	else  
	{
		m_B = m_B2;
		m_N = N_2;

		if (n_2 < 0)
		{
			//AfxMessageBox(_T("�����˳����޷�����"));
			return false;
		}

		m_n = n_2;

		if (k2 > 105 || k2 < 0)	//��K2�����жϣ����K2����105������false
		{
			//AfxMessageBox(_T("�����˳����޷�����"));
			return false;
		}
		m_K = k2;
	}

	return true;
}

int CRCRailingTieyi::GenStandardSegCount(double p_lenth, double p_segLength)const		//�����׼����������p_lenthΪ���˳���p_segLengthΪ���˵ı�׼�γ�
{
	int nCount = 0;
	if ((int)(p_lenth / p_segLength) == 1)
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
int CRCRailingTieyi::GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)	const
{
	//��׼�������൥Ԫ�������� = �������ܳߴ� - ��׼���˳ߴ� - H(�����ռ�ߴ�)��/ ��Ԫ����ߴ�
	int n = (int)((GetLength() - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - GetH()) / Getb());

	//��������Ҫ��2�ı�����������ǣ�����ȡ��ӽ���2�ı���
	if ((n % 2) != 0)
	{
		n--;
	}

	return n/2; //���յ�nȡһ�������,��˳�2
}

double CRCRailingTieyi::GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const
{
	//ͨ����׼���˳ߴ�������������׼�����ܳߴ�,��ʽ����׼�����ܳߴ� = ��׼���˳ߴ�*��׼��������- 40*(��������-1)
	return p_segLength *p_standardSegCount - GetPillarWidth() * (p_standardSegCount - 1);
}

double CRCRailingTieyi::GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount) const//������ռ�϶ = ���������ͽṹǽ��ߴ�/2
{
	double nonStandardLen = p_nonStandardUnitCount * 2 * Getb(); //�Ǳ�׼�εĵ�Ԫ�����ܳ���
	double k = (p_lenth - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - nonStandardLen - GetH()) / 2;
	return k;
}


double CRCRailingTieyi::GetStandardRailingTotalLen()const
{
	return GenStandardRailingTotalLen(m_B, m_N);
}
double CRCRailingTieyi::GetNonstandardLen()const   //��ȡ�Ǳ�����˳��ȣ�����������
{
	//�����ܳߴ� = ��������/2*����ߴ�   /2��ֻ����һ��ĳ���
	double nonStandardLen = m_n * Getb() + (Getb()-GetSmallPillarWidth()) + GetPillarWidth() * 2; //����һ�������϶����������
	return nonStandardLen;
}

//////////////////////////////////////////////////////////////////////////

CRCRailingT1::CRCRailingT1()
{
	m_B1 = 1260;
	m_B2 = 1380;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingT3::CRCRailingT3()
{
	m_B1 = 1250;
	m_B2 = 1320;
}


//////////////////////////////////////////////////////////////////////////
CRCRailingT4::CRCRailingT4()
{
	m_B1 = 1206;
	m_B2 = 1320;
}
AcDbObjectId CRCRailingT4::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id1 = AcDbObjectId::kNull;

	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const double railH0 = 1200 - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sNonStandardBlockName = GetNonStandardBlockName();

	if (MD2010_CheckBlockDefExist(sNonStandardBlockName))
	{
		MD2010_InsertBlockReference_ModelSpace(sNonStandardBlockName, id1, p_pos);
	}
	else
	{
		id1 = InsertBlockRefFromDwg(fileName, sNonStandardBlockName, ACDB_MODEL_SPACE, p_pos);
	}
	assert(id1 != AcDbObjectId::kNull);

	double L = GetNonstandardLen();
	double Ln1 = L - GetPillarWidth() * 2 + 100; //��һ��С�˵����з�ʽ, ��100��������Ծʽ
	double Ln2 = L - GetPillarWidth() * 2;
	double H1 = 219 + (railH - railH0) / 2;  //219��H1��ԭ���еĳ�ʼֵ
	double H2 = 232 + (railH - railH0) / 2;  //232��H1��ԭ���еĳ�ʼֵ

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), L);
	DQ_SetDynamicAttribute(id1, _T("Ln1"), Ln1);
	DQ_SetDynamicAttribute(id1, _T("Ln2"), Ln2);
	DQ_SetDynamicAttribute(id1, _T("Ln21"), Ln2);
	DQ_SetDynamicAttribute(id1, _T("Ln22"), Ln2);
	DQ_SetDynamicAttribute(id1, _T("H"), railH);
	DQ_SetDynamicAttribute(id1, _T("H1"), H1);
	DQ_SetDynamicAttribute(id1, _T("H2"), H2);

	return id1;
}


AcDbObjectIdArray CRCRailingT4::GenerateRailing_Standard(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const double railH0 = 1200 - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sStandardBlockName = GetStandardBlockName();
	double H1 = 219 + (railH - railH0) / 2;  //219��H1��ԭ���еĳ�ʼֵ
	double H2 = 232 + (railH - railH0) / 2;  //232��H1��ԭ���еĳ�ʼֵ

	AcDbObjectIdArray idsOut;
	AcDbObjectId id2;
	for (int i = 0; i < GetN(); i++)
	{
		if (i==0)
			id2 = InsertBlockRefFromDwg(fileName, sStandardBlockName, ACDB_MODEL_SPACE, pos);
		else
			MD2010_InsertBlockReference_ModelSpace(sStandardBlockName, id2, pos);
		assert(id2 != AcDbObjectId::kNull);

		DQ_SetDynamicAttribute(id2, _T("H"), railH);
		DQ_SetDynamicAttribute(id2, _T("H1"), H1);
		DQ_SetDynamicAttribute(id2, _T("H2"), H2);

		idsOut.append(id2);

		pos.x += GetB() - GetPillarWidth(); //��ȥ������
	}

	return idsOut;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingT5::CRCRailingT5()
{
	m_B1 = 1220;
	m_B2 = 1430;
}


AcDbObjectId CRCRailingT5::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id1 = AcDbObjectId::kNull;

	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sNonStandardBlockName = GetNonStandardBlockName();

	if (MD2010_CheckBlockDefExist(sNonStandardBlockName))
	{
		MD2010_InsertBlockReference_ModelSpace(sNonStandardBlockName, id1, p_pos);
	}
	else
	{
		id1 = InsertBlockRefFromDwg(fileName, sNonStandardBlockName, ACDB_MODEL_SPACE, p_pos);
	}
	assert(id1 != AcDbObjectId::kNull);

	double L = GetNonstandardLen();
	double Ln1 = L - GetPillarWidth() * 2 ;		//�����඼����һ���ˣ���˲��ü�100
	double Ln2 = L - GetPillarWidth() * 2 - 100;

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), L);
	DQ_SetDynamicAttribute(id1, _T("Ln"), Ln1);
	DQ_SetDynamicAttribute(id1, _T("Ln2"), Ln2);
	DQ_SetDynamicAttribute(id1, _T("H"), railH);

	//�ж����һ���ǳ��ϻ��ǳ���
	int n1 = (int)(Ln1 / (Getb() * 2));
	int n2 = (int)(Ln2 / (Getb() * 2));
	bool bUp = n1 > n2;
	DQ_SetDynamicAttribute(id1, _T("�ɼ���"), bUp ? _T("����") : _T("����"));

	return id1;
}
//////////////////////////////////////////////////////////////////////////
CRCRailingT6::CRCRailingT6()
{
	m_B1 = 1355;
	m_B2 = 1570;
}


AcDbObjectId CRCRailingT6::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id1 = AcDbObjectId::kNull;

	const double railH = m_railingAtt.m_height - GetHandRailHeight();//�۳����ֵĸ߶�
	const CString fileName = GetPrototypeFilePath();
	const CString sNonStandardBlockName = GetNonStandardBlockName();

	if (MD2010_CheckBlockDefExist(sNonStandardBlockName))
	{
		MD2010_InsertBlockReference_ModelSpace(sNonStandardBlockName, id1, p_pos);
	}
	else
	{
		id1 = InsertBlockRefFromDwg(fileName, sNonStandardBlockName, ACDB_MODEL_SPACE, p_pos);
	}
	assert(id1 != AcDbObjectId::kNull);

	double L = GetNonstandardLen();
	double Ln1 = L - GetPillarWidth() * 2 + Getb()/2;		//�����඼����һ���ˣ���˲��ü�
	double Ln2 = Ln1 - Getb();

	//���÷Ǳ�γ���
	DQ_SetDynamicAttribute(id1, _T("L"), L);
	DQ_SetDynamicAttribute(id1, _T("Ln"), Ln1);
	DQ_SetDynamicAttribute(id1, _T("Ln2"), Ln2);
	DQ_SetDynamicAttribute(id1, _T("H"), railH);

	//�ж����һ���ǳ��ϻ��ǳ���
	int n1 = (int)(Ln1 / (Getb() * 2));
	int n2 = (int)(Ln2 / (Getb() * 2));
	if (n1>0 && n2 ==0) //
	{
		DQ_SetDynamicAttribute(id1, _T("�ɼ���"),_T("2��"));
	}
	else if (n1==0 && n2 ==0)
	{
		DQ_SetDynamicAttribute(id1, _T("�ɼ���"), _T("1��"));
	}

	return id1;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingT7::CRCRailingT7()
{
	m_B1 = 1510;
	m_B2 = 1716;
}

AcDbObjectId CRCRailingT7::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id = CRCRailingTieyi::GenerateRailing_NonStandard(p_pos);

	double Ln = GetNonstandardLen() - GetPillarWidth() * 2;
	if (Ln<200)
	{
		DQ_SetDynamicAttribute(id, _T("�ɼ���"), _T("����"));
	}

	return id;
}