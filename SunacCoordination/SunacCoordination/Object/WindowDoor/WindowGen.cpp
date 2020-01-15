#include "StdAfx.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "..\..\GlobalSetting.h"
#include "..\..\Tool\DocLock.h"
#include "WindowGen.h"
#include "WindowAutoName.h"
#include "WindowSelect.h"
#include "..\..\Src\DocumentData.h"


//////////////////////////////////////////////////////////////////////////

bool CWindowGen::MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����
	

#if 1 
	//ͨ�����ÿ����õ��������Ӿ��񣬴˷�����CAD�Դ��ľ��������ͬ
	AcDbEntity * pEnt = 0;
	CDocLock docLock; //�����ĵ�
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForWrite);
	if (es != Acad::eOk || pEnt == NULL)
	{
		assert(false);
		return false;
	}

	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
	if (pBRef!=NULL)
	{
		AcGeScale3d  scale = pBRef->scaleFactors();
		bool bPreMirror = scale.sx < 0;
		scale.sx = -scale.sx;
		pBRef->setScaleFactors(scale);
	}
	pEnt->close();

	//�������Ҫ�ƶ���ԭ����λ��
	AcGePoint3d minPt2, maxPt2;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt2, maxPt2);

	AcGeVector3d offset = minPt - minPt2;
	TYCOM_Move(p_id, offset);

#else
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
#endif

	return true;
}

double CWindowGen::GetBlockRotateAngle(const AttrWindow& curWinAtt, eViewDir p_view, E_DIRECTION p_winDir)
{
	double rotateAngle = 0;
	if (p_view == E_VIEW_TOP)
	{
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			rotateAngle = PI;
			break;
		case E_DIR_RIGHT:
			rotateAngle = -PI / 2;
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			rotateAngle = PI / 2;
			break;
		case E_DIR_UNKNOWN:
		default:
			break;
		}
	}

	return rotateAngle;
}

AcGePoint3d CWindowGen::GetBlockInsertPos(const AcDbObjectId p_id, const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir)
{
	double W = curWinAtt.GetW();

	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����
	W = (maxPt.x - minPt.x);

	AcGePoint3d insertPos = pos;
	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			offsetXY.x += W;
			break;
		case E_DIR_RIGHT:
			W = (maxPt.y - minPt.y);
			offsetXY.y += W;
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			break;
		default:
			break;
		}

		insertPos += offsetXY;
	}

	return insertPos;
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

	//oneWindow.RunParameters();
	//////////////////////////////////////////////////////////////////////////
	//����ɼ���
	if (p_view == E_VIEW_FRONT || p_view == E_VIEW_EXTEND)
	{
		double B = -100; //��Ϊ��Чֵ
		
		if (curWinAtt.GetType() == DOOR)
		{
			B = 1100;
		}
		if (curWinAtt.m_heightUnderWindow != 0)
		{
			if (curWinAtt.m_openType == L"�⿪" || curWinAtt.m_openType == L"����")
			{
				B = 1650 - curWinAtt.m_heightUnderWindow;
			}
			else if(curWinAtt.m_openType == L"�ڿ�")
			{
				if (curWinAtt.m_isBayWindow == true)
				{
					if (abs(curWinAtt.m_heightUnderWindow - 600) < TOL)//������600
					{
						B = 1950 - curWinAtt.m_heightUnderWindow;
					}
					else if (curWinAtt.m_heightUnderWindow > TOL)//������0
					{
						B = 1800 - curWinAtt.m_heightUnderWindow;
					}
				}
				else B = 1650 - curWinAtt.m_heightUnderWindow;

				if (B > 0) 
				{
					if (B < curWinAtt.GetH() - 200)//���Ű��ֹ��ߣ��������Ű��ָ߶�
					{
						oneWindow.SetParameter(L"B", B);
					}
				}

			}			
		}
		
	}
	oneWindow.RunParameters();
	if (p_view == E_VIEW_FRONT || p_view == E_VIEW_EXTEND)
	{
		DQ_SetDynamicAttribute(oneWindow.m_id, _T("�ɼ���1"), p_bDetailWnd ? _T("��ͼ") : _T("����"));
	}
}

