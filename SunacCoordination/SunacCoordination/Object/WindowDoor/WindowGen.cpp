#include "StdAfx.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "WindowGen.h"



//////////////////////////////////////////////////////////////////////////


CWindowInsertPara CWindowGen::InitInsertPara(const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir)
{
	CWindowInsertPara insertPara;
	insertPara.insertPos = pos;
	insertPara.rotateAngle = 0;
	insertPara.mirrorBasePt = AcGePoint3d(pos.x + curWinAtt.GetW() / 2, 0, 0);
	insertPara.mirrorAxis = AcGeVector3d(0, 1, 0);

	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			insertPara.rotateAngle = PI;
			offsetXY.x += curWinAtt.GetW();
			break;
		case E_DIR_RIGHT:
			insertPara.rotateAngle = -PI / 2;
			offsetXY.y += curWinAtt.GetW();
			insertPara.mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			insertPara.mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			insertPara.rotateAngle = PI / 2;
			insertPara.mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			insertPara.mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_UNKNOWN:
		default:
			break;
		}

		insertPara.insertPos += offsetXY;
	}

	return insertPara;
}

void CWindowGen::UpdateRcWindowPara(RCWindow &oneWindow, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd)
{
	//���²���
	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", curWinAtt.GetH());
	oneWindow.SetParameter(L"W", curWinAtt.GetW());
	oneWindow.SetParameter(L"A", curWinAtt.GetA());
	oneWindow.SetParameter(L"D", curWinAtt.GetD());

	oneWindow.SetParameter(L"W1", curWinAtt.GetW1());
	if (curWinAtt.HasValue(_T("W2")))
		oneWindow.SetParameter(L"W2", curWinAtt.GetW2());
	if (curWinAtt.HasValue(_T("W3")))
		oneWindow.SetParameter(L"W3", curWinAtt.GetW3());

	if (curWinAtt.HasValue(_T("H1")))
		oneWindow.SetParameter(L"H1", curWinAtt.GetH1());
	if (curWinAtt.HasValue(_T("H2")))
		oneWindow.SetParameter(L"H2", curWinAtt.GetH2());
	if (curWinAtt.HasValue(_T("H3")))
		oneWindow.SetParameter(L"H3", curWinAtt.GetH3());

	oneWindow.RunParameters();
	//////////////////////////////////////////////////////////////////////////
	//����ɼ���
	if (p_view == E_VIEW_FRONT || p_view == E_VIEW_EXTEND)
	{
		DQ_SetDynamicAttribute(oneWindow.m_id, _T("�ɼ���1"), p_bDetailWnd ? _T("��ͼ") : _T("����"));
	}

	//�������
	if (p_bDetailWnd == false) //�Ŵ���ͼ����Ҫ�������
	{
		AttrWindow * pWindow = oneWindow.GetAttribute();
		if (pWindow == NULL)
		{
			//�����ݼ�¼��ͼ�����չ�ֵ���
			pWindow = new AttrWindow(curWinAtt);
			pWindow->m_viewDir = p_view;
			oneWindow.AddAttribute(pWindow);
			pWindow->close();
		}
		else
		{
			*pWindow = curWinAtt;
			pWindow->close();
		}
	}
}

AcDbObjectId  CWindowGen::GenerateWindow(const AttrWindow& curWinAtt, const AcGePoint3d pos,
	E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName)
{
	eViewDir p_view = curWinAtt.m_viewDir;
	const CWindowInsertPara insertPara = InitInsertPara(curWinAtt, pos, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, insertPara.insertPos, insertPara.rotateAngle, p_sLayerName, 256);
	//������
	bool bMirror = curWinAtt.m_isMirror;
	if (p_view == E_VIEW_TOP)
	{
		bMirror = !bMirror; // yuan 1124 ԭ��ƽ��ͼԭ�͵ķ��������ͼì�ܵ�����
	}
	if (bMirror && (curWinAtt.m_isMirrorWindow == false))
	{
		TYCOM_Mirror(oneWindow.m_id, insertPara.mirrorBasePt, insertPara.mirrorAxis);
	}

	//////////////////////////////////////////////////////////////////////////
	//���USC���괦��
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}


	//////////////////////////////////////////////////////////////////////////
	UpdateRcWindowPara(oneWindow, curWinAtt, p_view, p_bDetailWnd);
	

	return id;
}

