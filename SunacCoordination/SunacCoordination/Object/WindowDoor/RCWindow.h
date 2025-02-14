/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "..\RCDynamicBlock.h"
#include "AttrWindow.h"

class RCWindow : public RCDynamicBlock
{
public:
	//Constructors
	RCWindow(void);
	RCWindow(const RCWindow &other);

	//Destructor
	~RCWindow(void);

	//Operator = 
	RCWindow & operator=(const RCWindow &rhs);

	virtual RCTYPE GetType()const{return T_WINDOW;}
	virtual AcDbObjectId Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color);
	void ModifyLayerName(AcDbObjectId BlockDefineId);//修改图层

	double GetH();
	double GetH1();
	double GetH2();
	double GetW();
	double GetW1();
	double GetW2();
	double GetW3();
	double GetA();

	int SetH(double newValue);
	int SetH1(double newValue);
	int SetH2(double newValue);
	int SetW(double newValue);
	int SetW1(double newValue);
	int SetW2(double newValue);
	int SetW3(double newValue);
	int SetA(double newValue);

	//门窗面积
	double GetWindowArea();

	void SetInstanceCode(CString str);
	CString GetInstanceCode();

	AttrWindow * GetAttribute();
	const AttrWindow * GetAttributeConst();
	void AddAttribute(AttrWindow * attr);

	virtual RCWindow * Clone();
	virtual void Draw();

	virtual bool isEqualTo(RCObject*other);//基础数据一致

protected: 
};

typedef std::vector<RCWindow> vRCWindow;
typedef std::vector<RCWindow*> vpRCWindow;