void CWindowGen::AddWinAtt(const AcDbObjectId p_id, const AttrWindow&  p_winAtt)
{
	//�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWinAtt = new AttrWindow(p_winAtt);
	TY_AddAttributeData(p_id, pWinAtt);
	pWinAtt->close();
}
void CWindowGen::UpdateWindowsAttribute(const AcDbObjectId p_id, const AttrWindow& p_winAtt)
{
	//�����ݼ�¼��ͼ�����չ�ֵ���
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		pWinAtt->Clone(p_winAtt); //ֻ�������ݣ������¹�����ϵ
		pWinAtt->close();
	}
}

AcDbObjectId  CWindowGen::GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId)
{
	CDocLock cLock;

	CString p_sLayerName = GlobalSetting::GetWindowBlockLayer();
	const eViewDir p_view = curWinAtt.m_viewDir;
	const double rotateAngle = GetBlockRotateAngle(curWinAtt, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, AcGePoint3d::kOrigin, rotateAngle, p_sLayerName, 256); //�Ȳ���ԭ�㣬���ƶ�

	UpdateRcWindowPara(id, curWinAtt, p_view, p_bDetailWnd);
	AddWinAtt(id, curWinAtt);

	//�ƶ�����
	AcGePoint3d insertPos = GetBlockInsertPos(id, curWinAtt, pos, p_view, p_winDir);
	TYCOM_Move(id, insertPos.asVector());

	//������
	if (curWinAtt.IsInstanceNeedMirror())
	{
		MirrorObjectByCenter(oneWindow.m_id, p_winDir);
	}

	//�������ƶ��Ŵ����
	if (p_bDetailWnd == false)
	{
		if (GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber || p_view == E_VIEW_TOP)
		{
			vector<CWinInCad> wins = CWindowSelect::SelectWindows(E_VIEW_ALL, true);
			if (wins.size() != 0)
				MoveWindowDoorCode(p_view, wins[0], curWinAtt.GetInstanceCode());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//���USC���괦��
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}

	

	//�����ɵ��Ŵ��ӵ��Ŵ���ſ���
	if (id != AcDbObjectId::kNull && p_bDetailWnd==false)
	{
		GetWindowAutoName()->AddWindowType(curWinAtt, id);
	}

	return id;
}

AcGePoint3d CWindowGen::GetWindowLeftBottomPos(AcDbObjectId p_id)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO ����ucs�����µ����
	return minPt;
}

bool CWindowGen::GetWindowInsertPos(AcDbObjectId p_id, AcGePoint3d &p_insertPos, double& p_angle)
{
	p_insertPos = AcGePoint3d::kOrigin;
	p_angle = 0;

	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk )
	{
		return false;
	}
	
	AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);
	if (pBlockReference==NULL)
	{
		return false;
	}

	p_insertPos = pBlockReference->position();
	p_angle = pBlockReference->rotation();
	pBlockReference->close();
	return true;
}

bool CWindowGen::GetWindowInsertDir(AcDbObjectId p_id, E_DIRECTION & p_dirOut)
{
	p_dirOut = E_DIR_TOP; 

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		assert(false);
		return false;
	}

	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
	if (pBRef == NULL)
	{
		pEnt->close();
		return false;
	}
	
	double rotateAngle = pBRef->rotation();

	if ((rotateAngle > -0.25*PI / 4 && rotateAngle < 0.25*PI) || rotateAngle>1.75*PI)
		p_dirOut = E_DIR_TOP;
	else if (rotateAngle > 0.25*PI &&rotateAngle < 0.75*PI)
		p_dirOut = E_DIR_LEFT;
	else if (rotateAngle > 0.75*PI &&rotateAngle < 1.25*PI)
		p_dirOut = E_DIR_BOTTOM;
	else if (rotateAngle > 1.25*PI &&rotateAngle < 1.75*PI)
		p_dirOut = E_DIR_RIGHT;

	pEnt->close();

	return true;
}

