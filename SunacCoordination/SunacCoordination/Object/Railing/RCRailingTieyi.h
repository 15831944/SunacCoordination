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
	
	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetStandardRailingTotalLen()const;
	virtual double GetNonstandardLen()const;				//��ȡ�Ǳ�����˳��ȣ�����������

protected:
	virtual bool GenRailing(); //������εĳ���
	virtual int GenStandardSegCount(double p_lenth, double p_segLength)const; //�����׼����������p_lenthΪ���˳���p_segLengthΪ���˵ı�׼�γ�
	virtual int GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)const;	//��ȡ��׼�������൥Ԫ��Ԫ��������
	virtual double GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const;	//��ȡ��׼�����ܳߴ�
	virtual double GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount)const;	//��ȡ���˲�����ռ�϶

protected:
	CString GetStandardBlockName() const;
	CString GetNonStandardBlockName() const;
	CString GetHandRailBlockName() const;
	double B1() const { return m_B1; }
	double B2() const { return m_B2; }

	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_HandRail(AcGePoint3d pos);


protected:
	double m_B1;
	double m_B2;
};

class CRCRailingT1 : public CRCRailingTieyi  //������, T1-T2�������ô���
{
public:
	CRCRailingT1();



};

//////////////////////////////////////////////////////////////////////////

class CRCRailingT3 : public CRCRailingT1  
{
public:
	CRCRailingT3();

	virtual double Getb()const{ return 120; }			//��Ԫ����ߴ�120
	virtual double GetH() const { return 280; }			//H�̶�ֵ280
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�40
	virtual double GetSmallPillarWidth()const { return 20; }//С���˳ߴ�40
	virtual double GetHandRailHeight()const { return 40; };	//���ֺ�ȸ�40

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingT4 : public CRCRailingTieyi
{
public:
	CRCRailingT4();

	virtual double Getb()const{ return 70; }			//��Ԫ����ߴ�
	virtual double GetH() const { return 180; }			//H�̶�ֵ
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�40
	virtual double GetSmallPillarWidth()const { return 20; }//С���˳ߴ�40
	virtual double GetHandRailHeight()const { return 40; };	//���ֺ�ȸ�40

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
};

//////////////////////////////////////////////////////////////////////////
class CRCRailingT5 : public CRCRailingTieyi
{
public:
	CRCRailingT5();

	virtual double Getb()const{ return 105; }			//��Ԫ����ߴ�
	virtual double GetH() const { return 260; }			//H�̶�ֵ
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�
	virtual double GetSmallPillarWidth()const { return 15; }//С���˳ߴ�
	virtual double GetHandRailHeight()const { return 40; };	//���ֺ�ȸ�

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingT6 : public CRCRailingTieyi
{
public:
	CRCRailingT6();

	virtual double Getb()const{ return 215; }			//��Ԫ����ߴ�
	virtual double GetH() const { return 480; }			//H�̶�ֵ
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�
	virtual double GetSmallPillarWidth()const { return 15; }//С���˳ߴ�
	virtual double GetHandRailHeight()const { return 40; };	//���ֺ�ȸ�

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingT7 : public CRCRailingTieyi
{
public:
	CRCRailingT7();

	virtual double Getb()const{ return 206; }			//��Ԫ����ߴ�
	virtual double GetH() const { return 468; }			//H�̶�ֵ
	virtual double GetPillarWidth()const { return 40; }	//�����ߴ�
	virtual double GetSmallPillarWidth()const { return 12; }//С���˳ߴ�
	virtual double GetHandRailHeight()const { return 100; };	//���ֺ�ȸ�

};

