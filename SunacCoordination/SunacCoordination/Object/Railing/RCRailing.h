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
	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetLength()const { return m_railingAtt.m_length; } //��ȡ�����ܳ���
	virtual double GetK()const { return m_K; }		//��ȡ���˲�����ռ�϶
	virtual double GetB()const { return m_B; }		//��ȡ��׼���˳ߴ�
	virtual int GetN()const { return m_N; }

	virtual double GetStandardRailingTotalLen()const = 0;
	virtual double GetNonstandardLen()const = 0;	//��ȡ�Ǳ�����˳��ȣ�����������


protected:
	virtual bool GenRailing() = 0;

	virtual AcDbObjectId CreateRailingBlockDefine(CString sRailingDefName)= 0;

	CString GetPrototypeFilePath()const ;

protected:
	//double m_length;	//�����ܳߴ� Ʃ��5400
	double m_B;			//��׼���˳ߴ� 1260����1380
	int	   m_N;			//��׼���˶����� 
	double m_K;			//���������ṹǽ��ߴ磨���ߣ�

protected: 
	AttrRailing m_railingAtt;


	//////////////////////////////////////////////////////////////////////////
	
};

typedef std::vector<CRCRailing*> vpRCRailing;

//////////////////////////////////////////////////////////////////////////

CRCRailing* CreateRailing(const AttrRailing p_railingAtt);