CWinInsertPara CWindowGen::GetWindowInsertPara(AcDbObjectId p_id) //�����Ѳ�����Ŵ���ȡ��������Ϣ
{
	CWinInsertPara insertPara;

	double angle;
	GetWindowInsertPos(p_id, insertPara.insertPos, angle);
	insertPara.leftBottomPos = GetWindowLeftBottomPos(p_id);

	insertPara.sLayerName = JHCOM_GetEntityLayer(p_id);
	GetWindowInsertDir(p_id, insertPara.insertDir);

	const AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		insertPara.viewDir = pWinAtt->m_viewDir;
		insertPara.bDetailWnd = false;
		insertPara.fromWinId = pWinAtt->GetFromWinId();
		insertPara.relatedWinIds = pWinAtt->GetRelatedWinIds();
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

	AcGePoint3d insertPos;
	double angle;
	GetWindowInsertPos(p_id, insertPos, angle);

	//ͨ�������ж�

	//�����������������½ǣ�����
	

	return false; //TODO,�����Ŵ�ʵ���жϵ�ǰ�Ŵ���ͼ���Ƿ���
}

bool CWindowGen::IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt)
{
	AttrWindow *pOldWinAtt = AttrWindow::GetWinAtt(p_id);
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
	UpdateWindowsAttribute(p_id, newWinAtt);

	//������
	if (newWinAtt.IsInstanceNeedMirror() && IsWindowMirror(p_id)==false ||
		newWinAtt.IsInstanceNeedMirror()==false && IsWindowMirror(p_id))
	{
		E_DIRECTION dirOut = E_DIR_TOP;
		GetWindowInsertDir(p_id,  dirOut);
		MirrorObjectByCenter(p_id, dirOut);
	}

	//����λ��
	AcGePoint3d newLeftBottomPos = GetWindowLeftBottomPos(p_id);
	AcGeVector3d offset = oldInsertPara.leftBottomPos - newLeftBottomPos;
	TYCOM_Move(p_id, offset);

	//////////////////////////////////////////////////////////////////////////
	//�޸ĺ����¸��µ����ƿ���
	GetWindowAutoName()->UpdateObject(p_id);
}

