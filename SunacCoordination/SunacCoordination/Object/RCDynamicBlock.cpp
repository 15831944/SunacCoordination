/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCDynamicBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCDynamicBlock.h"
#include <float.h>
#include <algorithm>
#include "../Common/ComFun_DynamicBlock.h"
#include "../Common/ComFun_Sunac.h"
#include "../Tool/DocLock.h"
//#include "../Common/ComFun_Str.h"
//Constructor
RCDynamicBlock::RCDynamicBlock(void)
{

}

//Destructor
RCDynamicBlock::~RCDynamicBlock(void)
{

}


//Constructor
RCDynamicBlock::RCDynamicBlock(const RCDynamicBlock &other):RCBlock(other)
{
	m_dKeyValues = other.m_dKeyValues;
	m_iKeyValues = other.m_iKeyValues;
	m_strKeyValues = other.m_strKeyValues;
}

//Operator = 
RCDynamicBlock & RCDynamicBlock::operator=(const RCDynamicBlock &other)
{
	m_dKeyValues = other.m_dKeyValues;
	m_iKeyValues = other.m_iKeyValues;
	m_strKeyValues = other.m_strKeyValues;
	return *this;
}

int RCDynamicBlock::SetParameter(CString key, double value)
{
	int error = -1;
	for (UINT m = 0; m < m_dKeyValues.size(); m++)
	{
        if (key == m_dKeyValues[m].first)
        {
			m_dKeyValues[m].second = value;
			error = 0;
			break;
        }
	}

	return error; 
}

//int RCDynamicBlock::SetParameter(CString key, int value)
//{
//	int error = -1;
//	for (UINT m = 0; m < m_iKeyValues.size(); m++)
//	{
//		if (key == m_iKeyValues[m].first)
//		{
//			m_iKeyValues[m].second = value;
//			error = 0;
//			break;
//		}
//	}
//	return error;
//}

