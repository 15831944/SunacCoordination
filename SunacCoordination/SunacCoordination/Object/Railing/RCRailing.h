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
	
	virtual RCTYPE GetType()const{ return T_RAILING; }
	
	void SetRailingAtt(const AttrRailing p_railingAtt);
	AttrRailing GetRailingAtt()const { return m_railingAtt; }

	virtual bool SetLength(double p_length);
	virtual bool CheckLength();
	virtual bool CheckHeight();
	virtual bool GenRailing() = 0;
	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetLength()const { return m_railingAtt.GetRLength(); } //获取栏杆总长度
	virtual double GetK()const { return m_K; }		//获取栏杆侧边留空间隙
	virtual double GetB()const { return m_B; }		//获取标准栏杆尺寸
	virtual int GetN()const { return m_N; }
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40

	virtual double GetStandardRailingTotalLen()const = 0;
	virtual double GetNonstandardLen()const = 0;	//获取非标段栏杆长度，含两侧立柱

	virtual double GetMinWidth()const { return 0; }
	virtual double GetMinHeight()const { return 400; }

	void SetSimpleDraw(bool p_bSimple){ m_bSimple = p_bSimple; }
	bool IsSampleDraw()const { return m_bSimple; }
	
	//////////////////////////////////////////////////////////////////////////
	//栏杆详图
	static void DrawRailingDetail();
	static void CreateRailingDetailDim(const AttrRailing& railAtt, AcDbObjectId m_id, CRCRailing* pRailing);
	static AcDbObjectId DrawRailingWhiteWall(const AttrRailing& railAtt, AcDbObjectId m_id);
	static AcDbObjectId DrawRailingYellowWall(const AttrRailing& railAtt, AcDbObjectId m_id);
	static AcDbObjectId DrawRailingFill(const AttrRailing& railAtt, AcDbObjectId m_id);

	static AcDbObjectId CreateDimension(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double size);
	static AcDbObjectId GetDimstylerID(CString dimname);//通过标注样式名称获得标注样式id
	static void CreateDimensionStyle();//创建标注样式
	static void SetRailingDetailTextStyle(CString dimname); //设置字体样式的字体
	static void CreateRailingDetailTextStyle(CString dimname); //创建字体样式的字体
	static AcDbObjectId CreateHatch(const AcDbObjectIdArray &loopIds, bool bAssociative);

	//栏杆平面图
	AcDbObjectId GenerateRailingTop(CString p_blockName, const vector<AcGePoint3d>& p_pts);
	AcDbObjectId GenerateRailingTop(CString p_blockName, AcGePoint3d p_pnt1, AcGePoint3d p_pnt2);
	bool CreateRailingTop(const vector<AcGePoint3d> p_pts);

	static bool CheckRailingStartEndPt(AcGePoint3d& p_pnt1, AcGePoint3d& p_pnt2, int& p_width);//计算平面图插入点，使栏杆长为10的倍数
	static void CheckRailingLength(int& p_width);



	AcDbObjectId CreateWipeOut();

protected:
	virtual AcDbObjectId CreateRailingBlockDefine(CString sRailingDefName)= 0;
	CString GetPrototypeFilePath()const ;

protected:
	double m_B;			//标准栏杆尺寸 1260或者1380
	int	   m_N;			//标准栏杆段数量 
	double m_K;			//侧边立柱与结构墙间尺寸（单边）

	AttrRailing m_railingAtt;

	bool m_bSimple; //是否为建筑的简化图
};

typedef std::vector<CRCRailing*> vpRCRailing;

//////////////////////////////////////////////////////////////////////////

CRCRailing* CreateRailing(const AttrRailing p_railingAtt);


