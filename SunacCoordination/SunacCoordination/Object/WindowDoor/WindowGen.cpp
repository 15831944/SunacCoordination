#include "StdAfx.h"
#include <dbobjptr.h>
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "..\..\Common\ComFun_Math.h"
#include "..\..\GlobalSetting.h"
#include "..\..\Tool\DocLock.h"
#include "WindowGen.h"
#include "WindowAutoName.h"
#include "WindowSelect.h"
#include "..\..\Src\DocumentData.h"
#include "..\..\Tangent\TangentOpen.h"


//////////////////////////////////////////////////////////////////////////

bool CWindowGen::MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况
	

#if 1 
	//通过设置块引用的缩放因子镜像，此方法和CAD自带的镜像操作相同
	AcDbEntity * pEnt = 0;
	CDocLock docLock; //锁定文档
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

	//镜像后需要移动到原来的位置
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
			rotateAngle = 0;
			break;
		case E_DIR_RIGHT:
			rotateAngle = PI / 2;
			break;
		case E_DIR_TOP:
			rotateAngle = PI;
			break;
		case E_DIR_LEFT:
			rotateAngle = -PI / 2;
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
	double D = curWinAtt.GetD();
	
	AcGePoint3d insertPos = pos;
	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:	//南
		case E_DIR_TOP:		//北
			offsetXY.x += W/2;
			offsetXY.y += D /2;
			break;
		case E_DIR_RIGHT:	//东
		case E_DIR_LEFT:	//西
			offsetXY.x += D /2;
			offsetXY.y += W / 2;
			break;
			break;
		default:
			break;
		}

		insertPos += offsetXY;
	}

	return insertPos;
}

	//更新参数
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
	//处理可见性
	if (p_view == E_VIEW_FRONT || p_view == E_VIEW_EXTEND)
	{
		double B = -100; //设为无效值
		
		if (curWinAtt.GetType() == S_DOOR)
		{
			B = 1100;
		}
		if (curWinAtt.m_heightUnderWindow != 0)
		{
			if (curWinAtt.m_openType == L"外开" || curWinAtt.m_openType == L"推拉")
			{
				B = 1650 - curWinAtt.m_heightUnderWindow;
			}
			else if(curWinAtt.m_openType == L"内开")
			{
				if (curWinAtt.m_isBayWindow == true)
				{
					if (abs(curWinAtt.m_heightUnderWindow - 600) < TOL)//不等于600
					{
						B = 1950 - curWinAtt.m_heightUnderWindow;
					}
					else if (curWinAtt.m_heightUnderWindow > TOL)//不等于0
					{
						B = 1800 - curWinAtt.m_heightUnderWindow;
					}
				}
				else B = 1650 - curWinAtt.m_heightUnderWindow;

				if (B > 0) 
				{
					if (B < curWinAtt.GetH() - 200)//若门把手过高，则不设置门把手高度
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
		DQ_SetDynamicAttribute(oneWindow.m_id, _T("可见性1"), p_bDetailWnd ? _T("详图") : _T("立面"));
	}
}

void CWindowGen::AddWinAtt(const AcDbObjectId p_id, const AttrWindow&  p_winAtt)
{
	//把数据记录在图框的扩展字典中
	AttrWindow * pWinAtt = new AttrWindow(p_winAtt);
	TY_AddAttributeData(p_id, pWinAtt);
	pWinAtt->close();
}
void CWindowGen::UpdateWindowsAttribute(const AcDbObjectId p_id, const AttrWindow& p_winAtt)
{
	CDocLock lock;
	//把数据记录在图框的扩展字典中
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id, false);
	if (pWinAtt!=NULL)
	{
		pWinAtt->Clone(p_winAtt); //只更新数据，不更新关联关系
		pWinAtt->close();
	}
}

bool CWindowGen::GetNearTangentWall(const AcGePoint3d pos, E_DIRECTION p_winDir, AcGePoint3d& p_posOnWall, double& p_wallThick)
{
	bool bHDir = p_winDir == E_DIR_BOTTOM || p_winDir == E_DIR_TOP;
	p_posOnWall = pos;
	TYRect nearRect(AcGePoint3d(pos.x - 100, pos.y - 100, 0), AcGePoint3d(pos.x + 100, pos.y + 100, 0));
	AcDbObjectIdArray nearObjIds = GetIdsCrossRect(nearRect);
	for (int i = 0; i < nearObjIds.length(); i++)
	{
		CTWallData walldata;
		HRESULT hr = CTangentOpen::GetTangentWallData(nearObjIds[i], walldata);
		if (SUCCEEDED(hr))
		{
			p_wallThick = walldata.thick;

			AcGePoint3d minPt = walldata.extents.minPoint();
			AcGePoint3d maxPt = walldata.extents.maxPoint();
			double h = maxPt.y - minPt.y;
			bool bWallH = JHCOM_equ(h, walldata.thick, 1);
			if (bWallH==bHDir) //必须是和门窗方向相同的墙体
			{
				if (bWallH)
					p_posOnWall.y = minPt.y;
				else
					p_posOnWall.x = minPt.x;
				return true;
			}
		}
	}

	return false;
}

AcDbObjectId  CWindowGen::GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId)
{
	AcGePoint3d posOnWall = pos;
	bool bFindTWall = false;
	if (curWinAtt.GetViewDir() == E_VIEW_TOP &&
		GlobalSetting::GetWinSetting()->m_bDrawTangentOpen) //只有平面图才绘制门洞
	{
		double wallThick = 200;
		bFindTWall = GetNearTangentWall(pos, p_winDir, posOnWall, wallThick);
		if (bFindTWall)
		{
			curWinAtt.SetD(wallThick);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	CString p_sLayerName = GlobalSetting::GetWindowBlockLayer();
	const eViewDir p_view = curWinAtt.GetViewDir();
	const double rotateAngle = GetBlockRotateAngle(curWinAtt, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, AcGePoint3d::kOrigin, rotateAngle, p_sLayerName, 256); //先插入原点，再移动

	UpdateRcWindowPara(id, curWinAtt, p_view, p_bDetailWnd);
	AddWinAtt(id, curWinAtt);

	//移动窗型到插入点
	AcGePoint3d insertPos = GetBlockInsertPos(id, curWinAtt, posOnWall, p_view, p_winDir);
	TYCOM_Move(id, insertPos.asVector());

	//处理镜像
	if (curWinAtt.IsMxMirror())
	{
		MirrorObjectByCenter(oneWindow.m_id, p_winDir);
	}

	//创建并移动门窗编号
	if (p_bDetailWnd == false)
	{
		if (GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber || p_view == E_VIEW_TOP)
		{
			CSunacObjInCad winInCad;
			winInCad.m_winId = id;
			winInCad.m_rootId = id;
			winInCad.m_bMxMirror = curWinAtt.IsMxMirror();

			AcDbEntity * pEnt = 0;
			Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
			if (es == Acad::eOk)
			{
				pEnt->getCompoundObjectTransform(winInCad.m_mx);
				AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
				if (pBRef != NULL)
					winInCad.m_rotateAngle = pBRef->rotation();
				pEnt->close();
			}
			
			CreateWindowDoorCode(p_view, winInCad, curWinAtt.GetInstanceCode());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//针对USC坐标处理
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}
	
	//将生成的门窗加到门窗编号库中
	if (id != AcDbObjectId::kNull && p_bDetailWnd==false)
	{
		GetWindowAutoName()->AddWindowType(curWinAtt, id);
	}

	//插入天正门洞(因在生成门窗appand到模型空间时尚未添加门窗属性，因此不能在反应器中自动天正门洞，需要在生成时添加门洞）
	if (bFindTWall)
	{
		DrawTangentOpen(id, curWinAtt, posOnWall, p_winDir);
	}

	//刷新
	actrTransactionManager->flushGraphics();//必须lock住文档才有效果 
	acedUpdateDisplay();

	return id;
}

bool CWindowGen::DrawTangentOpen(AcDbObjectId p_winId, const AttrWindow& curWinAtt, const AcGePoint3d pos, E_DIRECTION p_winDir)//绘制天正门洞
{
	if (curWinAtt.GetViewDir() !=E_VIEW_TOP) //只有平面图才绘制门洞
		return true;

	if (GlobalSetting::GetWinSetting()->m_bDrawTangentOpen == false)
		return true;

	AcGePoint3d centerPt;
	if (p_winDir==E_DIR_BOTTOM || p_winDir==E_DIR_TOP)
	{
		centerPt = AcGePoint3d(pos.x + curWinAtt.GetW() / 2, pos.y + curWinAtt.GetD() / 2, 0);
	}
	else
	{
		centerPt = AcGePoint3d(pos.x + curWinAtt.GetD() / 2, pos.y + curWinAtt.GetW() / 2, 0);
	}

	CTOpenData tWinData;
	tWinData.width = curWinAtt.GetW();
	tWinData.height = curWinAtt.GetH();
	tWinData.bottomHeight = curWinAtt.GetHeightUnderWindow();
	tWinData.sWinCode = curWinAtt.GetInstanceCode();

	AcDbObjectId tWinOpenIdOut = AcDbObjectId::kNull;
	HRESULT hr = CTangentOpen::InsertWinOpenning(centerPt, tWinData, tWinOpenIdOut);

	//添加和门窗的关联
	const AttrWindow* pWinAtt = AttrWindow::GetWinAtt(p_winId, true);
	if (pWinAtt!=NULL)
	{
		GetWinTangentOpenMap()->AddWindow(p_winId, tWinOpenIdOut);
	}

	return hr == Acad::eOk;
}

AcGePoint3d CWindowGen::GetWindowLeftBottomPos(AcDbObjectId p_id)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况
	return minPt;
}

bool CWindowGen::GetWindowInsertPos(AcDbObjectId p_id, AcGePoint3d &p_insertPos, double& p_angle)
{
	p_insertPos = AcGePoint3d::kOrigin;
	p_angle = 0;

	AcDbObjectPointer<AcDbBlockReference> pRef(p_id, AcDb::kForRead);
	if (pRef.openStatus() == Acad::eOk)
	{
		p_insertPos = pRef->position();
		p_angle = pRef->rotation();
	}
	return true;
}

bool CWindowGen::GetWindowInsertDir(AcDbObjectId p_id, E_DIRECTION & p_dirOut)
{
	p_dirOut = E_DIR_TOP; 

	AcDbObjectPointer<AcDbBlockReference> pBRef(p_id, AcDb::kForRead);
	if (pBRef.openStatus() != Acad::eOk)
		return false;

	double rotateAngle = pBRef->rotation();
	if (rotateAngle > 0.25*PI && rotateAngle < 0.75*PI)
		p_dirOut = E_DIR_RIGHT;
	else if (rotateAngle > 0.75*PI && rotateAngle < 1.25*PI)
		p_dirOut = E_DIR_TOP;
	else if (rotateAngle > 1.25*PI && rotateAngle < 1.75*PI)
		p_dirOut = E_DIR_LEFT;
	else
		p_dirOut = E_DIR_BOTTOM;

	return true;
}

CWinInsertPara CWindowGen::GetWindowInsertPara(AcDbObjectId p_id) //根据已插入的门窗获取其插入的信息
{
	CWinInsertPara insertPara;

	JHCOM_GetObjectMinMaxPoint(p_id, insertPara.leftBottomPos, insertPara.rightTopPos);

	insertPara.sLayerName = JHCOM_GetEntityLayer(p_id);
	GetWindowInsertDir(p_id, insertPara.insertDir);

	const AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt!=NULL)
	{
		insertPara.viewDir = pWinAtt->GetViewDir();
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

	//////////////////////////////////////////////////////////////////////////
	//根据方位，重新得到插入点： 窗户外墙左侧点
	double angle;
	GetWindowInsertPos(p_id, insertPara.insertPos, angle);

	if (insertPara.viewDir==E_VIEW_TOP)
	{
		switch (insertPara.insertDir)
		{
		case E_DIR_BOTTOM:	//下侧、南方
		case E_DIR_TOP:		//上侧、北方
			insertPara.insertPos = AcGePoint3d(insertPara.insertPos.x - pWinAtt->GetW()/2, insertPara.rightTopPos.y - pWinAtt->GetD()/2, 0);
		case E_DIR_LEFT:	//左侧、西方
		case E_DIR_RIGHT:	//右侧、东方
			insertPara.insertPos = AcGePoint3d(insertPara.insertPos.x - pWinAtt->GetD() / 2, insertPara.rightTopPos.y - pWinAtt->GetW() / 2, 0);
			break;
		default:
			break;
		}
	}

	return insertPara;
}


bool CWindowGen::IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt)
{
	const AttrWindow *pOldWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pOldWinAtt != NULL)
	{
		return pOldWinAtt->m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	}
	else
	{
		return true;
	}
}