int RCDynamicBlock::GetParameter(CString key, CString &value)
{
	int error = -1;
	for (UINT m = 0; m < m_strKeyValues.size(); m++)
	{
		if (key == m_strKeyValues[m].first)
		{
			value = m_strKeyValues[m].second;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::GetParameter(CString key, double &value)
{
	int error = -1;
	for (UINT m = 0; m < m_dKeyValues.size(); m++)
	{
		if (key == m_dKeyValues[m].first)
		{
			value = m_dKeyValues[m].second;
			error = 0;
			break;
		}
	}

	return error; 
}

int RCDynamicBlock::GetParameter(CString key, int &value)
{
	int error = -1;
	for (UINT m = 0; m < m_iKeyValues.size(); m++)
	{
		if (key == m_iKeyValues[m].first)
		{
			value = m_iKeyValues[m].second;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::SetParameter(CString key, CString value)
{
	int error = -1;
	for (UINT m = 0; m < m_strKeyValues.size(); m++)
	{
		if (key == m_strKeyValues[m].first)
		{
			m_strKeyValues[m].second = value;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::InitParameters()
{
	CDocLock lock;
	
	if (m_id == 0)
		return -1;

	if(!TYCOM_IsDyanamicBlock(m_id))
		return -1;

	//es = acdbOpenObject(pEnt1, m_id, AcDb::kForWrite);


	m_strKeyValues.clear();
	m_iKeyValues.clear();
	m_dKeyValues.clear();

	AcDbObjectPointer<AcDbBlockReference> pBlkRef(m_id, AcDb::kForRead);
	if (pBlkRef.openStatus() != Acad::eOk)
		return 1;
	pBlkRef->close(); //关闭以便能以动态块的方式重建

	m_blockInsertPos = pBlkRef->position();
	pBlkRef->getGeomExtents(m_blockExtent);

	AcDbObjectPointer<AcDbBlockTableRecord>pBlkTblRcd11(pBlkRef->blockTableRecord(), AcDb::kForRead);
	if (pBlkTblRcd11.openStatus() != Acad::eOk)
		return 1;
	pBlkTblRcd11->close();

	ACHAR *pName = 0;
	pBlkTblRcd11->getName(pName);
	m_blockRecordName = pName;

	//////////////////////////////////////////////////////////////////////////

	// initialise a AcDbDynBlockReference from the object id of the blockreference
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());
	AcDbObjectId idBlkTblRcd= pDynBlkRef->dynamicBlockTableRecord();

	bool noValue = true;
	if (pDynBlkRef)
	{
		AcDbDynBlockReferencePropertyArray blkPropAry;
		pDynBlkRef->getBlockProperties(blkPropAry);
		AcDbDynBlockReferenceProperty blkProp;
		for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
		{
			blkProp = blkPropAry[lIndex1];

			AcString a = blkProp.propertyName();
			CString aa(a.constPtr());
			AcDbEvalVariant va;
			
			if (blkProp.propertyType() == AcDb::kDwgInt32)
			{
				va = blkProp.value();
				int value = va.resval.rint;
				m_iKeyValues.push_back(RCPairKeyIValue(aa, value));
			}
			else if (blkProp.propertyType() == AcDb::kDwgReal)
			{
				va = blkProp.value();
				double value = va.resval.rreal;
				m_dKeyValues.push_back(RCPairKeyDValue(aa, value));
			}
			else if (blkProp.propertyType() == AcDb::kDwgText)
			{
				va = blkProp.value();
				CString value(va.resval.rstring);
				m_strKeyValues.push_back(RCPairKeyStrValue(aa, value));
			}
			
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	pBlkTblRcd11->close();
	return noValue;
}

int RCDynamicBlock::RunParameters()
{
	CDocLock lock;

	if (m_id == 0)
		return -1;

	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, m_id, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		pEnt->close();
		return -1;
	}
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	// initialise a AcDbDynBlockReference from the object id of the blockreference
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());
	//Don't forget to close the blockreference here, otherwise you wont be able to modify properties
	pEnt->close();
	if (pDynBlkRef)
	{
		AcDbDynBlockReferencePropertyArray blkPropAry;
		pDynBlkRef->getBlockProperties(blkPropAry);
		Acad::ErrorStatus err;
		AcDbDynBlockReferenceProperty blkProp;
		for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
		{
			blkProp = blkPropAry[lIndex1];
			if (blkProp.readOnly())
				continue;

			for (UINT m = 0; m < m_dKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_dKeyValues[m].first) != 0)
					continue;

				AcDbEvalVariant eval(m_dKeyValues[m].second);
				es = blkProp.setValue(eval);
				if (es != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}

			for (UINT m = 0; m < m_iKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_iKeyValues[m].first) != 0) 
					continue;

				AcDbEvalVariant eval(m_iKeyValues[m].second);
				if ((err = blkProp.setValue(eval)) != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}

			for (UINT m = 0; m < m_strKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_strKeyValues[m].first) != 0) continue;

				AcDbEvalVariant eval(m_strKeyValues[m].second);
				if ((err = blkProp.setValue(eval)) != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}
			
		}

		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}

	actrTransactionManager->flushGraphics();//必须lock住文档才有效果
	acedUpdateDisplay();
	return 0;
}

bool RCDynamicBlock::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCDynamicBlock * pObj = dynamic_cast<RCDynamicBlock *>(other);
	if (pObj == 0)
		return false;

	if (!RCBlock::isEqualTo(other))
		return false;

	if (!TY_IsPairsEqual(m_dKeyValues, pObj->m_dKeyValues))
		return false;

	if (!TY_IsPairsEqual(m_iKeyValues, pObj->m_iKeyValues))
		return false;

	if (!TY_IsPairsEqual(m_strKeyValues, pObj->m_strKeyValues))
		return false;
	
	return true;
}

/*

AcDbObjectId RCDynamicBlock::InsertFromFile(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	WCHAR blockname[256] = L"";
	CF_STR_get_file_name(fileName, blockname);
	CF_STR_get_file_name_2(blockname, blockname);
	__time64_t tick = CTime::GetCurrentTime().GetTime();
	CString str;
	str.Format(L"_%d",tick);
	m_blockRecordName = CString(blockname) + str;
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, fileName, m_blockRecordName,  m_id, origin, angle, AcGeScale3d(1), layerName, color);
	return m_id;
}
*/
