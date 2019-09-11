#include "stdafx.h"
#include "TreeCtrlEx.h"
 
void CTreeCtrlEx::UpdateMenuIDRange(CMenu* pMenu)
{
	for (int i = 0; i < pMenu->GetMenuItemCount(); i++)
	{
		CMenu* pSubMenu = pMenu->GetSubMenu(i);
		if (pSubMenu == NULL)
		{
			m_StartID = min(m_StartID, pMenu->GetMenuItemID(i));
			m_EndID = max(m_EndID, pMenu->GetMenuItemID(i));
		}
		else
			UpdateMenuIDRange(pSubMenu);
	}
}

// CMyTreeCtrl
 
IMPLEMENT_DYNAMIC(CTreeCtrlEx, CTreeCtrl)
 
CTreeCtrlEx::CTreeCtrlEx()
{
	m_MenuID = 0;
	m_MenuWidth = 150;
	m_StartID = ID_COMMAND_END;
	m_EndID = ID_COMMAND_START;
}

CTreeCtrlEx::~CTreeCtrlEx()
{

} 


BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND_RANGE(ID_COMMAND_START, ID_COMMAND_END, OnClickedPopupMenu)
END_MESSAGE_MAP()


// CMyTreeCtrl ��Ϣ�������


void CTreeCtrlEx::OnPaint()
{
	CPaintDC dc(this);
	GetClientRect(&m_ClientRect); //�������ݵ��ڴ�DC��λͼ������λͼѡ�뵽�ڴ�DC
	CBitmap bitmap;
	CDC MemeDc;
	MemeDc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc,m_ClientRect.Width(),m_ClientRect.Height());
	CBitmap* pOldBmp = MemeDc.SelectObject(&bitmap);
	DrawBackGround(&MemeDc); //����ݵ��ڴ�DC�л��Ʊ���
	DrawItem(&MemeDc); //��ÿһ��������Ƶ����ؼ���
	dc.BitBlt( m_ClientRect.left, m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), &MemeDc, 0, 0,SRCCOPY);
	MemeDc.SelectObject(pOldBmp);
	MemeDc.DeleteDC();
}

void CTreeCtrlEx::DrawBackGround(CDC* pDC)
{
	pDC->FillSolidRect(m_ClientRect,RGB(255,255,255));
}
 
void CTreeCtrlEx::DrawItem(CDC* pDC)
{
	HTREEITEM hCurrentItem;//���Ƶĵ�ǰ����
	CRect CurItemRect;//��ǰ�������
	int CurItemState;//��ǰ���״̬
 
	hCurrentItem = GetFirstVisibleItem();//��ȡ��һ���ɼ�����,�������ľ��ֵ
	do 
	{
		if (GetItemRect(hCurrentItem,CurItemRect,TRUE))
		{
			CRect fillRect(0,CurItemRect.top,m_ClientRect.right,CurItemRect.bottom);
 
			CurItemState = GetItemState(hCurrentItem,TVIF_STATE);
 
			//��ǰ�����Ƶ����ѳ������ڵı߽磬���Բ����ƣ����˳�����
			if (CurItemRect.bottom > m_ClientRect.bottom)  
			{
				break;
			}
			//��������ȵ�
			if (hCurrentItem == m_CurMouseItem)
			{
				pDC->FillSolidRect(&fillRect, RGB(220,220,255));
			}
			//����ѡ��״̬
			if(CurItemState & TVIS_SELECTED)
			{
				pDC->FillSolidRect(&fillRect, GetSysColor(COLOR_MENUHILIGHT));
			}
			//���Ʒָ���
			if (GetParentItem(hCurrentItem) == NULL)
			{
				CPoint ptTopLeft = fillRect.TopLeft();
				CPoint ptTopRight(fillRect.right, fillRect.top);
				CPoint ptBottomLeft(fillRect.left, fillRect.bottom);
				CPoint ptBottomRight = fillRect.BottomRight();

				pDC->MoveTo(ptTopLeft);
				pDC->LineTo(ptTopRight);
				pDC->MoveTo(ptBottomLeft);
				pDC->LineTo(ptBottomRight);
			}
			//��������
			CurItemRect.right = m_ClientRect.right;
 
			DrawItemText(pDC, hCurrentItem, CurItemRect);
		}
	} while ((hCurrentItem = GetNextVisibleItem(hCurrentItem)) != NULL);
}

void CTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTest(point);//�ж�����ڿؼ�����һ������
 
	if (hItem != NULL && m_CurMouseItem != hItem)
	{
		m_CurMouseItem = hItem;
		Invalidate(FALSE);
	}
 
	CTreeCtrl::OnMouseMove(nFlags, point);
}
 
void CTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hSel = HitTest(point);
	CString sText;

	if (GetParentItem(hSel) == NULL)
	{
		//һ���ڵ�����ѡ��״̬
		SelectItem(hSel);
		//����������
		for (HTREEITEM i = GetFirstVisibleItem(); i != NULL; i = GetNextVisibleItem(i))
		{
			if (hSel != i)
				Expand(i, TVE_COLLAPSE);
		}
		Expand(hSel, TVE_TOGGLE);
		Invalidate(FALSE);
	}
	else
	{
		//�����ڵ����ø���
		m_CurMouseItem = hSel;
		Invalidate(FALSE);

		//�����˵���λ���뵱ǰ�ڵ����
		if (m_MenuID > 0)
		{
			CRect rect;
			GetItemRect(hSel, &rect, FALSE);
			CPoint menuPos(rect.right + 1, rect.top);
			ClientToScreen(&menuPos);

			CMenu menu;
			menu.LoadMenu(m_MenuID);
			//���Ҷ�Ӧ�Ĳ˵�����ʾ
			for (int i = 0; i < menu.GetMenuItemCount(); i++)
			{
				menu.GetMenuStringW(i, sText, MF_BYPOSITION);
				if (sText == GetItemText(hSel))
				{
					CMenu *pMenu = menu.GetSubMenu(i);
					if (pMenu == NULL)
						break;
					UINT menuID = pMenu->GetMenuItemID(0);
					for (int j = 0; j < pMenu->GetMenuItemCount(); j++)
					{
						pMenu->ModifyMenu(menuID + j, MF_BYCOMMAND | MF_OWNERDRAW, menuID + j);
					}
					pMenu->TrackPopupMenu(TPM_LEFTALIGN, menuPos.x, menuPos.y, this);
					break;
				}
			}
		}
	}
}

void CTreeCtrlEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = GetItemHeight();
	lpMeasureItemStruct->itemWidth = m_MenuWidth;
}

void CTreeCtrlEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_MENU)
	{
		CBrush *brush = new CBrush;
		CPen *pen = new CPen;
		CString strText;
		CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC); //��ȡ�˵�����豸���
		//�˵����Ƿ�Ϊѡ��״̬
		if ((lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			//�ڲ˵������Ի���ο�ı�����ɫ
			brush->CreateSolidBrush(RGB(182, 189,210));
			//�ڲ˵����Ի���εı߿���ɫ
			pen->CreatePen(PS_SOLID, 1, RGB(10,36,106));
			//���ò˵�������ֱ�����ɫ
			pDC->SetBkColor(RGB(182,189,210));
		}
		else
		{
			brush->CreateSolidBrush(GetSysColor(COLOR_MENU));
			pen->CreatePen(PS_SOLID, 0, GetSysColor(COLOR_MENU));
			pDC->SetBkColor(GetSysColor(COLOR_MENU));
		}
		pDC->SelectObject(pen);
		pDC->SelectObject(brush);
		//�ڵ�ǰ�˵����ϻ�һ�����ο�
		pDC->Rectangle(lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right,
			lpDrawItemStruct->rcItem.bottom);
		/*--------------------------------------*/
		//��ȡ��ǰ��Ϣ���ڲ˵�����ı�
		CMenu menu;
		menu.Attach((HMENU)lpDrawItemStruct->hwndItem);
		menu.GetMenuStringW(lpDrawItemStruct->itemID, strText,MF_BYCOMMAND);
		/*--------------------------------------*/
		//���Ϊ�˵�������DrawItem��Ϣ
		if (nIDCtl == 0)
		{
			//�ڲ˵���������˵��ı�
			CFont Font;
			Font.CreatePointFont(105,_T("Calibri"));//��������
			pDC->SelectObject(&Font);
			CRect rectText = lpDrawItemStruct->rcItem;
			rectText.left += 20;
			pDC->DrawText(strText, &rectText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
		menu.Detach();
		delete brush;
		delete pen;
	}
}

void CTreeCtrlEx::OnClickedPopupMenu(UINT nID)
{
	if (nID > m_EndID || nID < m_StartID)
		return;
	GetParent()->SendMessage(WM_MENU_CLICKED, nID, 0);
}

void CTreeCtrlEx::DrawItemText(CDC * pDC,HTREEITEM pItem,CRect Rect)
{
	CFont Font;
	Font.CreatePointFont(105,_T("Calibri"));//��������
	pDC->SelectObject(&Font);
	CString ItemText = GetItemText(pItem);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(ItemText, Rect, DT_SINGLELINE|DT_LEFT|DT_VCENTER);//��ʾ���ı�
}

void CTreeCtrlEx::SetMenuID(UINT menuID)
{
	CMenu menu;
	menu.LoadMenu(menuID);
	UpdateMenuIDRange(&menu);
	m_MenuID = menuID;
}