AcDbObjectId CWindowGen::UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin, bool bUpdateSameInstanceCode)
{
	const CString oldInstanceCode = AttrWindow::GetWinInstanceCode(p_id);
	if (bUpdateSameInstanceCode==false || oldInstanceCode.IsEmpty())
	{
		return UpdateOneWindow(p_id, newWinAtt, bUpdateRelatedWin);
	}
	else
	{
		//����޸ĵ������Ѿ����ڶ�Ӧ�ı�ţ������±�ţ�������ʹ��ԭ���ı��
		if (CWindowAutoName::IsSamePrototypeAndSize(oldInstanceCode, newWinAtt.GetInstanceCode()) &&
			GetWindowAutoName()->IsInstanceCodeExist(newWinAtt.GetInstanceCode())==false)
		{
			newWinAtt.SetInstanceCode(oldInstanceCode); 
		}

		//1 �ҵ��͵�ǰ���һ�����Ŵ�
		vector<AcDbObjectId> sameCodeWins = GetWindowAutoName()->GetAllIdsByInstantCode(oldInstanceCode);
		//2 �Ƴ���ǰ��ŵ��Ŵ�
		GetWindowAutoName()->RemoveObjectsByInstantCode(oldInstanceCode);

		//3. ���µ�ǰ�Ŵ�
		vector<AcDbObjectId> newCodeWins;
		AcDbObjectId idOut = UpdateOneWindow(p_id, newWinAtt, false); //ȫ�����²���Ҫ�������£���ȫ������
		newCodeWins.push_back(idOut);

		//4. ���������͵�ǰ���һ�µ��Ŵ�
		for (UINT i = 0; i < sameCodeWins.size(); i++)
		{
			if (sameCodeWins[i]==p_id)
				continue;

			AcDbObjectId idOutTemp = UpdateOneWindow(sameCodeWins[i], newWinAtt, false); //ȫ�����¾Ͳ��ù���������
			if (idOutTemp !=AcDbObjectId::kNull)
			{
				newCodeWins.push_back(idOutTemp);
			}
		}

		//5. ���º���Ŵ����뵽�Ŵ���������
		GetWindowAutoName()->AddWindowType(newWinAtt, newCodeWins);

		//�����Ǿ���ԭ�ͣ���Ծ�����Ŵ������޸�
		if (false==newWinAtt.m_isMirrorWindow) 
		{
			CString oldInstanceCodeMirror = CWindowAutoName::GetMirrorInstanceCode(oldInstanceCode);
			CString newInstanceCoceMirror = CWindowAutoName::GetMirrorInstanceCode(newWinAtt.GetInstanceCode());
			AttrWindow newWinAttMirror = newWinAtt;
			newWinAttMirror.SetInstanceCode(newInstanceCoceMirror);

			//1 �ҵ��͵�ǰ���һ�����Ŵ�
			vector<AcDbObjectId> sameCodeWinsMirror = GetWindowAutoName()->GetAllIdsByInstantCode(oldInstanceCodeMirror);

			//2 �Ƴ���ǰ��ŵ��Ŵ�
			GetWindowAutoName()->RemoveObjectsByInstantCode(oldInstanceCodeMirror);

			//3 ���������͵�ǰ���һ�µ��Ŵ�
			vector<AcDbObjectId> newCodeWinsMirror;
			for (UINT i = 0; i < sameCodeWinsMirror.size(); i++)
			{
				AcDbObjectId idOutTemp = UpdateOneWindow(sameCodeWinsMirror[i], newWinAttMirror, false); //ȫ�����¾Ͳ��ù���������
				if (idOutTemp != AcDbObjectId::kNull)
				{
					newCodeWinsMirror.push_back(idOutTemp);
				}
			}

			//4. ���º���Ŵ����뵽�Ŵ���������
			GetWindowAutoName()->AddWindowType(newWinAttMirror, newCodeWinsMirror);
		}

		return idOut;
	}
}
AcDbObjectId CWindowGen::UpdateOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin)
{
	if (p_id == AcDbObjectId::kNull)
		return p_id;

	if (IsObjectExsit(p_id)==false)
	{
		return AcDbObjectId::kNull;
	}

	const CWinInsertPara insertPara = GetWindowInsertPara(p_id);
	AttrWindow *pOldWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pOldWinAtt==NULL) //�Ŵ���ͼ�������ֱ��ɾ��֮ǰ���Ŵ�
	{
		DeleteWindowObj(p_id);

		AcDbObjectId newId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull);
		return newId;
	}

	const AttrWindow oldWinAtt = *pOldWinAtt;
	pOldWinAtt->close();
	
	AcDbObjectId fromWinId = AcDbObjectId::kNull;
	AcDbObjectIdArray relatedWinIds;
	if (oldWinAtt.GetFromWinId() ==AcDbObjectId::kNull) //�����Ǹ��ڵ��Ŵ�
	{
		fromWinId = p_id;
		relatedWinIds = oldWinAtt.GetRelatedWinIds();
	}
	else  //�����ǹ������ɵ��Ŵ�
	{
		fromWinId = oldWinAtt.GetFromWinId();
		AcDbObjectId tempId;
		GetWinRelationIDs(fromWinId, tempId, relatedWinIds);
	}

	//��ԭ��û�иı䣬��ֻ����³ߴ�����
	AcDbObjectId outId = p_id;
	const bool bProtoTypeSame = oldWinAtt.m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
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
				DeleteWindowObj(fromWinId);

				newMainId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull);
				if (p_id==fromWinId)
					outId = newMainId;
			}

			AcDbObjectIdArray newRelatedIds;
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(relatedWinIds[i]);
				DeleteWindowObj(relatedWinIds[i]);

				AcDbObjectId newId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull);
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
			DeleteWindowObj(p_id);
			outId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull);
		}		
	}	

	return outId;
}

