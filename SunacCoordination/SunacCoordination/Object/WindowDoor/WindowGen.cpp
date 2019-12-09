#include "StdAfx.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "WindowGen.h"
#include "..\..\GlobalSetting.h"



//////////////////////////////////////////////////////////////////////////

bool CWindowGen::MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����

	AcGePoint3d mirrorBasePt;
	AcGeVector3d mirrorAxis;

	AcGeVector3d offsetXY(0, 0, 0);
	switch (p_winDir)
	{
	case E_DIR_BOTTOM:
	case E_DIR_TOP:
		mirrorBasePt = AcGePoint3d((minPt.x + maxPt.x) / 2, 0, 0);
		mirrorAxis = AcGeVector3d(0, 1, 0);
		break;
	case E_DIR_RIGHT:
	case E_DIR_LEFT:
		mirrorBasePt = AcGePoint3d(0, (minPt.y + maxPt.y) / 2, 0);
		mirrorAxis = AcGeVector3d(1, 0, 0);
		break;
	default:
		return false;
		break;
	}

	TYCOM_Mirror(p_id, mirrorBasePt, mirrorAxis);

	return true;
}

CWinTranslationPara CWindowGen::InitTransPara(const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir)
{
	CWinTranslationPara insertPara;
	insertPara.insertPos = pos;
	insertPara.rotateAngle = 0;

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
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			insertPara.rotateAngle = PI / 2;
			break;
		case E_DIR_UNKNOWN:
		default:
			break;
		}

		insertPara.insertPos += offsetXY;
	}

	return insertPara;
}

	//���²���
void CWindowGen::UpdateRcWindowPara(const AcDbObjectId p_id, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd)
{
	RCWindow oneWindow;
	oneWindow.m_id = p_id;
	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", curWinAtt.GetH());
	oneWindow.SetParameter(L"W", curWinAtt.GetW());

	if (p_bDetailWnd || GlobalSetting::GetInstance()->m_winSetting.m_bUseAinLimian)
	{
		oneWindow.SetParameter(L"A", curWinAtt.GetA());
	}

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
}

void CWindowGen::AddWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt)
{
	//�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWinAtt = new AttrWindow(p_winAtt);
	TY_AddAttributeData(p_id, pWinAtt);
	pWinAtt->close();
}
void CWindowGen::UpdateWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt)
{
	//�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		*pWinAtt = p_winAtt;
		pWinAtt->close();
	}
}


AcDbObjectId  CWindowGen::GenerateWindow(const AttrWindow& curWinAtt, const AcGePoint3d pos,
	E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName)
{
	eViewDir p_view = curWinAtt.m_viewDir;
	const CWinTranslationPara transPara = InitTransPara(curWinAtt, pos, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, transPara.insertPos, transPara.rotateAngle, p_sLayerName, 256);

	//������
	if (curWinAtt.IsInstanceNeedMirror())
	{
		MirrorObjectByCenter(oneWindow.m_id, p_winDir);
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
	UpdateRcWindowPara(id, curWinAtt, p_view, p_bDetailWnd);
	AddWinAtt(id, curWinAtt);
	

	return id;
}

AcGePoint3d CWindowGen::GetWindowLeftBottomPos(AcDbObjectId p_id)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����
	return minPt;
}

AcGePoint3d CWindowGen::GetWindowInsertPos(AcDbObjectId p_id)
{
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk )
	{
	}
	
	AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);
	if (pBlockReference==NULL)
	{
		return AcGePoint3d::kOrigin;
	}

	AcGePoint3d pos = pBlockReference->position();
	pBlockReference->close();
	return pos;
}

E_DIRECTION CWindowGen::GetWindowInsertDir(AcDbObjectId p_id)
{
	E_DIRECTION winDir = E_DIR_TOP; //TODO �Զ�������ͼ����

	return winDir;
}

CWinInsertPara CWindowGen::GetWindowInsertPara(AcDbObjectId p_id) //�����Ѳ�����Ŵ���ȡ��������Ϣ
{
	CWinInsertPara insertPara;
	insertPara.insertPos = GetWindowInsertPos(p_id);
	insertPara.leftBottomPos = GetWindowLeftBottomPos(p_id);

	insertPara.sLayerName = JHCOM_GetEntityLayer(p_id);
	insertPara.insertDir = GetWindowInsertDir(p_id);

	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		insertPara.viewDir = pWinAtt->m_viewDir;
		insertPara.bDetailWnd = false;
		insertPara.fromWinId = pWinAtt->m_fromWinId;
		insertPara.relatedWinIds = pWinAtt->m_relatedWinIds;
	}
	else
	{
		insertPara.bDetailWnd = true;
		insertPara.viewDir = E_VIEW_EXTEND;
		insertPara.fromWinId = AcDbObjectId::kNull;
	}
	return insertPara;
}

bool CWindowGen::IsWindowMirror(AcDbObjectId p_id) 
{
	//����������½ǵ�
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����

	AcGePoint3d insertPos = GetWindowInsertPos(p_id);
	

	return false; //TODO,�����Ŵ�ʵ���жϵ�ǰ�Ŵ���ͼ���Ƿ���
}
AttrWindow* CWindowGen::GetWinAtt(AcDbObjectId p_id)
{
	AcDbObject* pObj = NULL;
	TY_GetAttributeData(p_id, pObj);
	AttrWindow *pWinAtt = dynamic_cast<AttrWindow *>(pObj);

	return pWinAtt;
}
bool CWindowGen::IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt)
{
	AttrWindow *pOldWinAtt = GetWinAtt(p_id);
	if (pOldWinAtt != NULL)
	{
		pOldWinAtt->close();
		return pOldWinAtt->m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	}
	else
	{
		return true;
	}
}

