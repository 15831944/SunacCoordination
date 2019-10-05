/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCLanGan.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "..\RCStandBlock.h"
#include "AttrRailing.h"

class CRCRailing : public RCStandBlock
{
public:
	CRCRailing(void);
	virtual ~CRCRailing(void);

	//CRCRailing(const CRCRailing &other);
	//virtual CRCRailing & operator=(const CRCRailing &rhs);

	virtual RCTYPE GetType()const{ return T_RAILING; }
	//virtual CRCRailing * Clone() = 0; //TODO
	
	void SetRailingAtt(const AttrRailing p_railingAtt);
	AttrRailing GetRailingAtt()const { return m_railingAtt; }

	virtual bool SetLength(double p_length);
	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end) = 0;

	//////////////////////////////////////////////////////////////////////////
	virtual double GetLenght()const { return m_railingAtt.m_length; } //��ȡ�����ܳ���
	virtual double Get2K()const { return 2 * m_K; }		//��ȡ���˲�����ռ�϶
	virtual double GetK()const { return m_K; }		//��ȡ���˲�����ռ�϶
	virtual double GetB()const { return m_B; }			//��ȡ��׼���˳ߴ�
	virtual int GetN()const { return m_N; }
	virtual int Getn()const { return m_n; }

	virtual double GetStandardRailingTotalLen()const = 0;
	virtual double GetNonstandardLen()const = 0;	//��ȡ�Ǳ�����˳��ȣ�����������

	virtual double Getb()const = 0;				//��Ԫ����ߴ�120
	virtual double GetH() const = 0;			//H�̶�ֵ280
	virtual double GetPillarWidth()const = 0;	//�����ߴ�40
	virtual double GetSmallPillarWidth()const = 0;//С���˳ߴ�40
	virtual double GetHandRailHeight()const = 0;	//���ֺ�ȸ�40

protected:
	virtual bool GenRailing() = 0;

protected:
	//double m_length;	//�����ܳߴ� Ʃ��5400
	double m_B;			//��׼���˳ߴ� 1260����1380
	int	   m_N;			//��׼���˶����� 
	int	   m_n;			//��׼�������൥Ԫ�������������ࣩ
	double m_K;			//���������ṹǽ��ߴ磨���ߣ�

protected: 
	AttrRailing m_railingAtt;


	//////////////////////////////////////////////////////////////////////////
	
};

typedef std::vector<CRCRailing*> vpRCRailing;


//////////////////////////////////////////////////////////////////////////
class CRCRailingTieyi : public CRCRailing
{
public:
	CRCRailingTieyi();
	~CRCRailingTieyi();

	virtual double Getb()const{ return 120; }			//��Ԫ����ߴ�120
	virtual double GetH() const { return 280; }			//H�̶�ֵ280
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�40
	virtual double GetSmallPillarWidth()const { return 20; }//С���˳ߴ�40
	virtual double GetHandRailHeight()const { return 40; };	//���ֺ�ȸ�40


private:

};

class CRCRailingT1 : public CRCRailingTieyi  //������
{
public:
	CRCRailingT1();

	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetStandardRailingTotalLen()const;
	virtual double GetNonstandardLen()const;				//��ȡ�Ǳ�����˳��ȣ�����������


protected:
	virtual bool GenRailing();
	int GenStandardSegCount(double p_lenth, double p_segLength)const;		//�����׼����������p_lenthΪ���˳���p_segLengthΪ���˵ı�׼�γ�
	int GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)const;					//��ȡ��׼�������൥Ԫ��Ԫ��������
	double GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const;	//��ȡ��׼�����ܳߴ�
	double GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount)const;							//��ȡ���˲�����ռ�϶

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingBoli : public CRCRailing
{
public:
	CRCRailingBoli();
	~CRCRailingBoli();

private:

};


CRCRailing* CreateRailing(const AttrRailing p_railingAtt);