//只修改尺寸等数据信息，不调整
void CWindowGen::ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt)
{
	if (p_id==AcDbObjectId::kNull)
	{
		assert(false);
		return;
	}

	const CWinInsertPara oldInsertPara = GetWindowInsertPara(p_id);
	//以下信息保持和原来的不变
	newWinAtt.SetViewDir(oldInsertPara.viewDir);
	newWinAtt.SetFromWinId(oldInsertPara.fromWinId);
	newWinAtt.SetRelatedWinIds(oldInsertPara.relatedWinIds);
	
	//更新尺寸信息
	UpdateRcWindowPara(p_id, newWinAtt, oldInsertPara.viewDir, oldInsertPara.bDetailWnd);
	UpdateWindowsAttribute(p_id, newWinAtt);

	//处理镜像
	bool bPreMxMirror = CSunacSelect::IsReferenctMirror(p_id);
	if (newWinAtt.IsMxMirror()!= bPreMxMirror)
	{
		E_DIRECTION dirOut = E_DIR_TOP;
		GetWindowInsertDir(p_id,  dirOut);
		MirrorObjectByCenter(p_id, dirOut);
	}

	//更新位置
	AcGePoint3d newLeftBottomPos = GetWindowLeftBottomPos(p_id);
	AcGeVector3d offset = oldInsertPara.leftBottomPos - newLeftBottomPos;
	TYCOM_Move(p_id, offset);

	//////////////////////////////////////////////////////////////////////////
	//修改后，重新更新到名称库中
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
		//如果修改的属性已经存在对应的编号，则用新编号，否则还是使用原来的编号
		if (CWindowAutoName::IsSamePrototypeAndSize(oldInstanceCode, newWinAtt.GetInstanceCode()) &&
			GetWindowAutoName()->IsInstanceCodeExist(newWinAtt.GetInstanceCode())==false)
		{
			newWinAtt.SetInstanceCode(oldInstanceCode); 
		}

		//1 找到和当前编号一样的门窗
		vector<AcDbObjectId> sameCodeWins = GetWindowAutoName()->GetAllIdsByInstantCode(oldInstanceCode);

		//2 移除当前编号的门窗
		GetWindowAutoName()->RemoveObjectsByInstantCode(oldInstanceCode);

		//3. 更新当前门窗
		vector<AcDbObjectId> newCodeWins;
		AcDbObjectId idOut = UpdateOneWindow(p_id, newWinAtt, false); //全部更新不需要关联更新，会全部更新
		newCodeWins.push_back(idOut);

		//4. 更新其他和当前编号一致的门窗
		for (UINT i = 0; i < sameCodeWins.size(); i++)
		{
			if (sameCodeWins[i]==p_id)
				continue;

			AcDbObjectId idOutTemp = UpdateOneWindow(sameCodeWins[i], newWinAtt, false); //全部更新就不用关联更新了
			if (idOutTemp !=AcDbObjectId::kNull)
			{
				newCodeWins.push_back(idOutTemp);
			}
		}

		//5. 更新后的门窗加入到门窗命名库中
		GetWindowAutoName()->AddWindowType(newWinAtt, newCodeWins);

		//若不是镜像原型，则对镜像的门窗配套修改
		if (false==newWinAtt.m_isMirrorWindow) 
		{
			CString oldInstanceCodeMirror = CWindowAutoName::GetMirrorInstanceCode(oldInstanceCode);
			CString newInstanceCoceMirror = CWindowAutoName::GetMirrorInstanceCode(newWinAtt.GetInstanceCode());
			AttrWindow newWinAttMirror = newWinAtt;
			newWinAttMirror.m_isMirror = !(newWinAtt.m_isMirror);
			newWinAttMirror.SetInstanceCode(newInstanceCoceMirror);

			//1 找到和当前编号一样的门窗
			vector<AcDbObjectId> sameCodeWinsMirror = GetWindowAutoName()->GetAllIdsByInstantCode(oldInstanceCodeMirror);

			//2 移除当前编号的门窗
			GetWindowAutoName()->RemoveObjectsByInstantCode(oldInstanceCodeMirror);

			//3 更新其他和当前编号一致的门窗
			vector<AcDbObjectId> newCodeWinsMirror;
			for (UINT i = 0; i < sameCodeWinsMirror.size(); i++)
			{
				AcDbObjectId idOutTemp = UpdateOneWindow(sameCodeWinsMirror[i], newWinAttMirror, false); //全部更新就不用关联更新了
				if (idOutTemp != AcDbObjectId::kNull)
				{
					newCodeWinsMirror.push_back(idOutTemp);
				}
			}

			//4. 更新后的门窗加入到门窗命名库中
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
	const AttrWindow *pOldWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pOldWinAtt==NULL) //门窗详图等情况则直接删除之前的门窗
	{
		DeleteWindowObj(p_id);

		AcDbObjectId newId = GenerateWindow(newWinAtt, insertPara.insertPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull);
		return newId;
	}

	newWinAtt.SetViewDir(pOldWinAtt->GetViewDir());

	AttrWindow oldWinAtt = *pOldWinAtt;

	AcDbObjectId fromWinId = AcDbObjectId::kNull;
	AcDbObjectIdArray relatedWinIds;
	if (oldWinAtt.GetFromWinId() == AcDbObjectId::kNull) //自身是父节点门窗
	{
		fromWinId = p_id;
		relatedWinIds = oldWinAtt.GetRelatedWinIds();
	}
	else  //自身是关联生成的门窗
	{
		fromWinId = oldWinAtt.GetFromWinId();
		AcDbObjectId tempId;
		GetWinRelationIDs(fromWinId, tempId, relatedWinIds);
	}

	//若原型没有改变，则只需更新尺寸数据
	AcDbObjectId outId = p_id;
	const bool bProtoTypeSame = oldWinAtt.m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	if (bProtoTypeSame) //任然是原来的原型，只是修改了参数
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
	else //若原型改变，则需要全部删除和重新生成
	{
		if (bUpdateRelatedWin) //同时更新关联的门窗
		{
			AcDbObjectId newMainId = AcDbObjectId::kNull;
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(fromWinId);
				DeleteWindowObj(fromWinId);

				newWinAtt.SetViewDir(newInsertPara.viewDir);
				newMainId = GenerateWindow(newWinAtt, newInsertPara.insertPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull);
				if (p_id==fromWinId)
					outId = newMainId;
			}

			AcDbObjectIdArray newRelatedIds;
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				if (IsObjectExsit(relatedWinIds[i])==false)
				{
					continue;
				}

				const CWinInsertPara newInsertPara = GetWindowInsertPara(relatedWinIds[i]);
				DeleteWindowObj(relatedWinIds[i]);

				newWinAtt.SetViewDir(newInsertPara.viewDir);
				AcDbObjectId newId = GenerateWindow(newWinAtt, newInsertPara.insertPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull);
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
			outId = GenerateWindow(newWinAtt, insertPara.insertPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull);
		}		
	}	

	return outId;
}

