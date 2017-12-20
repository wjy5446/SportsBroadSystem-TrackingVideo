// Zoom1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "Zoom1Dlg.h"
#include "ImageProcessingDoc.h"
#include "afxdialogex.h"


// CZoom1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CZoom1Dlg, CDialog)

CZoom1Dlg::CZoom1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoom1Dlg::IDD, pParent)
{
	m_bCreated = false;

	m_img = new BYTE[100];

	for (int i = 0; i < 0; i++)
		m_img[i] = 0;
}

CZoom1Dlg::~CZoom1Dlg()
{
	delete[] m_img;
}

void CZoom1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ST_DISP_Z1, m_stDispZ1);
}


BEGIN_MESSAGE_MAP(CZoom1Dlg, CDialog)
END_MESSAGE_MAP()


// CZoom1Dlg 메시지 처리기입니다.