bool CWindowGen::RenameWindow(const AcDbObjectId p_id, CString p_sNewName, const bool bUpdateRelatedWin) //ֻ��������������������
{
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	pWinAtt->SetInstanceCode(p_sNewName);

	if (bUpdateRelatedWin)
	{
		AcDbObjectIdArray ids = pWinAtt->GetRelatedWinIds();
		for (int i = 0; i < ids.length(); i++)
		{
			RenameWindow(ids[i], p_sNewName, true);
		}
	}
	return true;
}

bool CWindowGen::DeleteWindowObj(const AcDbObjectId p_id)
{
	AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt!=NULL &&pWinAtt->GetInstanceCodeObjectId()!=AcDbObjectId::kNull)
	{
		JHCOM_DeleteCadObject(pWinAtt->GetInstanceCodeObjectId());
		pWinAtt->close();
	}

	JHCOM_DeleteCadObject(p_id);

	GetWindowAutoName()->RemoveObject(p_id);
	return true;
}
bool CWindowGen::SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds)
{
	AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt==NULL)
		return false;

	pWinAtt->m_fromWinId = p_fromWinId;
	pWinAtt->m_relatedWinIds = p_relatedIds;

	pWinAtt->close();

	return true;
}

bool CWindowGen::GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds)
{
	AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	p_fromWinId = pWinAtt->GetFromWinId();
	p_relatedIds = pWinAtt->GetRelatedWinIds();

	pWinAtt->close();
	return true;

}

//�����Ŵ����
AcDbObjectId CWindowGen::InsertWindowDoorCode(eViewDir p_viewDir, CString p_number, AcGePoint3d p_pos)
{
	//�����Ŵ��������Ϊˮƽ������ʾ
	acDocManager->lockDocument(curDoc());

	AcDbObjectId sWindowDoorTextId;
	CString oldLayerName;
	CString sWindowDoorLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;

	MD2010_GetCurrentLayer(oldLayerName);

	if (p_viewDir == E_VIEW_TOP)
	{
		if (sWindowDoorLayerName.GetLength() && JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
		}
	}
	else
	{
		sWindowDoorLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian;

		if (sWindowDoorLayerName.GetLength() && JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
			JHCOM_SetLayerPrint(sWindowDoorLayerName);
		}
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	//�Ŵ���Ų����
	sWindowDoorTextId = JHCOM_CreateText(p_pos,
		AcGeVector3d(0, 0, 1),
		GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize, 0,
		p_number);

	MD2010_SetCurrentLayer(oldLayerName);

	acDocManager->unlockDocument(curDoc());

	return sWindowDoorTextId;
}

void CWindowGen::MoveWindowDoorCode(eViewDir p_viewDir, CWinInCad p_win, CString p_Code)
{
	//���ݴ�����λ��ת���Ŵ����λ��
	double winWidth = GetWinLength(GetWinWidth(p_win.m_winId), GetWinHeight(p_win.m_winId));

	AcGePoint3d textPos(winWidth / 2, GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize, 0);
	textPos.transformBy(p_win.m_mx);

	AcDbObjectId textId = InsertWindowDoorCode(p_viewDir, p_Code, textPos);
	GetInstanceCodeMrg()->AddInstanceCode(p_win.m_rootId, textId);

	//���Ŵ����λ�ý���ϸ΢������ʹ����У��������
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, textId, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		AcGeVector3d offsetXYZ = AcGeVector3d(0, 0, 0);
		AcDbText *pText = AcDbText::cast(pEnt);
		if (pText == NULL)
		{
			assert(false);
		}
		if (p_viewDir == E_VIEW_TOP)
		{
			E_DIRECTION winDir = E_DIR_TOP;
			GetWindowInsertDir(p_win.m_winId, winDir);

			switch (winDir)
			{
			case E_DIR_BOTTOM:
				pText->setRotation(p_win.m_rotateAngle - PI);
				offsetXYZ = AcGeVector3d(0, -20, 0);
				break;
			case E_DIR_TOP:
				pText->setRotation(p_win.m_rotateAngle);
				offsetXYZ = AcGeVector3d(0, 20, 0);
				break;
			case E_DIR_RIGHT:
				pText->setRotation(p_win.m_rotateAngle - PI);
				offsetXYZ = AcGeVector3d(20, 0, 0);
				break;
			case E_DIR_LEFT:
				pText->setRotation(p_win.m_rotateAngle);
				offsetXYZ = AcGeVector3d(-20, 0, 0);
				break;
			case E_DIR_UNKNOWN:
				break;
			default:
				break;
			}
		}
		else
		{
			offsetXYZ = AcGeVector3d(0, -2*GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize - 20, 0);
		}
		pText->setHorizontalMode(AcDb::kTextMid);
		pText->setAlignmentPoint(textPos);
		pText->close();

		TYCOM_Move(textId, offsetXYZ);
	}
}

