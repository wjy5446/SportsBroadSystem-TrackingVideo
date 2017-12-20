#if !defined(AFX_MAINPROCESSDLG_H__9D0BB200_9920_4FE1_84D5_8B823B5BAA47__INCLUDED_)
#define AFX_MAINPROCESSDLG_H__9D0BB200_9920_4FE1_84D5_8B823B5BAA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainProcessDlg.h : header file
//

#include <vfw.h>

#include "SR.h"
#include "Fruc.h"
#include "Stitch.h"
#include "MyFRUC.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "opencv2\core\core.hpp"
#include "opencv2\video\tracking.hpp"
#include "opencv2\gpu\gpu.hpp"
#include "Kalman.h"

using namespace cv;

#define TID_SEQUENCE_RUN		0
#define TID_AVI_RUN				1
#define TID_YUV_RUN				2 
#define TID_CAM_RUN				3
#define TID_MYAVI_RUN			4
#define TID_MYYUV_RUN			5
/////////////////////////////////////////////////////////////////////////////
// CMainProcessDlg dialog

class CMainProcessDlg : public CDialog
{
// Construction
public:
	CMainProcessDlg(CWnd* pParent = NULL);   // standard constructor

	CImageList m_ImageList;
	int m_nCurrentProcessingNum;

	bool m_bSequenceTimerRun, m_bCamTimerRun, m_bAVITimerRun, m_bYUVTimerRun;
	bool m_bTimerPause;
	bool m_bFirstFrame;

	void SelFolder();
	void SelAvi();
	void SelYuv();

	void SequenceRun();
	void AviRun();
	void YuvRun();

	void ReadFile(CString SelFolder);
	void ReadFile(CString SelFolder, bool bSubFolder);

	HWND m_hWndCap;
	BITMAPINFO	m_BmInfo;
	BYTE **m_CurrentImageRed, **m_CurrentImageGreen, **m_CurrentImageBlue;
	int m_nMW, m_nMH;

	int m_nYuvW, m_nYuvH, m_nYuvType;
	CString m_YuvFileName;
	CFile m_YuvFile;
	int m_nYuvLength;

	bool m_bGrabOn;
	int m_nElapsedTime;

	int m_nSequenceLength;

	_int64 m_TimeFreq, m_TimeStart, m_TimeEnd;

	bool m_bInitSaveAvi;
	PAVISTREAM m_psCompressedForSave, m_psForSave;
	PAVIFILE m_pfileForSave;
	BITMAPINFOHEADER m_BmInfoForSaveAvi;
	int m_nSaveAviW, m_nSaveAviH;
		
	bool AVIFileInitFunction();

	CSR m_Sr;
	CFruc m_Fruc;
	CStitch m_Stitch;
	MyFRUC m_MyFRUC;

	// VALUE FOR VIDEO STITCH
	CString m_AviFileName_L;
	PAVIFILE m_pavi_L;
	AVIFILEINFO m_fi_L;
	PAVISTREAM m_pstm_L;
	AVISTREAMINFO m_si_L;
	PGETFRAME m_pfrm_L;
	int m_nAviLength_L;

	CString m_AviFileName_R;
	PAVIFILE m_pavi_R;
	AVIFILEINFO m_fi_R;
	PAVISTREAM m_pstm_R;
	AVISTREAMINFO m_si_R;
	PGETFRAME m_pfrm_R;
	int m_nAviLength_R;


	// Zoom 1에 대한 변수
	CString m_AviFileName_Zoom1;
	PAVIFILE m_pavi_Zoom1;
	AVIFILEINFO m_fi_Zoom1;
	PAVISTREAM m_pstm_Zoom1;
	AVISTREAMINFO m_si_Zoom1;
	PGETFRAME m_pfrm_Zoom1;
	int m_nAviLength_Zoom1;

	// Zoom 2에 대한 변수
	CString m_AviFileName_Zoom2;
	PAVIFILE m_pavi_Zoom2;
	AVIFILEINFO m_fi_Zoom2;
	PAVISTREAM m_pstm_Zoom2;
	AVISTREAMINFO m_si_Zoom2;
	PGETFRAME m_pfrm_Zoom2;
	int m_nAviLength_Zoom2;

