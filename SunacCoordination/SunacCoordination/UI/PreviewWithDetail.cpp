#include "StdAfx.h"
#include "PreviewWithDetail.h"


BEGIN_MESSAGE_MAP(CPreviewWithDetail, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


CPreviewWithDetail::CPreviewWithDetail() : CStatic(), m_pGsPreviewCtrl(NULL), m_bSelected(false)
{
}

CPreviewWithDetail::~CPreviewWithDetail()
{
	delete m_pGsPreviewCtrl;
}

void CPreviewWithDetail::SetLayoutMode(PREVIEW_LAYOUT_DIR dir, double rate)
{
	CRect innerRect;
	GetClientRect(&innerRect);
	innerRect.DeflateRect(5, 5);
	if (dir == PREVIEW_LAYOUT_HORIZONTAL)
	{
		CRect recPreview(innerRect), recText(innerRect);
		recText.left += long(innerRect.Width() * rate) + 5;
		recPreview.right = recText.left - 5;
		SetPreviewRect(recPreview);
		SetTextRect(recText);
	}
	else
	{
		CRect recPreview(innerRect), recText(innerRect);
		recText.top += long(innerRect.Height() * rate) + 5;
		recPreview.bottom = recText.top - 5;
		SetPreviewRect(recPreview);
		SetTextRect(recText);
	}
}

void CPreviewWithDetail::SetPreview(CString sPath)
{
	if (m_pGsPreviewCtrl==NULL)
	{
		m_pGsPreviewCtrl = new CGsPreviewCtrl;
		m_pGsPreviewCtrl->Create(_T(""), WS_CHILD | WS_VISIBLE, m_recPreview, this);
		m_pGsPreviewCtrl->Init(theArxDLL.ModuleResourceInstance(), true);
		m_pGsPreviewCtrl->SetMouseWheel(false);
	}
	m_pGsPreviewCtrl->SetDwgFile(sPath);
}

bool CPreviewWithDetail::SetPreviewImage(CString p_sFileName)
{
	if (!m_img.IsNull())
		m_img.Destroy();

	if (m_img.Load(p_sFileName) == S_OK)
	{
		m_sFileName = p_sFileName;
		return true;
	}
	else
		return false;
}

void CPreviewWithDetail::SetSelected(bool bSelected)
{
	if (m_bSelected != bSelected)
	{
		m_bSelected = bSelected;
		Invalidate();
	}
}

void CPreviewWithDetail::DrawText(CDC* pDC)
{
	CRect innerRect(m_recText);
	int radius = 10;
	innerRect.DeflateRect(radius, radius);

	//���������ɫ
	CBrush brush;
	if (m_bSelected)
		brush.CreateSysColorBrush(COLOR_HIGHLIGHT);
	else
		brush.CreateSysColorBrush(NULL_BRUSH);
	pDC->SelectObject(&brush);

	//����Բ�Ǿ���
	pDC->RoundRect(m_recText, CPoint(radius * 2, radius * 2));

	//��������
	pDC->SetBkMode(TRANSPARENT);
	CFont Font;
	Font.CreatePointFont(80,_T("Calibri"));
	pDC->SelectObject(&Font);
	if (m_bSelected)
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(m_sText, innerRect, DT_EDITCONTROL | DT_WORDBREAK);
}

void CPreviewWithDetail::DrawBackGround(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	if (m_bSelected)
		pDC->FillSolidRect(clientRect, GetSysColor(COLOR_HIGHLIGHT));
	else
		pDC->FillSolidRect(clientRect, RGB(255,255,255));
}

void CPreviewWithDetail::OnPaint()
{
	CPaintDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	CBitmap bitmap;
	CDC MemeDc;
	MemeDc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	CBitmap* pOldBmp = MemeDc.SelectObject(&bitmap);
	DrawBackGround(&MemeDc);

	if (!m_img.IsNull())
	{
		MemeDc.SetStretchBltMode(HALFTONE);
		m_img.Draw(MemeDc, m_recPreview);
	}

	DrawText(&MemeDc);
	dc.BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), &MemeDc, 0, 0,SRCCOPY);
	MemeDc.SelectObject(pOldBmp);
	MemeDc.DeleteDC();

	if (m_pGsPreviewCtrl!=NULL)
	{
		m_pGsPreviewCtrl->Invalidate();
	}
}
