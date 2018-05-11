// Panel.cpp: implementation of the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "����˹����.h"
#include "Panel.h"
#include "Manager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMyApp theApp;
extern CManager* pManager;
CPanel::CPanel()
{

}

CPanel::~CPanel()
{

}

void CPanel::DrawPanel(CDC* pDC)
{
	CBrush brush;
	CPen pen(PS_DOT,2,RGB(10,10,255)),GridPen(PS_SOLID,1,RGB(190,190,190));
	brush.CreateSolidBrush(PanelColor);
	pDC->SelectObject(&pen);
	CBrush *pOldBrush=pDC->SelectObject(&brush);
	//DrawBegin

	RECT Rect=PanelRect;
	Rect.bottom=PanelRect.bottom;
	Rect.right=PanelRect.right;
	pDC->Rectangle(&Rect);
	pDC->Rectangle(&TextRect);
	//Text
	CString str;
	pDC->SetTextColor(RGB(255,0,255));
	pDC->SetBkColor(PanelColor);

	str="������ " + pManager->Name;
	pDC->TextOut(TextRect.left+10,TextRect.top+10,str);
	str.Format("�ȼ���        %d",pManager->Level);
	pDC->TextOut(TextRect.left+10,TextRect.top+40,str);
	str.Format("����������%d",pManager->LineNumber);	
	pDC->TextOut(TextRect.left+10,TextRect.top+70,str);
	str.Format("��ֵ��        %d",pManager->Result);
	pDC->TextOut(TextRect.left+10,TextRect.top+100,str);
	//DrawLine
	pDC->SelectObject(&GridPen);
	for(int iy=PanelRect.top;iy<PanelRect.bottom;iy+=GridSize.cy)
	{
		pDC->MoveTo(PanelRect.left,iy);
		pDC->LineTo(PanelRect.right,iy);
	}

	for(int ix=PanelRect.left;ix<PanelRect.right;ix+=GridSize.cx)
	{
		pDC->MoveTo(ix,PanelRect.top);
		pDC->LineTo(ix,PanelRect.bottom);
	}
	//DrawEnd
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

void CPanel::PanelPosToPos(POINT &PanelPos)
{
	PanelPos.x=PanelRect.left+PanelPos.x*GridSize.cx;
	PanelPos.y=PanelRect.top +PanelPos.y*GridSize.cy;
}

void CPanel::PosToPanelPos(POINT &Pos)
{
}