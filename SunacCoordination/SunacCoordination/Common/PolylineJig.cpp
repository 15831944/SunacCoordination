#include "stdafx.h"
#include <dbobjptr.h>
#include "PolylineJig.h"
#include "..\Tool\DocLock.h"

CPolylineJig::CPolylineJig()
{
	m_pPolyline = NULL;
	m_polylineId = AcDbObjectId::kNull;
}


CPolylineJig::~CPolylineJig()
{

}

//���ļ�����Ϊ�����
bool CPolylineJig::Init(vector<AcGePoint3d> & pts)
{
	m_pts = pts;
	AcDbPolyline *pPoly = new AcDbPolyline((UINT) m_pts.size());
	for (UINT i = 0; i < m_pts.size(); i++)
	{
		pPoly->addVertexAt(i, AcGePoint2d(m_pts[i].x, m_pts[i].y));
	}

	CDocLock doclock;
	AcDbBlockTableRecordPointer curSpace(curDoc()->database()->currentSpaceId(), AcDb::kForWrite);
	if (curSpace.openStatus() != Acad::eOk)
		return false;

	Acad::ErrorStatus es = curSpace->appendAcDbEntity(pPoly);
	if (es != Acad::eOk)
	{
		return false;
	}

	m_pPolyline = pPoly;
	m_polylineId = pPoly->objectId();

	return m_polylineId != AcDbObjectId::kNull;
}

void CPolylineJig::Erase(AcDbObjectId entId)
{
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite);
	if (es == Acad::eOk)
	{
		pEnt->erase();
		pEnt->close();
	}
}


bool CPolylineJig::doIt(vector<AcGePoint3d> pts, AcGePoint3d& ptOut)
{
	if (pts.size() == 0)
		return false;

	m_curPoint = pts[pts.size() - 1];
	pts.push_back(m_curPoint); //����һ����Ϊ��ǰ�㣬��ʼֵΪ���һ��

	Init(pts);

	if (m_polylineId.isNull())
		return false;
		
	CString sPrompt = L"\n��ѡ��㣺";
	setDispPrompt(sPrompt);
	AcEdJig::DragStatus stat = drag();
	acedPostCommandPrompt();

	//�϶��������������ز���
	Erase(m_polylineId);
	m_pPolyline = NULL;

	ptOut = m_curPoint;
	return (stat == kNormal);
}


AcEdJig::DragStatus CPolylineJig::sampler()
{
	setUserInputControls(UserInputControls(AcEdJig::kAccept3dCoordinates));

	AcGePoint3d ptTemp;
	DragStatus stat = acquirePoint(ptTemp);
	if (m_curPoint != ptTemp)
		m_curPoint = ptTemp;
	else if (stat == AcEdJig::kNormal)
		return AcEdJig::kNoChange;

	return stat;
}

Adesk::Boolean CPolylineJig::update()
{
	UINT nCount = m_pPolyline->numVerts();
	m_pPolyline->setPointAt(nCount - 1, AcGePoint2d( m_curPoint.x, m_curPoint.y));
	return Adesk::kTrue;
}
AcDbEntity* CPolylineJig::entity() const
{
	return m_pPolyline;
}