bool CWindowGen::RenameWindow(const AcDbObjectId p_id, CString p_sNewName, const bool bUpdateRelatedWin) //只是重命名，不更改其他
{
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id, false);
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
	pWinAtt->close();
	return true;
}

bool CWindowGen::DeleteWindowObj(const AcDbObjectId p_id)
{
	const AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt != NULL)
	{
		if (pWinAtt->GetInstanceCodeObjectId() != AcDbObjectId::kNull)
		{
			JHCOM_DeleteCadObject(pWinAtt->GetInstanceCodeObjectId());
		}
	}

	JHCOM_DeleteCadObject(p_id);

	GetWindowAutoName()->RemoveObject(p_id);
	GetInstanceCodeMrg()->RemoveInstanceCodeByWinId(p_id);
	return true;
}
bool CWindowGen::SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds)
{
	AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id, false);
	if (pWinAtt==NULL)
		return false;

	pWinAtt->SetFromWinId( p_fromWinId);
	pWinAtt->SetRelatedWinIds( p_relatedIds);

	pWinAtt->close();

	return true;
}

bool CWindowGen::GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds)
{
	const AttrWindow *pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt == NULL)
		return false;

	p_fromWinId = pWinAtt->GetFromWinId();
	p_relatedIds = pWinAtt->GetRelatedWinIds();

	return true;

}