	// Zoom 3에 대한 변수
	CString m_AviFileName_Zoom3;
	PAVIFILE m_pavi_Zoom3;
	AVIFILEINFO m_fi_Zoom3;
	PAVISTREAM m_pstm_Zoom3;
	AVISTREAMINFO m_si_Zoom3;
	PGETFRAME m_pfrm_Zoom3;
	int m_nAviLength_Zoom3;


	int m_ListIdx_Video;

// Dialog Data
	//{{AFX_DATA(CMainProcessDlg)
	enum { IDD = IDD_MAIN_PROCESS };
	CComboBox	m_SeqTypeCtrl;
	CButton	m_SourceType0Ctrl;
	CButton	m_SourceType1Ctrl;
	CButton	m_SourceType2Ctrl;
	CEdit	m_GwCtrl;
	CEdit	m_GhCtrl;
	CListBox	m_ListCtrl;
	CSliderCtrl	m_SleepCtrl;
	CListCtrl	m_FileListCtrl;
	CButton m_Radio_Method1;
	CButton m_Radio_Method2;
	CButton m_Radio_Method3;

	int		m_nStart;
	int		m_nEnd;
	int		m_nGW;
	int		m_nGH;
	int		m_nFileType;
	int		m_nGap;
	int		m_nSleep;
	int		m_nSourceType;
	BOOL	m_bSubFolder;
	BOOL	m_bStep;
	int		m_nSeqType;
	BOOL	m_bSaveScreen;
	int		m_nZoomScale;
	int		m_nSearch;
	int		m_nGrow;
	int		m_nMask;
	BOOL	m_bViewPsnr;
	int		m_nRunType;
	int		m_nPsnrType;
	int		m_nUseColor;
	int		m_nMeGap;
	int		m_nThreDiff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainProcessDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPause();
	afx_msg void OnSelFolder2();
	afx_msg void OnGrab();
	afx_msg void OnDestroy();
	afx_msg void OnVs();
	afx_msg void OnCamOn();
	afx_msg void OnResetGNum();
	afx_msg void OnCustomdrawSleep(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelSourceLeft();
	afx_msg void OnSelSourceRight();
	afx_msg void OnRun();
	afx_msg void OnSaveScreen();
	afx_msg void OnBnClickedSelSourceZoom();
	afx_msg void OnBnClickedSelSourceZoom2();
	afx_msg void OnBnClickedSelSourceZoom3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	//=====================================================================================================================================================================
	//=====================================================================================================================================================================
	//=====================================================================================================================================================================

	CListCtrl m_List_Video;
	CButton m_CHECK_DEBUG;

	//====이미지 정보==========

	//input image's info
	BYTE* m_frm1D_L;
	Mat m_MImgLCurColor, m_MImgLCurGray;
	Mat m_MImgLPreGray;
	int m_nHL, m_nWL;

	BYTE* m_frm1D_R;
	Mat m_MImgRCurColor, m_MImgRCurGray;
	Mat m_MImgRPreGray;
	int m_nHR, m_nWR;

	// total Image's info
	Mat m_MImgLTransColor;
	Mat m_MImgLTransGray;

	Mat m_MImgRTransColor;
	Mat m_MImgRTransGray;

	BYTE** m_ImgWarpR, **m_ImgWarpG, **m_ImgWarpB;
	Mat m_MImgWarp, m_MImgWarpGray;
	int m_nH_Total, m_nW_Total;

	// zoom image's info
	BYTE* m_frm1D_Z1;
	Mat m_MImgZ1CurColor, m_MImgZ1CurGray;
	int m_nHZ1, m_nWZ1;

	BYTE* m_frm1D_Z2;
	Mat m_MImgZ2CurColor, m_MImgZ2CurGray;
	int m_nHZ2, m_nWZ2;

	BYTE* m_frm1D_Z3;
	Mat m_MImgZ3CurColor, m_MImgZ3CurGray;
	int m_nHZ3, m_nWZ3;

	//====호모그래피 정보==========
	
	// CP
	Mat m_CPL_cur, m_CPL_pre;
	Mat m_CML, m_CML_pre;
	Mat m_warpCPL;
	Mat m_warpCPL_scale;

	Mat m_CPR_cur, m_CPR_pre;
	Mat m_CMR, m_CMR_pre;
	Mat m_warpCPR;
	Mat m_warpCPR_scale;

	Mat m_BC;

	vector<Point2f> m_arrPtPreL, m_arrPtPreR; // save Feature Points 
	vector<Point2f> m_arrPtPreZ;
	vector<Point2f> arrMatchL, arrMatchR;

	// CP of Zoom
	Mat m_warpHomoZ;

	// ROI Image
	Mat m_ROI1, m_scaleZoom1;
	int m_nWROI1, m_nHROI1;
	int m_nW_scaleZoom1, m_nH_scaleZoom1;
	Mat m_homoZ1;
	Point2f m_posROI1;
	Point2f pt_target1[4]; // Zoom1 영상의 크기
	Point2f pt_target1Trans[4]; // Zoom1 영상에 Homo 적용
	CPoint m_ptTarget1, m_ptCorrect1;

	Mat m_ROI2, m_scaleZoom2;
	int m_nWROI2, m_nHROI2;
	int m_nW_scaleZoom2, m_nH_scaleZoom2;
	Mat m_homoZ2;
	Point2f m_posROI2;
	Point2f pt_target2[4]; // Zoom2 영상의 크기
	Point2f pt_target2Trans[4]; // Zoom2 영상에 Homo 적용
	CPoint m_ptTarget2, m_ptCorrect2;

	Mat m_ROI3, m_scaleZoom3;
	int m_nWROI3, m_nHROI3;
	int m_nW_scaleZoom3, m_nH_scaleZoom3;
	Mat m_homoZ3;
	Point2f m_posROI3;
	Point2f pt_target3[4]; // Zoom3 영상의 크기
	Point2f pt_target3Trans[4]; // Zoom3 영상에 Homo 적용
	CPoint m_ptTarget3, m_ptCorrect3;

	// Blend
	Mat m_MImgMask, m_MImgEdgeMask, m_MImgMaskSmall;
	Mat m_MImgLTransMask, m_MImgRTransMask;
	Mat m_MImgEdgeLTransMask, m_MImgEdgeRTransMask;
	Mat m_MImgLTransMaskSmall, m_MImgRTransMaskSmall;

	Mat m_MImgLBlendMask, m_MImgRBlendMask;
	Mat m_MImgLBlendColor, m_MImgRBlendColor;

	Mat m_MImgBlend, m_MImgBlendMask;

	int m_offsetX_Blend, m_offsetY_Blend;
	int m_nH_Blend, m_nW_Blend;

	//
	CKalman m_kalman1, m_kalman2, m_kalman3;


	// control
	bool m_bDebug;
	bool m_bEnlarge;
	bool m_bStopView;
	bool m_bMultiBandBlend;
	bool m_bFileStream;
	bool m_bStitchScore;
	bool m_bParallel;

	enum LR{ LEFT, RIGHT, ZOOM };

	// time
	int64 TimeFreq;
	int64 TimeStart_Total, TimeEnd_Total;
	int64 TimeStart_EstimateHomo, TimeEnd_EstimateHomo;
	int64 TimeStart_CorrectHomo, TimeEnd_CorrectHomo;
	int64 TimeStart_Warp, TimeEnd_Warp;
	int64 TimeStart_Blend, TimeEnd_Blend;
	int64 TimeStart_Test, TimeEnd_Test;

	double result_Total;
	double result_EstimateHomo, result_CorrectHomo, result_Warp;
	double result_Blend;
	double result_Test;

	// stitch score

	double result_stitchScore;

	// file stream
	FILE* file;
	CString filePos;

	//===================================
	//======= Function ==================
	//===================================
	void Initialize(bool bIsZ1, bool bIsZ2, bool bIsZ3);
	void Destroy(bool bIsL, bool bIsR, bool bIsZ1, bool bIsZ2, bool bIsZ3);
	
	bool GetFrame(PGETFRAME inputFrm, int currentFrm, BYTE** outImg, int& nH, int& nW);

	void GethomographySIFT(Mat& dstImg, int nH_dst, int nW_dst, Mat& srcImg, int nH_src, int nW_src, Mat& homo, int numPt, bool bDebug);
	void GethomographyGoodFeature(vector<Point2f>& arrPtPre, Mat& homo, Mat& homo_pre, int thresh_distance, int minFeature, double ransac, bool bFirst, bool bDebug = false, bool bEnlarge = false, int nEnlargeRate = 1, LR bLR = LEFT);
	void GethomographyNew(vector<Point2f>& arrPtPre, Mat& homo, Mat& homo_pre, int thresh_distance, int minFeature, double ransac, bool bFirst, bool bDebug = false, bool bEnlarge = false, int nEnlargeRate = 1, LR bLR = LEFT);
	void GethomographyHarris(vector<Point2f>& arrPtPre, Mat& homo, Mat& homo_pre, int thresh_distance, int minFeature, double ransac, bool bFirst, bool bDebug = false, bool bEnlarge = false, int nEnlargeRate = 1, LR bLR = LEFT);
	void GethomographyWithoutPreHomo(vector<Point2f>& arrPtPre, Mat& homo, Mat& homo_pre, int thresh_distance, int minFeature, double ransac, bool bFirst, bool bDebug = false, bool bEnlarge = false, int nEnlargeRate = 1, LR bLR = LEFT);

	void CorrectHomography(vector<Point2f>& arrPtR, bool bDebug);
	void CorrectHomographyNew(vector<Point2f>& arrPtR, bool bDebug);
	void CorrectHomographyOpticalFlow(vector<Point2f>& arrPtR, bool bDebug);
	void CorrectHomographyPre(vector<Point2f>& arrPtR, bool bDebug);
	void CorrectHomographyTemplate(vector<Point2f>& arrPtR, bool bDebug);

	void WarpFrame(bool bIsMultiband, double dScaleRate);

	void CheckZoom(bool bIsZoom1, bool bIsZoom2, bool bIsZoom3, int nPadding, bool bDebug);

	void DisplayImage(bool bIsZoom1, bool bIsZoom2, bool bIsZoom3, bool bDebug);

	double MeasureStitchScore();

	void MakeMaskForBlend();
	void WarpHomographyForVideo(Mat& BL, Mat& BR, int nHL, int nWL, int nHR, int nWR, bool bMultiBandBlend, bool bDebug);
	void WarpHomographyForVideoCheckBox(Mat& BL, Mat& BR, int nHL, int nWL, int nHR, int nWR, bool bMultiBandBlend, bool bDebug, int frame);

	void ScaleHomography(Mat& Homo, Mat& scaledHomo, double dScale);

	void MakeROI(Mat& ROI, Mat& scale_zoom, Point2f& posROI, Mat& zoom, int frame);
	void MakeROITemplate(Mat& temROI, Mat& scaleZoom, int posX, int posY, int H, int W, int pading, int& offsetX, int& offsetY);

	bool DetermineConv(Point2f equ1, Point2f equ2, Point2f pt1, Point2f pt2);
	bool DetermineHomo(Mat& homo, Point2f target[], int nH_ROI, int nW_ROI);

	void TemplateMatching(Mat& search, Mat& tmp, Point2f& vec);

	void CalcuHeightWidth(Point2f pt[], int& nH, int& nW, int& LX, int& LY);

	afx_msg void OnBnClickedOptimization();
	CButton m_CheckFile;

	clock_t Tstart, Tend;
	double m_processNoise;
	double m_measureNoise;
	CButton m_Check_StopView;
	CButton m_Check_Enlarge;
	int m_Edit_EnlargeRate;
	CButton m_Check_MultiBandBlend;
	int m_nStitchW;
	int m_nStitchH;
	double m_Stitch_Scale;
	CButton m_Check_File;
	CButton m_Check_StitchScore;
	CButton m_Check_Parallel;
	
	int m_Edit_UL_Y;
	int m_Edit_UL_X;
	int m_Edit_LL_Y;
	int m_Edit_LL_X;
	int m_Edit_UR_Y;
	int m_Edit_UR_X;
	int m_Edit_LR_Y;
	int m_Edit_LR_X;
	double m_Edit_scaleZoom;

	int m_Edit_UL_Y2;
	int m_Edit_UL_X2;
	int m_Edit_LL_Y2;
	int m_Edit_LL_X2;
	int m_Edit_UR_Y2;
	int m_Edit_UR_X2;
	int m_Edit_LR_Y2;
	int m_Edit_LR_X2;
	double m_Edit_scaleZoom2;

	int m_Edit_UL_Y3;
	int m_Edit_UL_X3;
	int m_Edit_LL_Y3;
	int m_Edit_LL_X3;
	int m_Edit_UR_Y3;
	int m_Edit_UR_X3;
	int m_Edit_LR_Y3;
	int m_Edit_LR_X3;
	double m_Edit_scaleZoom3;
	double m_edit_smooth;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPROCESSDLG_H__9D0BB200_9920_4FE1_84D5_8B823B5BAA47__INCLUDED_)