//ֻ�޸ĳߴ��������Ϣ��������
void CWindowGen::ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt)
{
	if (p_id==AcDbObjectId::kNull)
	{
		assert(false);
		return;
	}
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk || pEnt==NULL)
	{
		return;
	}
	pEnt->close();

	const CWinInsertPara oldInsertPara = GetWindowInsertPara(p_id);
	//������Ϣ���ֺ�ԭ���Ĳ���
	newWinAtt.m_viewDir = oldInsertPara.viewDir; 
	newWinAtt.m_fromWinId = oldInsertPara.fromWinId;
	newWinAtt.m_relatedWinIds = oldInsertPara.relatedWinIds;
	
	//���³ߴ���Ϣ
	UpdateRcWindowPara(p_id, newWinAtt, oldInsertPara.viewDir, oldInsertPara.bDetailWnd);
	UpdateWinAtt(p_id, newWinAtt);

	//������
	if (newWinAtt.IsInstanceNeedMirror() && IsWindowMirror(p_id)==false ||
		newWinAtt.IsInstanceNeedMirror()==false && IsWindowMirror(p_id))
	{
		MirrorObjectByCenter(p_id, GetWindowInsertDir(p_id));
	}
	//����λ��

	const CWinTranslationPara transPara = InitTransPara(newWinAtt, oldInsertPara.insertPos, oldInsertPara.viewDir, oldInsertPara.insertDir);

	AcGePoint3d newLeftBottomPos = GetWindowLeftBottomPos(p_id);
	AcGeVector3d offset = oldInsertPara.leftBottomPos - newLeftBottomPos;
	TYCOM_Move(p_id, offset);
}
AcDbObjectId CWindowGen::UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin)
{
	if (p_id == AcDbObjectId::kNull)
		return p_id;

	const CWinInsertPara insertPara = GetWindowInsertPara(p_id);
	AttrWindow *pOldWinAtt = GetWinAtt(p_id);
	if (pOldWinAtt==NULL)
	{
		JHCOM_DeleteCadObject(p_id);

		AcDbObjectId newId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull, insertPara.sLayerName);
		return newId;
	}
	
	AcDbObjectId fromWinId = AcDbObjectId::kNull;
	AcDbObjectIdArray relatedWinIds;
	if (pOldWinAtt->m_fromWinId ==AcDbObjectId::kNull) //�����Ǹ��ڵ��Ŵ�
	{
		fromWinId = p_id;
		relatedWinIds = pOldWinAtt->m_relatedWinIds;
	}
	else  //�����ǹ������ɵ��Ŵ�
	{
		fromWinId = pOldWinAtt->m_fromWinId;
		AcDbObjectId tempId;
		GetWinRelationIDs(fromWinId, tempId, relatedWinIds);
	}

	//��ԭ��û�иı䣬��ֻ����³ߴ�����
	AcDbObjectId outId = p_id;
	const bool bProtoTypeSame = pOldWinAtt->m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	if (bProtoTypeSame)
	{
		if (bUpdateRelatedWin)
		{
			ModifyOneWindow(fromWinId, newWinAtt);
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				ModifyOneWindow(relatedWinIds[i], newWinAtt);
			}
		}
		else
		{
			ModifyOneWindow(p_id, newWinAtt);
		}
	}
	else //��ԭ�͸ı䣬����Ҫȫ��ɾ������������
	{
		if (bUpdateRelatedWin)
		{
			AcDbObjectId newMainId = AcDbObjectId::kNull;
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(fromWinId);
				JHCOM_DeleteCadObject(fromWinId);

				newMainId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull, newInsertPara.sLayerName);
				if (p_id==fromWinId)
					outId = newMainId;
			}

			AcDbObjectIdArray newRelatedIds;
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(relatedWinIds[i]);
				JHCOM_DeleteCadObject(relatedWinIds[i]);

				AcDbObjectId newId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull, newInsertPara.sLayerName);
				newRelatedIds.append(newId);
				if (p_id == relatedWinIds[i])
					outId = newId;

				AcDbObjectIdArray relatedIds;
				SetWinRelationIDs(newId, newMainId, relatedIds);
			}

			SetWinRelationIDs(newMainId, AcDbObjectId::kNull, newRelatedIds);
		}
		else
		{
			JHCOM_DeleteCadObject(p_id);
			outId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull, insertPara.sLayerName);
		}		
	}	

	return outId;
}

bool CWindowGen::SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds)
{
	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt==NULL)
		return false;

	pWinAtt->m_fromWinId = p_fromWinId;
	pWinAtt->m_relatedWinIds = p_relatedIds;

	pWinAtt->close();

	return true;
}

bool CWindowGen::GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds)
{
	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	p_fromWinId = pWinAtt->m_fromWinId;
	p_relatedIds = pWinAtt->m_relatedWinIds;

	pWinAtt->close();
	return true;

}