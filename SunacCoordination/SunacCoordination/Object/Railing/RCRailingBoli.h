
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

	virtual double GetMinWidth()const { return 0; }
	virtual double GetMinHeight()const { return 300; }

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


	virtual AcDbObjectId CreateRailingBlockDefine(CString sRailingDefName);

	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_HandRail(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingB1 : public CRCRailingBoli
{
public:
	CRCRailingB1();
	~CRCRailingB1();

	virtual double GetMinHeight()const { return 400; }

protected:
	virtual int GenStandardSegCount(double p_lenth)const;		//�����׼��������
	virtual double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�

	virtual double GetK()const { return 275/2.0; }			//2K�̶�ֵ275
	virtual double GetH() const { return 150; }				//��Ԫ֮��ļ��
	virtual double GetHandRailHeight()const { return 33; }	//���ֺ�ȸ�

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB2 : public CRCRailingBoli
{
public:
	CRCRailingB2();
	~CRCRailingB2();

	virtual double GetK()const { return 275 / 2.0; }			//2K�̶�ֵ275
	virtual double GetH() const { return 0; }
	
protected:
	int GenStandardSegCount(double p_lenth)const;			//�����׼��������
	double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�

	virtual double GetHandRailHeight()const { return 27.5; }	//���ֺ�ȸ�
};

class CRCRailingB3_1 : public CRCRailingB2
{
protected:
	virtual double GetHandRailHeight()const { return 33.5; }	//���ֺ�ȸ�
};
class CRCRailingB3_2 : public CRCRailingB2
{
protected:
	virtual double GetHandRailHeight()const { return 48; }	//���ֺ�ȸ�
};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB4 : public CRCRailingBoli
{
public:
	CRCRailingB4();
	~CRCRailingB4();

	virtual double GetK()const { return 150/2.0; }
	virtual double GetH() const { return 0; }

protected:
	int GenStandardSegCount(double p_lenth)const;			//�����׼��������
	double GenStandardRailingLen(double p_lenth)const;		//�����׼���˳ߴ�

	virtual double GetHandRailHeight()const { return 45; }	//���ֺ�ȸ�
};

class CRCRailingB5 : public CRCRailingB4
{
protected:
	virtual double GetHandRailHeight()const { return 0; }	//���ֺ�ȸ�

	virtual double GetMinHeight()const { return 500; }
};
class CRCRailingB6 : public CRCRailingB4
{
protected:
	virtual double GetHandRailHeight()const { return 57.8; }	//���ֺ�ȸ�

	virtual double GetMinHeight()const { return 700; }
};