AcDbObjectId CWindowGen::UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin, const AcDbObjectId p_originalId)
{
	if (p_id == AcDbObjectId::kNull)
	{
		return p_id;
	}

	const CString p_sLayerName = JHCOM_GetEntityLayer(p_id);

	E_DIRECTION p_winDir = E_DIR_TOP; //TODO �Զ�������ͼ����
	bool p_bDetailWnd = false;
	CString sOldPrototypeCode;
	AcDbObjectId fromWinId = AcDbObjectId::kNull; //��ʾ���Ŵ���Դ��fromWinId����

	AcDbObject* pOldObj = NULL;
	TY_GetAttributeData(p_id, pOldObj);
	const AttrWindow *pOldWinAtt = dynamic_cast<AttrWindow *>(pOldObj);
	if (pOldWinAtt != NULL)
	{
		sOldPrototypeCode = pOldWinAtt->m_prototypeCode;
		fromWinId = pOldWinAtt->m_fromWinId;
	}
	else
	{
		p_bDetailWnd = true;

	}

	AcGePoint3d insertPos;
	{
		AcGePoint3d minPt, maxPt;
		JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt);
		insertPos = minPt;
	}


	AcDbObjectId newId = AcDbObjectId::kNull;
	//��ԭ�ͺ�֮ǰ��ԭ��Ϊͬһ��ԭ�ͣ���ֻ���²���������������
	if (sOldPrototypeCode.IsEmpty()==false && sOldPrototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0)
	{
		const CWindowInsertPara insertPara = InitInsertPara(newWinAtt, insertPos, newWinAtt.m_viewDir, p_winDir);
		//������
		bool bMirror = newWinAtt.m_isMirror;
		if (newWinAtt.m_viewDir == E_VIEW_TOP)
		{
			bMirror = !bMirror; // yuan 1124 ԭ��ƽ��ͼԭ�͵ķ��������ͼì�ܵ�����
		}
		if (bMirror && (newWinAtt.m_isMirrorWindow == false))
		{
			TYCOM_Mirror(p_id, insertPara.mirrorBasePt, insertPara.mirrorAxis);
		}

		//////////////////////////////////////////////////////////////////////////
		RCWindow oneWindow;
		oneWindow.m_id = p_id;
		oneWindow.InitParameters();

		UpdateRcWindowPara(oneWindow, newWinAtt, newWinAtt.m_viewDir, p_bDetailWnd);

		newId = p_id;
	}
	else
	{
		//ɾ��ԭ����id
		JHCOM_DeleteCadObject(p_id);
		newId = GenerateWindow(newWinAtt, insertPos, p_winDir, p_bDetailWnd, fromWinId, p_sLayerName);
	}


	//����ԭ���������Ŵ�
	if (pOldWinAtt!=NULL && bUpdateRelatedWin)
	{
		if (p_originalId==p_id && pOldWinAtt->m_fromWinId != NULL) //ֻ���ڷ�����Ŵ����Ÿ�����Դ�Ŵ�
		{
			UpdateWindow(pOldWinAtt->m_fromWinId, newWinAtt, true, p_originalId);
		}
		else if (pOldWinAtt->m_relatedWinIds.length() > 0)
		{
			for (int i = 0; i < pOldWinAtt->m_relatedWinIds.length(); i++)
			{
				if (pOldWinAtt->m_relatedWinIds[i]== p_originalId) //�������Ŵ�Ϊ�����޸ĵ��Ŵ�����ֱ������
				{
					continue;
				}
				UpdateWindow(pOldWinAtt->m_relatedWinIds[i], newWinAtt, true, p_originalId);
			}
		}
	}

	return newId;
}