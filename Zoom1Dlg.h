#pragma once
#include "afxwin.h"


// CZoom1Dlg 대화 상자입니다.

class CZoom1Dlg : public CDialog
{
	DECLARE_DYNAMIC(CZoom1Dlg)

public:
	CZoom1Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CZoom1Dlg();

	BOOL m_bCreated;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ZOOM1DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_stDispZ1;

	// Image
	BYTE* m_img;

	// function
	void DisplayImage(int IDC_PICTURE_TARGET, BYTE* image);
};