//插入门窗编号
AcDbObjectId CWindowGen::InsertWindowDoorCode(eViewDir p_viewDir, CString p_number, AcGePoint3d p_pos)
{
	//调整门窗编号文字为水平居中显示
	CDocLock doclock;

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

	//门窗编号插入点
	AcDbObjectId sWindowDoorTextId = JHCOM_CreateText(p_pos,
		AcGeVector3d(0, 0, 1),
		GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize, 0,
		p_number);

	MD2010_SetCurrentLayer(oldLayerName);

	return sWindowDoorTextId;
}

void CWindowGen::CreateWindowDoorCode(eViewDir p_viewDir, CSunacObjInCad p_win, CString p_Code)
{
	//若已经存在，则直接修改原来的编号值
	bool bSuc = GetInstanceCodeMrg()->SetNewInstanceCode(p_win, p_Code);
	if (bSuc) 
		return;
	
	//根据窗户的位置转换门窗编号位置
	double offsetX = 0;
	double offsetY = 0;
	double angle = 0;
	if (p_viewDir == E_VIEW_TOP)
	{
		offsetX = 0;
		offsetY = -GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize -100;

		if (p_win.m_rotateAngle>PI / 4 && p_win.m_rotateAngle<PI * 3 / 4 ||
			p_win.m_rotateAngle>PI * 5 / 4 && p_win.m_rotateAngle < PI * 7 / 4)
		{
			angle = (PI / 2);
		}
	}
	else
	{
		double winWidth, winHeight = 0;
		GetWinWidthHeight(p_win.m_winId, winWidth, winHeight);
		offsetX = winWidth / 2;
		offsetY = winHeight / 2 - GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize; //立面图居中显示编号
	}

	AcGePoint3d textPos(offsetX, offsetY, 0);
	textPos.transformBy(p_win.m_mx);

	AcDbObjectId textId = InsertWindowDoorCode(p_viewDir, p_Code, textPos);
	GetInstanceCodeMrg()->AddInstanceCode(p_win, textId);	

	CDocLock doclock;
	//对门窗编号位置进行细微调整，使其居中，间距适中
	AcDbObjectPointer<AcDbText> pText(textId, AcDb::kForWrite);
	if (pText.openStatus() == Acad::eOk)
	{
		pText->setRotation(angle);
		pText->setHorizontalMode(AcDb::kTextMid);
		pText->setAlignmentPoint(textPos);
	}
}

