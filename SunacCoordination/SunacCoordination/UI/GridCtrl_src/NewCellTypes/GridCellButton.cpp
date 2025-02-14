#include "stdafx.h"  
#include "../../GridCtrl_src/GridCell.h"  
#include "../../GridCtrl_src/GridCtrl.h"  
#include "GridCellButton.h"  


#ifdef _DEBUG  
#define new DEBUG_NEW  
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;
#endif  


IMPLEMENT_DYNCREATE(CGridCellButton, CGridCell)
	CGridCellButton::CGridCellButton(void)
{
	m_bPushing = FALSE;
}
CGridCellButton::~CGridCellButton(void)
{
}
BOOL CGridCellButton::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /* = TRUE */)
{
	m_rect = rect;
	pDC->SetBkMode(TRANSPARENT);
	rect.DeflateRect(GetMargin(), 0);
	CFont* pOldFont = pDC->SelectObject(GetFontObject());
	pDC->DrawFrameControl(rect, DFC_BUTTON, m_bPushing ? DFCS_BUTTONPUSH | DFCS_PUSHED : DFCS_BUTTONPUSH);
	COLORREF ColorCurrent = pDC->GetTextColor();
	pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	pDC->DrawText(GetText(), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	pDC->SetTextColor(ColorCurrent);
	return TRUE;
}
void CGridCellButton::OnClickDown(CPoint PointCellRelative)
{
	m_oriRect = m_rect;
	m_bPushing = !m_bPushing;
	GetGrid()->InvalidateRect(m_rect);
}

void CGridCellButton::OnClick(CPoint PointCellRelative)
{
	m_bPushing = FALSE;
	GetGrid()->InvalidateRect(m_oriRect);
}

void CGridCellButton::OnMouseOver()
{
	/*TRACKMOUSEEVENT   tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;
	//tme.hwndTrack = m_hWnd;
	_TrackMouseEvent(&tme);*/
	m_bPushing = FALSE;
	GetGrid()->InvalidateRect(m_oriRect);

}

void CGridCellButton::OnMouseLeave()
{
	m_bPushing = FALSE;
	GetGrid()->InvalidateRect(m_oriRect);
}
