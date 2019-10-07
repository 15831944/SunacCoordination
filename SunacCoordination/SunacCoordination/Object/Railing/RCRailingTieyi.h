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
#include "RCRailing.h"

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

	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

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