bool CWindowGen::GetWinWidthHeight(AcDbObjectId p_id, double& p_widthOut, double& p_heightOut)
{
	AcGePoint3d minWindowPt, maxWindowPt;
	Acad::ErrorStatus es = JHCOM_GetObjectMinMaxPoint(p_id, minWindowPt, maxWindowPt);
	if (es != Acad::eOk)
		return false;

	p_widthOut = maxWindowPt.x - minWindowPt.x;
	p_heightOut = maxWindowPt.y - minWindowPt.y;
	return true;
}

bool CWindowGen::SelectSunacObjs(vector<CSunacObjInCad>& p_winsOut, vector<AcDbObjectId>& p_textIdsOut, bool &p_bAllOut)
{
	CString sDir = _T("R");
	bool bSuc = GetStringInput(_T("\n按区域选择[R],全部选择[A]<R>:"), sDir);
	if (bSuc == false)
		return false;

	p_bAllOut = false;
	sDir.Trim();
	if (sDir.CompareNoCase(_T("A")) == 0)
	{
		p_bAllOut = true;
	}
	else if (sDir.CompareNoCase(_T("R")) == 0 || sDir.IsEmpty())
	{
		p_bAllOut = false;
	}
	else
	{
		return false;
	}

	if (p_bAllOut)
	{
		p_winsOut = CSunacSelect::SelectSunacObjs(S_WINDOW, E_VIEW_ALL, true);
		p_textIdsOut = CInstanceCodeTextMrg::GetAllInstanceCodeIds();
	}
	else
	{
		TYRect rect;
		bSuc = TY_GetOneRect(rect);
		if (bSuc == false)
			return false;

		//////////////////////////////////////////////////////////////////////////
		p_winsOut = CSunacSelect::SelectSunacObjsByRect(S_WINDOW, E_VIEW_ALL, rect);

		p_textIdsOut = CInstanceCodeTextMrg::GetInstanceCodeIdsInRect(rect);
	}

	return true;
}

