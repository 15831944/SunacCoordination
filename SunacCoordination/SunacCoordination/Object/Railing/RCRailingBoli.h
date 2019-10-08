
#pragma once

#include <vector>
#include "..\RCStandBlock.h"
#include "AttrRailing.h"
#include "RCRailing.h"

//////////////////////////////////////////////////////////////////////////
class CRCRailingBoli : public CRCRailing
{
public:
	virtual double GetNonstandardLen()const{ return 0; }//û�зǱ�׼��
	virtual double GetStandardRailingTotalLen()const{ return GetLength(); } //û�зǱ�׼��

	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);


protected:
	virtual bool GenRailing();
	virtual int GenStandardSegCount(double p_lenth)const = 0;			//�����׼��������
	virtual double GenStandardRailingLen(double p_lenth)const = 0;		//�����׼���˳ߴ�


	CString GetStandardBlockName() const;
	CString GetLeftBlockName() const;
	CString GetRightBlockName() const;
	CString GetHandRailBlockName() const;

	virtual double GetH() const { return 0; }				//��Ԫ֮��ļ�࣬Ĭ��Ϊ0
	virtual double GetHandRailHeight()const { return 33; };	//���ֺ�ȸ�

	virtual AcDbObjectId GenerateRailing_Left(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_Right(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_HandRail(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingB1 : public CRCRailingBoli
{
public:
	CRCRailingB1();
	~CRCRailingB1();

	virtual double Get2K()const { return 275; }			//2K�̶�ֵ275
	virtual double GetK()const { return 275/2.0; }			//2K�̶�ֵ275

protected:
	virtual int GenStandardSegCount(double p_lenth)const;		//�����׼��������
	virtual double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�



protected:
	virtual double GetH() const { return 150; }		//��Ԫ֮��ļ��
	double GetHandRailHeight()const { return 33; }	//���ֺ�ȸ�40


};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB2 : public CRCRailingBoli
{
public:
	CRCRailingB2();
	~CRCRailingB2();

	virtual double GetH() const { return 0; }

protected:
	int GenStandardSegCount(double p_lenth)const;			//�����׼��������
	double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�
};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB4 : public CRCRailingBoli
{
public:
	CRCRailingB4();
	~CRCRailingB4();

	virtual double Get2K()const { return 150; }
	virtual double GetH() const { return 0; }

protected:
	int GenStandardSegCount(double p_lenth)const;			//�����׼��������
	double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�

};