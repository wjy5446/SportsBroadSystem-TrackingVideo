#pragma once



// CImageZoom1View 뷰입니다.

class CImageZoom1View : public CScrollView
{
	DECLARE_DYNCREATE(CImageZoom1View)

protected:
	CImageZoom1View();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CImageZoom1View();

public:
	BITMAPINFOHEADER m_BmiHeader;	// 화면 출력을 위한 비트맵 헤더


#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
};