double CWindowGen::GetWinWidth(AcDbObjectId p_id)
{
	AcGePoint3d minWindowPt, maxWindowPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minWindowPt, maxWindowPt);
	return maxWindowPt.x - minWindowPt.x;
}

double CWindowGen::GetWinHeight(AcDbObjectId p_id)
{
	AcGePoint3d minWindowPt, maxWindowPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minWindowPt, maxWindowPt);
	return maxWindowPt.y - minWindowPt.y;
}

double CWindowGen::GetWinLength(double p_winWidth, double p_winHeight)
{
	if (p_winWidth >= p_winHeight)
		return p_winWidth;
	else
		return p_winHeight;
}

void CWindowGen::AutoNameAllWindow()
{
	//TODO ����ѡ��ķ�Χ�ڵ��Ŵ����н��б��

	GetInstanceCodeMrg()->RemoveAll();

	//1.  ��CAD�����ϻ�ȡ���е��Ŵ�
	const vector<CWinInCad> wins = CWindowSelect::SelectWindows(E_VIEW_ALL, true);
	if (wins.size() == 0)
		return;

	//2. ��ԭ�����Ŵ�������Ч�Խ��м�飬�Ƴ���ƥ�����ɾ������
	GetWindowAutoName()->CheckAndRemoveObjectNotBelong();

	//3. ���Ŵ����뵽���Ϳ�
	for (UINT i = 0; i < wins.size(); i++)
	{
		//3.1 �����е��Ŵ���ȡ����
		AttrWindow* pWinAtt = AttrWindow::GetWinAtt(wins[i].m_winId);
		if (pWinAtt==NULL)
			continue;

		//3.2 �ж����յ��Ŵ������Ƿ���
		bool bMirror = CWindowSelect::IsReferenctMirror(wins[i].m_winId);
		pWinAtt->m_isMirror = wins[i].m_bMirror;
		CString sInstanceCode = GetWindowAutoName()->GetWindowName(*pWinAtt);
		pWinAtt->SetInstanceCode(sInstanceCode);

		//3.3 �����е��Ŵ���ӵ����ƿ���
		//GetWindowAutoName()->AddWindowType(sInstanceCode, wins[i].m_winId);
		GetWindowAutoName()->AddWindowType(*pWinAtt, wins[i].m_winId);

		//////////////////////////////////////////////////////////////////////////
		//4 ���Ŵ����ֱ�Ÿ��£� ʵ�ʵ��Ŵ������ǿ��п飬��Ҫ�����յľ�����
		AttrWindow winAtt = *pWinAtt;
		winAtt.m_isMirror = wins[i].m_bMirror;
		CString sInstanceCode2 = GetWindowAutoName()->GetWindowName(winAtt);
		winAtt.SetInstanceCode(sInstanceCode2);

		//�Ŵ��������
		if (winAtt.m_viewDir == E_VIEW_TOP || GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber && winAtt.m_viewDir != E_VIEW_EXTEND)
		{
			MoveWindowDoorCode(winAtt.m_viewDir, wins[i], sInstanceCode2);
		}
	}

	acutPrintf(L"\n�Ŵ��Զ�������\n");
}