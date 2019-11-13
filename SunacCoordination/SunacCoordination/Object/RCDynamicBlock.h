/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCDynamicBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCBlock.h"

class RCDynamicBlock : public RCBlock
{
public:
	//Constructors
	RCDynamicBlock(void);
	RCDynamicBlock(const RCDynamicBlock &other);

	//Destructor
	~RCDynamicBlock(void);

	//Operator = 
	RCDynamicBlock & operator=(const RCDynamicBlock &rhs);

	int SetParameter(CString key, double value);
	//int SetParameter(CString key, int value); //���ݶ���Ϊdouble�ͣ�Ϊ�������ã��˴�ע�ͣ�YUAN191113
	int SetParameter(CString key, CString value);

	int GetParameter(CString key, double &value);
	int GetParameter(CString key, int &value);
	int GetParameter(CString key, CString &value);

	virtual int InitParameters();

	//��������
	int RunParameters();

	virtual bool isEqualTo(RCObject*other = 0);//��������һ��

	//virtual AcDbObjectId InsertFromFile(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color);

public: 
	vRCPairKeyDValue m_dKeyValues;
	vRCPairKeyIValue m_iKeyValues;
	vRCPairKeyStrValue m_strKeyValues;
};

typedef std::vector<RCDynamicBlock> vRCDynamicBlock;
typedef std::vector<RCDynamicBlock*> vpRCDynamicBlock;