void CWindowGen::AutoNameAllWindow()
{
	CDocLock lock;
	vector<CSunacObjInCad> wins;
	vector<AcDbObjectId> textIds;
	bool bAll = false;
	//1.  从CAD界面上获取所有的门窗
	bool bSuc = SelectSunacObjs(wins, textIds, bAll);
	if (wins.size() == 0)
		return;	

	//3. 将门窗加入到类型库
	for (UINT i = 0; i < wins.size(); i++)
	{
		//3.1 对所有的门窗获取名称
		AttrWindow* pWinAtt = AttrWindow::GetWinAtt(wins[i].m_winId, false);
		if (pWinAtt==NULL)
			continue;

		//3.2 判断最终的门窗引用是否镜像
		pWinAtt->SetMxMirror(wins[i].m_bMxMirror);
		CString sInstanceCode = GetWindowAutoName()->GetWindowName(*pWinAtt);
		pWinAtt->SetInstanceCode(sInstanceCode);

		//3.3 对所有的门窗添加到名称库中
		//GetWindowAutoName()->AddWindowType(sInstanceCode, wins[i].m_winId);
		GetWindowAutoName()->AddWindowType(*pWinAtt, wins[i].m_winId);

		//////////////////////////////////////////////////////////////////////////
		//4 对门窗文字编号更新， 实际的门窗可能是块中块，需要用最终的镜像结果
		AttrWindow winAtt = *pWinAtt;
		winAtt.SetMxMirror(wins[i].m_bMxMirror);
		CString sInstanceCode2 = GetWindowAutoName()->GetWindowName(winAtt);
		winAtt.SetInstanceCode(sInstanceCode2);

		//门窗编号生成
		if (winAtt.GetViewDir() == E_VIEW_TOP ||  //平面图必须编号
			GSINST->m_winSetting.m_bShowLimianNumber && winAtt.GetViewDir() != E_VIEW_EXTEND) //立面图是否编号取决于设置
		{
			CreateWindowDoorCode(winAtt.GetViewDir(), wins[i], sInstanceCode2);
		}

		pWinAtt->close();
	}

	//////////////////////////////////////////////////////////////////////////
	//2. 对原来的门窗分类有效性进行检查，移除不匹配和已删除的项
	//for (UINT i = 0; i < textIds.size(); i++)
	//{
	//	if (false == GetInstanceCodeMrg()->IsTextIdIn(textIds[i]))
	//		JHCOM_DeleteCadObject(textIds[i]);
	//}
	GetWindowAutoName()->CheckAndRemoveObjectNotBelong();

	acutPrintf(L"\n门窗自动编号完成\n");
}