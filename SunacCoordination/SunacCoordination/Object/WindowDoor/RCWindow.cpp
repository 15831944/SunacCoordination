/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCWindow.h"
#include <float.h>
#include <algorithm>
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Tool/DocLock.h"

//Constructor
RCWindow::RCWindow(void)
{
	m_pAttribute = 0;
}

//Destructor
RCWindow::~RCWindow(void)
{

}


//Constructor
RCWindow::RCWindow(const RCWindow &other):RCDynamicBlock(other)
{
	m_pAttribute = other.m_pAttribute;
}

//Operator = 
RCWindow & RCWindow::operator=(const RCWindow &other)
{
	m_pAttribute = other.m_pAttribute;
	return *this;
}


void RCWindow::Draw()
{
}

RCWindow * RCWindow::Clone()
{
	RCWindow * newObj = new RCWindow;

	return newObj;
}

double RCWindow::GetH()
{
	double value = 0;
	GetParameter(L"H",value);
	return value;
}
double RCWindow::GetH1()
{
	double value = 0;
	GetParameter(L"H1",value);
	return value;
}
double RCWindow::GetH2()
{
	double value = 0;
	GetParameter(L"H2",value);
	return value;
}
double RCWindow::GetW()
{
	double value = 0;
	GetParameter(L"W",value);
	return value;
}
double RCWindow::GetW1()
{
	double value = 0;
	GetParameter(L"W1",value);
	return value;
}
double RCWindow::GetW2()
{
	double value = 0;
	GetParameter(L"W2",value);
	return value;
}
double RCWindow::GetW3()
{
	double value = 0;
	GetParameter(L"W3",value);
	return value;
}
double RCWindow::GetA()
{
	double value = 0;
	GetParameter(L"A",value);
	return value;
}

int RCWindow::SetH(double newValue)
{
	return SetParameter(L"H", newValue);
}
int RCWindow::SetH1(double newValue)
{
	return SetParameter(L"H1", newValue);
}
int RCWindow::SetH2(double newValue)
{
	return SetParameter(L"H2", newValue);
}
int RCWindow::SetW(double newValue)
{
	return SetParameter(L"W", newValue);
}
int RCWindow::SetW1(double newValue)
{
	return SetParameter(L"W1", newValue);
}
int RCWindow::SetW2(double newValue)
{
	return SetParameter(L"W2", newValue);
}
int RCWindow::SetW3(double newValue)
{
	return SetParameter(L"W3", newValue);
}
int RCWindow::SetA(double newValue)
{
	return SetParameter(L"A", newValue);
}

AttrWindow * RCWindow::GetAttribute()
{
	if (m_pAttribute == 0)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(m_id, pDataEnt);
		m_pAttribute = dynamic_cast<AttrWindow *>(pDataEnt);
	}
	return m_pAttribute;
}

void RCWindow::AddAttribute(AttrWindow * attr)
{
	if (attr != 0)
	    TY_AddAttributeData(m_id, attr);
}

bool RCWindow::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCWindow * pObj = dynamic_cast<RCWindow *>(other);
	if (pObj == 0)
		return false;

	if (!RCDynamicBlock::isEqualTo(other))
		return false;

	AttrWindow * thisAttr = GetAttribute();
	AttrWindow * otherAttr = pObj->GetAttribute();
	if (!thisAttr->isEqualTo(otherAttr))
		return false;

	return true;
}

//门窗面积
double RCWindow::GetWindowArea()
{
    return GetH() * GetW();
}


void RCWindow::SetInstanceCode(CString str)
{
	GetAttribute()->SetInstanceCode(str);
}
CString RCWindow::GetInstanceCode()
{
	return GetAttribute()->GetInstanceCode();
}

int RCWindow::CreateDims()
{
	CDocLock lockEnt;
	if (m_id == 0)
		return -1;

	const double W = GetW();
	const double H = GetH();
	const double A = GetA();
	
	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + W;
	rect.m_rt.y = rect.m_lb.y + H;
	rect.m_rt.z = 0;
	
	//JHCOM_GetObjectMinMaxPoint(m_id, rect.m_lb, rect.m_rt);
	double offset = 150;

	//----------------先标注竖向的--------------------//
	const double h2 = GetH2();
	const double h1 = GetH1();
	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();

	AcGePoint3d  start = rightBottomPt;
	AcGePoint3d  end = rightBottomPt;
	AcGePoint3d  mid = rightBottomPt;

	int colorIndex = 2;
	int textHeight = 80;
	CString layer = L"0";

	if (A > TOL)//如果A值存在 先标注两端的A
	{
		end.y += A;
		mid.y += A / 2;
		mid.x = start.x + offset;
		MD2010_AddAlignedDimension2(start, end, mid, layer,colorIndex,textHeight);

		AcGePoint3d start1 = rightTopPt;
		AcGePoint3d end1 = rightTopPt;
		AcGePoint3d mid1 = rightTopPt;
		end1.y -= A;
		mid1.y -= A / 2;
		mid.x = start.x + offset;
		MD2010_AddAlignedDimension2(rightTopPt, end1, mid1, layer,colorIndex,textHeight);
	}

	if (h2 > TOL)
	{
		start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A, 0);
		end = AcGePoint3d(start.x, start.y + h2, 0);
		mid = AcGePoint3d(start.x + offset,  start.y + h2 / 2 , 0);
		MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);
	}

	//H1一定有
	start = AcGePoint3d(rightBottomPt.x, rightBottomPt.y + A + h2, 0);
	end = AcGePoint3d(start.x, start.y + h1, 0);
	mid = AcGePoint3d(start.x + offset, start.y + h1 / 2, 0);
	MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);

	//总高度一定有
	start = rightBottomPt;
	end = rightTopPt;
	mid = AcGePoint3d(start.x + offset* 2, (start.y+end.y)/2, 0);
	MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);

	//////////////////////////////////////////////////////////////////////////
	//----------------标注横向的--------------------//

	double W1 = GetW1();
	double W2 = GetW2();
	double W3 = GetW3();

	start = rect.GetLT();
	end = start;
	mid = start;
	if (A > TOL)//如果A值存在 先标注两端的A
	{
		end.x += A;
		mid.x += A/2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);

		AcGePoint3d start1 = rect.GetRT();
		AcGePoint3d end1 = start1;
		AcGePoint3d mid1 = start1;
		end1.x -= A;
		mid1.x -= A/2;
		mid1.y += offset;
		MD2010_AddAlignedDimension2(start1,end1, mid1, layer,colorIndex,textHeight);
	}

	//W1 都会存在
	start = end;
	mid = end;
	end.x += W1;
	mid.x += W1/2;
	mid.y += offset;
	MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);

	//标注W2
	if (W2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W2;
		mid.x += W2/2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);
	}

	//最后可能还有一个W1
	if (W - W1 - W2 - A*2 > TOL)
	{
		start = end;
		mid = end;
		end.x += W1;
		mid.x += W1/2;
		mid.y += offset;
		MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);
	}

	//标注总的宽度
	start = rect.GetLT();
	mid = start;
	end = start;
	end.x += W;
	mid.x += W/2;
	mid.y += offset*2;
	MD2010_AddAlignedDimension2(start,end, mid, layer,colorIndex,textHeight);

	return 0;
}
