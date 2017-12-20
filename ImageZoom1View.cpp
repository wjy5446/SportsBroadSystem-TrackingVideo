// ImageZoom1View.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ImageZoom1View.h"


// CImageZoom1View

IMPLEMENT_DYNCREATE(CImageZoom1View, CScrollView)

CImageZoom1View::CImageZoom1View()
{
	m_BmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BmiHeader.biPlanes = 1;
	m_BmiHeader.biBitCount = 24;
	m_BmiHeader.biCompression = BI_RGB;
	m_BmiHeader.biXPelsPerMeter = 1000;
	m_BmiHeader.biYPelsPerMeter = 1000;
	m_BmiHeader.biClrUsed = 0;
	m_BmiHeader.biClrImportant = 0;
}

CImageZoom1View::~CImageZoom1View()
{
}


BEGIN_MESSAGE_MAP(CImageZoom1View, CScrollView)
END_MESSAGE_MAP()


// CImageZoom1View 그리기입니다.

void CImageZoom1View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageZoom1View::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CImageZoom1View 진단입니다.

#ifdef _DEBUG
void CImageZoom1View::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageZoom1View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageZoom1View 메시지 처리기입니다.
