#include "stdafx.h"
#include "Image.h"
#include "Mesh.h"
#include <cmath>
#include "time.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

#include<vector>
using namespace cv;

#include "StitchBasic.h"

CImageStitchBasic::CImageStitchBasic()
{
	// 영상 정보 읽기
	if (!GetCurrentImageInfo(&imgL.nW, &imgL.nH, &imgL.nPosX, &imgL.nPosY)) return;
	if (!GetCurrentImageInfo(&imgR.nW, &imgR.nH, &imgR.nPosX, &imgR.nPosY, 1)) return;

	imgL.imgGray = cmatrix(imgL.nH, imgL.nW);
	imgR.imgGray = cmatrix(imgR.nH, imgR.nW);

	imgL.imgRed = cmatrix(imgL.nH, imgL.nW);
	imgL.imgGreen = cmatrix(imgL.nH, imgL.nW);
	imgL.imgBlue = cmatrix(imgL.nH, imgL.nW);

	imgR.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgR.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgR.imgBlue = cmatrix(imgR.nH, imgR.nW);

	imgMark.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgMark.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgMark.imgBlue = cmatrix(imgR.nH, imgR.nW);

	// 회색조 영상 읽기
	GetCurrentImageGray(imgL.imgGray);
	GetCurrentImageGray(imgR.imgGray, 1);

	// 컬러 영상 읽기
	GetCurrentImageColor(imgL.imgRed, imgL.imgGreen, imgL.imgBlue);
	GetCurrentImageColor(imgR.imgRed, imgR.imgGreen, imgR.imgBlue, 1);
	GetCurrentImageColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, 1);

	//Set weightmap for Seam
	weightSeamR = dmatrix(imgR.nH, imgR.nW);
	weightSeamL = dmatrix(imgL.nH, imgL.nW);

	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);
}

CImageStitchBasic::CImageStitchBasic(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR)
{
	bErr = false;

	int y, x;
	imgL.nH = nHL, imgL.nW = nWL;
	imgR.nH = nHR, imgR.nW = nWR;

	imgL.imgGray = cmatrix(imgL.nH, imgL.nW);
	imgR.imgGray = cmatrix(imgR.nH, imgR.nW);

	imgL.imgRed = cmatrix(imgL.nH, imgL.nW);
	imgL.imgGreen = cmatrix(imgL.nH, imgL.nW);
	imgL.imgBlue = cmatrix(imgL.nH, imgL.nW);

	imgR.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgR.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgR.imgBlue = cmatrix(imgR.nH, imgR.nW);

	imgMark.imgRed = cmatrix(imgR.nH, imgR.nW);
	imgMark.imgGreen = cmatrix(imgR.nH, imgR.nW);
	imgMark.imgBlue = cmatrix(imgR.nH, imgR.nW);

	// Set weightmap for Seam
	weightSeamR = dmatrix(imgR.nH, imgR.nW);
	weightSeamL = dmatrix(imgL.nH, imgL.nW);

	MakeWeightSeam(weightSeamR, imgR.nH, imgR.nW);
	MakeWeightSeam(weightSeamL, imgL.nH, imgL.nW);

	// 컬러영상 읽기
	// - 왼쪽 -
	int Pos;

	for (y = 0; y < nHL; y++)
		for (x = 0; x < nWL; x++)
		{
			Pos = GetBmp24Pos(nWL, nHL, x, y);
			imgL.imgBlue[y][x] = img1D_L[Pos++];
			imgL.imgGreen[y][x] = img1D_L[Pos++];
			imgL.imgRed[y][x] = img1D_L[Pos];
		}
	// - 오른쪽 -
	for (y = 0; y < nHR; y++)
		for (x = 0; x < nWR; x++)
		{
			Pos = GetBmp24Pos(nWR, nHR, x, y);
			imgR.imgBlue[y][x] = img1D_R[Pos++];
			imgR.imgGreen[y][x] = img1D_R[Pos++];
			imgR.imgRed[y][x] = img1D_R[Pos];
		}

	for (y = 0; y < nHR; y++)
		for (x = 0; x < nWR; x++)
		{
			Pos = GetBmp24Pos(nWR, nHR, x, y);
			imgMark.imgBlue[y][x] = img1D_R[Pos++];
			imgMark.imgGreen[y][x] = img1D_R[Pos++];
			imgMark.imgRed[y][x] = img1D_R[Pos];
		}

	// 회색영상 읽기
	// - 왼쪽 -

	for (y = 0; y < nHL; y++)
		for (x = 0; x < nWL; x++)
		{
			Pos = GetBmp24Pos(nWL, nHL, x, y);
			imgL.imgGray[y][x] = (img1D_L[Pos] + img1D_L[Pos + 1] + img1D_L[Pos + 2]) / 3;
		}

	// - 오른쪽 -
	for (y = 0; y < nHR; y++)
		for (x = 0; x < nWR; x++)
		{
			Pos = GetBmp24Pos(nWR, nHR, x, y);
			imgR.imgGray[y][x] = (img1D_R[Pos] + img1D_R[Pos + 1] + img1D_R[Pos + 2]) / 3;
		}
}

CImageStitchBasic::~CImageStitchBasic()
{
	free_cmatrix(imgL.imgGray, imgL.nH, imgL.nW);
	free_cmatrix(imgR.imgGray, imgR.nH, imgR.nW);

	free_cmatrix(imgL.imgRed, imgL.nH, imgL.nW);
	free_cmatrix(imgL.imgGreen, imgL.nH, imgL.nW);
	free_cmatrix(imgL.imgBlue, imgL.nH, imgL.nW);

	free_cmatrix(imgR.imgRed, imgR.nH, imgR.nW);
	free_cmatrix(imgR.imgGreen, imgR.nH, imgR.nW);
	free_cmatrix(imgR.imgBlue, imgR.nH, imgR.nW);

	free_cmatrix(imgMark.imgRed, imgR.nH, imgR.nW);
	free_cmatrix(imgMark.imgGreen, imgR.nH, imgR.nW);
	free_cmatrix(imgMark.imgBlue, imgR.nH, imgR.nW);

	free_dmatrix(weightSeamR, imgR.nH, imgR.nW);
	free_dmatrix(weightSeamL, imgL.nH, imgL.nW);
}

void CImageStitchBasic::GetMatchingPoint(int nPara, bool bOnFeature, bool bOnMatch)
{
	Mat M_ImageL(cvSize(imgL.nW, imgL.nH), CV_8UC1, cvScalar(0));
	Mat M_ImageR(cvSize(imgR.nW, imgR.nH), CV_8UC1, cvScalar(0));

	// Byte -> Mat

	ByteToMatGray(imgL.imgGray, M_ImageL.data, imgL.nH, imgL.nW);
	ByteToMatGray(imgR.imgGray, M_ImageR.data, imgR.nH, imgR.nW);
	
	//Detect the keypoints using SURF Detector
	
	FastFeatureDetector detector(10);
	std::vector< KeyPoint > keypoints_L, keypoints_R;
	detector.detect(M_ImageL, keypoints_L);
	detector.detect(M_ImageR, keypoints_R);

	//-- Step 2: Calculate descriptors (feature vectors)

	SurfDescriptorExtractor extractor;
	Mat descriptors_L, descriptors_R;
	extractor.compute(M_ImageL, keypoints_L, descriptors_L);
	extractor.compute(M_ImageR, keypoints_R, descriptors_R);

	//-- Step 3: Matching descriptor vectors using FLANN matcher

	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_L, descriptors_R, matches);
	double max_dist = 0; double min_dist = 100;


	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_L.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )

	std::vector< DMatch > good_matches;
	for (int i = 0; i < descriptors_L.rows; i++)
		if (matches[i].distance <  4 * min_dist)
			good_matches.push_back(matches[i]);

	//--특징점 갯수 확인
	if (good_matches.size()<10)
	{
		bErr = true;
		return;
		//AfxMessageBox(_T("특징점의 갯수가 부족합니다."));
	}


	for (unsigned int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		GodPointL.push_back(keypoints_L[good_matches[i].queryIdx].pt);
		GodPointR.push_back(keypoints_R[good_matches[i].trainIdx].pt);
	}

	nNumPoint = GodPointR.size();

	// Option

	if (bOnFeature)
	{
		DrawFeature();
	}

	if (bOnMatch)
	{
		DrawMatch();
	}
}

void CImageStitchBasic::GetHomography(Mat& homo)
{
	homo = findHomography(GodPointR, GodPointL, CV_RANSAC);
	
	return ;
}

void CImageStitchBasic::WarpHomography(Mat& homo, bool bOnLR, bool bMultiBandBlend)
{
	int y, x;
	int i;

	//--GetSize
	int offsetX = 0, offsetY = 0;
	int nH_transR, nW_transR;
	int maxX_totalR = 0, maxY_totalR = 0, minX_totalR = imgR.nW, minY_totalR = imgR.nH;
	double maxX_transR = 0, maxY_transR = 0, minX_transR = imgR.nW, minY_transR = imgR.nH;
	int nH_total, nW_total;
	Point2f inputPoint, transPoint;

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			inputPoint = Point2f(x, y);
			GetPerspectivePoint(inputPoint, transPoint, homo);

			if (transPoint.y > maxY_transR) maxY_transR = transPoint.y;
			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
			if (transPoint.y < minY_transR) minY_transR = transPoint.y;
			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
		}

	if (minY_transR < 0) minY_totalR = minY_transR; else minY_totalR = 0;
	if (maxY_transR > imgR.nH - 1) maxY_totalR = maxY_transR; else maxY_totalR = imgR.nH - 1;

	if (minX_transR < 0) minX_totalR = minX_transR; else minX_totalR = 0;
	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;

	nH_total = maxY_totalR - minY_totalR + 1;
	nW_total = maxX_totalR - minX_totalR + 1;

	if (nH_total > 10000 || nW_total > 10000)
	{
		bErr = true;
		return;
	}

	if (minY_transR < 0) offsetY = abs(minY_transR);
	if (minX_transR < 0) offsetX = abs(minX_transR);

	if (bMultiBandBlend)
	{
		imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
		imgLtrans.imgRed = cmatrix(nH_total, nW_total); imgLtrans.imgGreen = cmatrix(nH_total, nW_total); imgLtrans.imgBlue = cmatrix(nH_total, nW_total);
		imgRtrans.imgRed = cmatrix(nH_total, nW_total); imgRtrans.imgGreen = cmatrix(nH_total, nW_total); imgRtrans.imgBlue = cmatrix(nH_total, nW_total);
		weightRtrans = dmatrix(nH_total, nW_total); weightLtrans = dmatrix(nH_total, nW_total);

		imgTotal.nH = nH_total;	imgTotal.nW = nW_total;
		imgLtrans.nH = nH_total; imgLtrans.nW = nW_total;
		imgRtrans.nH = nH_total; imgRtrans.nW = nW_total;


		//---image warping

		// 초기화
		for (y = 0; y < nH_total; y++)
			for (x = 0; x < nW_total; x++)
			{
				weightRtrans[y][x] = 0;	weightLtrans[y][x] = 0;
				imgLtrans.imgRed[y][x] = 0; imgLtrans.imgGreen[y][x] = 0; imgLtrans.imgBlue[y][x] = 0;
				imgRtrans.imgRed[y][x] = 0; imgRtrans.imgGreen[y][x] = 0; imgRtrans.imgBlue[y][x] = 0;
			}

		// 왼쪽
		for (y = 0; y < imgL.nH; y++)
			for (x = 0; x < imgL.nW; x++)
			{
				imgLtrans.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
				imgLtrans.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
				imgLtrans.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
				weightLtrans[y + offsetY][x + offsetX] = weightSeamL[y][x];
			}

		// 오른쪽
		Mat inverH = homo.inv();

		for (y = minY_transR; y < maxY_transR; y++)
			for (x = minX_transR; x < maxX_transR; x++) {

				inputPoint = Point2f(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverH);
				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{
					imgRtrans.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
					imgRtrans.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
					imgRtrans.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
					weightRtrans[y + offsetY][x + offsetX] = weightSeamR[int(transPoint.y)][int(transPoint.x)];
				}
			}

		// Seam 만들기
		for (y = 0; y < nH_total; y++)
			for (x = 0; x < nW_total; x++)
			{
				if (weightRtrans[y][x] > weightLtrans[y][x])
				{
					weightRtrans[y][x] = 1;
					weightLtrans[y][x] = 0;
				}

				if (weightLtrans[y][x] > weightRtrans[y][x])
				{
					weightRtrans[y][x] = 0;
					weightLtrans[y][x] = 1;
				}
			}

		// multiband blending
		this->MultiBandBlending(imgTotal.nH, imgTotal.nW, imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue);

		DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0, false, true);

		// Option
		if (bOnLR)
		{
			DrawLR();
		}

		free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);

		free_dmatrix(weightRtrans, imgTotal.nH, imgTotal.nW);
		free_dmatrix(weightLtrans, imgTotal.nH, imgTotal.nW);

		free_cmatrix(imgRtrans.imgRed, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgRtrans.imgGreen, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgRtrans.imgBlue, imgTotal.nH, imgTotal.nW);

		free_cmatrix(imgLtrans.imgRed, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgLtrans.imgGreen, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgLtrans.imgBlue, imgTotal.nH, imgTotal.nW);
	}

	if (!bMultiBandBlend)
	{
		imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
		imgTotal.nH = nH_total;	imgTotal.nW = nW_total;

		//---image warping

		// 초기화
		for (y = 0; y < nH_total; y++)
			for (x = 0; x < nW_total; x++)
			{
				imgTotal.imgRed[y][x] = 0; imgTotal.imgGreen[y][x] = 0; imgTotal.imgBlue[y][x] = 0;
			}

		// 왼쪽
		for (y = 0; y < imgL.nH; y++)
			for (x = 0; x < imgL.nW; x++)
			{
				imgTotal.imgRed[y + offsetY][x + offsetX] = imgL.imgRed[y][x];
				imgTotal.imgGreen[y + offsetY][x + offsetX] = imgL.imgGreen[y][x];
				imgTotal.imgBlue[y + offsetY][x + offsetX] = imgL.imgBlue[y][x];
			}

		// 오른쪽
		Mat inverH = homo.inv();

		for (y = minY_transR; y < maxY_transR; y++)
			for (x = minX_transR; x < maxX_transR; x++) {

				inputPoint = Point2f(x, y);
				GetPerspectivePoint(inputPoint, transPoint, inverH);
				if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
				{
					imgTotal.imgRed[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
					imgTotal.imgGreen[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
					imgTotal.imgBlue[y + offsetY][x + offsetX] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
				}
			}

		DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0, false, true);

		// Option
		if (bOnLR)
		{
			DrawLR();
		}

		free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
		free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);
	}
}

void CImageStitchBasic::WarpHomographyForVideo(Mat& homo)
{
	int y, x;
	int i;

	//--GetSize
	int nH_total, nW_total;

	nH_total = imgR.nH;

	int offsetX = 0, offsetY = 0;
	int nH_transR, nW_transR;
	int maxX_totalR = 0, minX_totalR = imgR.nW;
	double maxX_transR = 0, minX_transR = imgR.nW;

	Point2f inputPoint, transPoint;

	for (y = 0; y < imgR.nH; y++)
		for (x = 0; x < imgR.nW; x++)
		{
			inputPoint = Point2f(x, y);
			GetPerspectivePoint(inputPoint, transPoint, homo);

			if (transPoint.x > maxX_transR) maxX_transR = transPoint.x;
			if (transPoint.x < minX_transR) minX_transR = transPoint.x;
		}

	if (minX_transR < 0)
	{
		bErr = true;
		return;
	}

	if (maxX_transR > imgR.nW - 1) maxX_totalR = maxX_transR; else maxX_totalR = imgR.nW - 1;

	nW_total = maxX_totalR;

	if (nH_total > 10000 || nW_total > 10000)
	{
		bErr = true;
		return;
	}

	imgTotal.imgRed = cmatrix(nH_total, nW_total); imgTotal.imgGreen = cmatrix(nH_total, nW_total); imgTotal.imgBlue = cmatrix(nH_total, nW_total);
	imgTotal.nH = nH_total;	imgTotal.nW = nW_total;

	//---image warping

	// 초기화
	for (y = 0; y < nH_total; y++)
		for (x = 0; x < nW_total; x++)
		{
			imgTotal.imgRed[y][x] = 0; imgTotal.imgGreen[y][x] = 0; imgTotal.imgBlue[y][x] = 0;
		}

	// 왼쪽
	for (y = 0; y < imgL.nH; y++)
		for (x = 0; x < imgL.nW; x++)
		{
			imgTotal.imgRed[y][x] = imgL.imgRed[y][x];
			imgTotal.imgGreen[y][x] = imgL.imgGreen[y][x];
			imgTotal.imgBlue[y][x] = imgL.imgBlue[y][x];
		}

	// 오른쪽
	Mat inverH = homo.inv();

	for (y = 0; y < nH_total; y++)
		for (x = 0; x < nW_total; x++) {

			inputPoint = Point2f(x, y);
			GetPerspectivePoint(inputPoint, transPoint, inverH);
			if (transPoint.x >= 0 && transPoint.x < imgR.nW && transPoint.y >= 0 && transPoint.y < imgR.nH)
			{
				imgTotal.imgRed[y][x] = BilinearInterpolation(transPoint, imgR.imgRed, imgR.nH, imgR.nW);
				imgTotal.imgGreen[y][x] = BilinearInterpolation(transPoint, imgR.imgGreen, imgR.nH, imgR.nW);;
				imgTotal.imgBlue[y][x] = BilinearInterpolation(transPoint, imgR.imgBlue, imgR.nH, imgR.nW);;
			}
		}

	DisplayCimage2DColor(imgTotal.imgRed, imgTotal.imgGreen, imgTotal.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0, false, true);

	free_cmatrix(imgTotal.imgRed, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgTotal.imgGreen, imgTotal.nH, imgTotal.nW);
	free_cmatrix(imgTotal.imgBlue, imgTotal.nH, imgTotal.nW);
}

void CImageStitchBasic::DrawFeature(int nPosX, int nPosY, int nRate)
{
	int nRand;
	int arrColor[3];

	// 색깔 랜덤으로 받기
	nRand = rand() % 255;	arrColor[0] = nRand;
	nRand = rand() % 255;   arrColor[1] = nRand;
	nRand = rand() % 255;   arrColor[2] = nRand;

	// 특징점 표시
	for (int i = 0; i < nNumPoint; i++)
	{
		DrawCross(imgMark.imgRed, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[0]);
		DrawCross(imgMark.imgGreen, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[1]);
		DrawCross(imgMark.imgBlue, imgR.nW, imgR.nH, CPoint(GodPointR[i].x, GodPointR[i].y), arrColor[2]);
	}

	DisplayCimage2DColor(imgMark.imgRed, imgMark.imgGreen, imgMark.imgBlue, imgR.nW, imgR.nH, nPosX, nPosY);
}

void CImageStitchBasic::DrawMatch(int nPosX, int nPosY, int nRate)
{
	Image2D imgMatch;

	imgMatch.nH = imgR.nH;
	imgMatch.nW = imgR.nW + imgL.nW;

	imgMatch.imgRed = cmatrix(imgMatch.nH, imgMatch.nW);
	imgMatch.imgGreen = cmatrix(imgMatch.nH, imgMatch.nW);
	imgMatch.imgBlue = cmatrix(imgMatch.nH, imgMatch.nW);

	for (int i = 0; i < imgL.nH; i++)
		for (int j = 0; j < imgL.nW; j++)
		{
			imgMatch.imgRed[i][j] = imgL.imgRed[i][j];
			imgMatch.imgGreen[i][j] = imgL.imgGreen[i][j];
			imgMatch.imgBlue[i][j] = imgL.imgBlue[i][j];
		}

	for (int i = 0; i < imgR.nH; i++)
		for (int j = 0; j < imgR.nW; j++)
		{
			imgMatch.imgRed[i][j + imgL.nW] = imgR.imgRed[i][j];
			imgMatch.imgGreen[i][j + imgL.nW] = imgR.imgGreen[i][j];
			imgMatch.imgBlue[i][j + imgL.nW] = imgR.imgBlue[i][j];
		}

	for (int i = 0; i < GodPointR.size(); i++)
	{
		DrawLine(imgMatch.imgRed, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
		DrawLine(imgMatch.imgGreen, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 255);
		DrawLine(imgMatch.imgBlue, imgR.nW + imgL.nW, imgR.nH, GodPointL[i].x, GodPointL[i].y, GodPointR[i].x + imgL.nW, GodPointR[i].y, 0);
	}

	DisplayCimage2DColor(imgMatch.imgRed, imgMatch.imgGreen, imgMatch.imgBlue, imgR.nW + imgL.nW, imgL.nH, nPosX, nPosY);

	free_cmatrix(imgMatch.imgRed, imgMatch.nH, imgMatch.nW);
	free_cmatrix(imgMatch.imgGreen, imgMatch.nH, imgMatch.nW);
	free_cmatrix(imgMatch.imgBlue, imgMatch.nH, imgMatch.nW);
}

void CImageStitchBasic::DrawLR(int nPosX, int nPosY, int nRate)
{
	DisplayCimage2DColor(imgLtrans.imgRed, imgLtrans.imgGreen, imgLtrans.imgBlue, imgTotal.nW, imgTotal.nH, nPosX, nPosY, false, true, nRate);
	DisplayCimage2DColor(imgRtrans.imgRed, imgRtrans.imgGreen, imgRtrans.imgBlue, imgTotal.nW, imgTotal.nH, 0, 0, false, true, nRate);
}

void CImageStitchBasic::MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B)
{
	int x, y;
	int nGSize = 3; // Gaussian Filter Size
	double dSigma = 20; // Gaussian's sigma
	double dSigma1, dSigma2;
	double** Gfilter = dmatrix(nGSize, nGSize);

	int nRate;
	int nH_Rate, nW_Rate;

	if (nW > 300)
		nRate = nW / 300;
	else
		nRate = 1;

	nH_Rate = nH / nRate;
	nW_Rate = nW / nRate;

	//down sampling 웨이트 값
	BYTE** dwnR_weight, ** dwnL_weight;

	// 가우시안 필터링한 이미지
	double** GL_Ch1_R, **GL_Ch1_G, **GL_Ch1_B; //왼쪽 영상
	double** GR_Ch1_R, **GR_Ch1_G, **GR_Ch1_B; //오른쪽 영상

	double** GL_Ch2_R, **GL_Ch2_G, **GL_Ch2_B;
	double** GR_Ch2_R, **GR_Ch2_G, **GR_Ch2_B;

	double** GL_Ch3_R, **GL_Ch3_G, **GL_Ch3_B;
	double** GR_Ch3_R, **GR_Ch3_G, **GR_Ch3_B;

	// 원본이미지와 가우시안한 이미지의 차이(즉 이미지의 주파수 대역)
	double** BL_Ch1_R, **BL_Ch1_G, **BL_Ch1_B; //왼쪽 영상
	double** BR_Ch1_R, **BR_Ch1_G, **BR_Ch1_B; //오른쪽 영상

	double** BL_Ch2_R, **BL_Ch2_G, **BL_Ch2_B;
	double** BR_Ch2_R, **BR_Ch2_G, **BR_Ch2_B;

	double** BL_Ch3_R, **BL_Ch3_G, **BL_Ch3_B;
	double** BR_Ch3_R, **BR_Ch3_G, **BR_Ch3_B;

	// 가우시안 필터링한 웨이트 값
	double** WL_Ch1, **WL_Ch2, **WL_Ch3; //왼쪽 영상
	double** WR_Ch1, **WR_Ch2, **WR_Ch3; //오른쪽 영상

	double** iWL_Ch1, **iWL_Ch2, ** iWL_Ch3;
	double** iWR_Ch1, **iWR_Ch2, ** iWR_Ch3;

	double** Out_Ch1_R, **Out_Ch1_G, **Out_Ch1_B;
	double** Out_Ch2_R, **Out_Ch2_G, **Out_Ch2_B;
	double** Out_Ch3_R, **Out_Ch3_G, **Out_Ch3_B;


	//메모리 할당
	dwnR_weight = cmatrix(nH_Rate, nW_Rate); dwnL_weight = cmatrix(nH_Rate, nW_Rate);

	WL_Ch1 = dmatrix(nH_Rate, nW_Rate); WL_Ch2 = dmatrix(nH_Rate, nW_Rate); WL_Ch3 = dmatrix(nH_Rate, nW_Rate);
	WR_Ch1 = dmatrix(nH_Rate, nW_Rate); WR_Ch2 = dmatrix(nH_Rate, nW_Rate); WR_Ch3 = dmatrix(nH_Rate, nW_Rate);

	iWL_Ch1 = dmatrix(nH, nW); iWL_Ch2 = dmatrix(nH, nW); iWL_Ch3 = dmatrix(nH, nW);
	iWR_Ch1 = dmatrix(nH, nW); iWR_Ch2 = dmatrix(nH, nW); iWR_Ch3 = dmatrix(nH, nW);

	GL_Ch1_R = dmatrix(nH, nW);	GL_Ch1_G = dmatrix(nH, nW);	GL_Ch1_B = dmatrix(nH, nW);
	GR_Ch1_R = dmatrix(nH, nW);	GR_Ch1_G = dmatrix(nH, nW);	GR_Ch1_B = dmatrix(nH, nW);

	GL_Ch2_R = dmatrix(nH, nW);	GL_Ch2_G = dmatrix(nH, nW);	GL_Ch2_B = dmatrix(nH, nW);
	GR_Ch2_R = dmatrix(nH, nW);	GR_Ch2_G = dmatrix(nH, nW);	GR_Ch2_B = dmatrix(nH, nW);

	GL_Ch3_R = dmatrix(nH, nW);	GL_Ch3_G = dmatrix(nH, nW);	GL_Ch3_B = dmatrix(nH, nW);
	GR_Ch3_R = dmatrix(nH, nW);	GR_Ch3_G = dmatrix(nH, nW);	GR_Ch3_B = dmatrix(nH, nW);

	BL_Ch1_R = dmatrix(nH, nW); BL_Ch1_G = dmatrix(nH, nW);	BL_Ch1_B = dmatrix(nH, nW);
	BR_Ch1_R = dmatrix(nH, nW);	BR_Ch1_G = dmatrix(nH, nW);	BR_Ch1_B = dmatrix(nH, nW);

	BL_Ch2_R = dmatrix(nH, nW);	BL_Ch2_G = dmatrix(nH, nW);	BL_Ch2_B = dmatrix(nH, nW);
	BR_Ch2_R = dmatrix(nH, nW);	BR_Ch2_G = dmatrix(nH, nW);	BR_Ch2_B = dmatrix(nH, nW);

	BL_Ch3_R = dmatrix(nH, nW);	BL_Ch3_G = dmatrix(nH, nW);	BL_Ch3_B = dmatrix(nH, nW);
	BR_Ch3_R = dmatrix(nH, nW);	BR_Ch3_G = dmatrix(nH, nW);	BR_Ch3_B = dmatrix(nH, nW);

	Out_Ch1_R = dmatrix(nH, nW); Out_Ch1_G = dmatrix(nH, nW); Out_Ch1_B = dmatrix(nH, nW);
	Out_Ch2_R = dmatrix(nH, nW); Out_Ch2_G = dmatrix(nH, nW); Out_Ch2_B = dmatrix(nH, nW);
	Out_Ch3_R = dmatrix(nH, nW); Out_Ch3_G = dmatrix(nH, nW); Out_Ch3_B = dmatrix(nH, nW);

	//-----------------------------------------------------------------------------------------
	// down sampling weightmap
	for (int y = 0; y < nH_Rate; y++)
	{
		for (int x = 0; x < nW_Rate; x++)
		{
			dwnL_weight[y][x] = weightLtrans[y*nRate][x*nRate];
			dwnR_weight[y][x] = weightRtrans[y*nRate][x*nRate];
		}
	}

	//Ch1(high frequency) 이미지 필터링
	GaussianFilter(Gfilter, dSigma, nGSize);

	//이미지 가우시안 필터링
	MirrorPadding(GL_Ch1_R, Gfilter, imgLtrans.imgRed, nGSize, nW, nH); MirrorPadding(GL_Ch1_G, Gfilter, imgLtrans.imgGreen, nGSize, nW, nH); MirrorPadding(GL_Ch1_B, Gfilter, imgLtrans.imgBlue, nGSize, nW, nH);
	MirrorPadding(GR_Ch1_R, Gfilter, imgRtrans.imgRed, nGSize, nW, nH); MirrorPadding(GR_Ch1_G, Gfilter, imgRtrans.imgGreen, nGSize, nW, nH); MirrorPadding(GR_Ch1_B, Gfilter, imgRtrans.imgBlue, nGSize, nW, nH);

	//BilinearInterpolation(W1_Ch1, iW1_Ch1, nW / srate, nH / srate, srate);
	//BilinearInterpolation(W2_Ch1, iW2_Ch1, nW / srate, nH / srate, srate);

	//웨이트 가우시안 필터링
	MirrorPadding(WL_Ch1, Gfilter, dwnL_weight, nGSize, nW_Rate, nH_Rate); MirrorPadding(WR_Ch1, Gfilter, dwnR_weight, nGSize, nW_Rate, nH_Rate);

	double val;
	for (int y = 0; y<nH; y++)
		for (int x = 0; x<nW; x++)
		{
			BL_Ch1_R[y][x] = imgLtrans.imgRed[y][x] - GL_Ch1_R[y][x];
			BL_Ch1_G[y][x] = imgLtrans.imgGreen[y][x] - GL_Ch1_G[y][x];
			BL_Ch1_B[y][x] = imgLtrans.imgBlue[y][x] - GL_Ch1_B[y][x];

			BR_Ch1_R[y][x] = imgRtrans.imgRed[y][x] - GR_Ch1_R[y][x];
			BR_Ch1_G[y][x] = imgRtrans.imgGreen[y][x] - GR_Ch1_G[y][x];
			BR_Ch1_B[y][x] = imgRtrans.imgBlue[y][x] - GR_Ch1_B[y][x];

			val = (BL_Ch1_R[y][x] * WL_Ch1[y][x] + BR_Ch1_R[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
			Out_Ch1_R[y][x] = val;

			val = (BL_Ch1_G[y][x] * WL_Ch1[y][x] + BR_Ch1_G[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
			Out_Ch1_G[y][x] = val;

			val = (BL_Ch1_B[y][x] * WL_Ch1[y][x] + BR_Ch1_B[y][x] * WR_Ch1[y][x]) / (WL_Ch1[y][x] + WR_Ch1[y][x]);
			Out_Ch1_B[y][x] = val;
		}

	//Ch2(middle frequency) 이미지 필터링
	dSigma1 = sqrt(3.)*dSigma;
	GaussianFilter(Gfilter, dSigma1, nGSize);

	MirrorPadding(GL_Ch2_R, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_G, Gfilter, GL_Ch1_R, nGSize, nW, nH); MirrorPadding(GL_Ch2_B, Gfilter, GL_Ch1_R, nGSize, nW, nH);
	MirrorPadding(GR_Ch2_R, Gfilter, GR_Ch1_R, nGSize, nW, nH); MirrorPadding(GR_Ch2_G, Gfilter, GR_Ch1_G, nGSize, nW, nH); MirrorPadding(GR_Ch2_B, Gfilter, GR_Ch1_B, nGSize, nW, nH);

	MirrorPadding(WL_Ch2, Gfilter, WL_Ch1, nGSize, nW, nH); MirrorPadding(WR_Ch2, Gfilter, WR_Ch1, nGSize, nW, nH);

	for (y = 0; y<nH; y++)
		for (x = 0; x<nW; x++)
		{
			BL_Ch2_R[y][x] = GL_Ch1_R[y][x] - GL_Ch2_R[y][x];
			BL_Ch2_G[y][x] = GL_Ch1_G[y][x] - GL_Ch2_G[y][x];
			BL_Ch2_B[y][x] = GL_Ch1_B[y][x] - GL_Ch2_B[y][x];

			BR_Ch2_R[y][x] = GR_Ch1_R[y][x] - GR_Ch2_R[y][x];
			BR_Ch2_G[y][x] = GR_Ch1_G[y][x] - GR_Ch2_G[y][x];
			BR_Ch2_B[y][x] = GR_Ch1_B[y][x] - GR_Ch2_B[y][x];

			val = (BL_Ch2_R[y][x] * WL_Ch2[y][x] + BR_Ch2_R[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
			Out_Ch2_R[y][x] = val;

			val = (BL_Ch2_G[y][x] * WL_Ch2[y][x] + BR_Ch2_G[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
			Out_Ch2_G[y][x] = val;

			val = (BL_Ch2_B[y][x] * WL_Ch2[y][x] + BR_Ch2_B[y][x] * WR_Ch2[y][x]) / (WL_Ch2[y][x] + WR_Ch2[y][x]);
			Out_Ch2_B[y][x] = val;
		}

	//Ch3(low frequency) 이미지 필터링
	dSigma2 = sqrt(5.)*dSigma;
	GaussianFilter(Gfilter, dSigma2, nGSize);

	MirrorPadding(GL_Ch3_R, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_G, Gfilter, GL_Ch2_R, nGSize, nW, nH); MirrorPadding(GL_Ch3_B, Gfilter, GL_Ch2_R, nGSize, nW, nH);
	MirrorPadding(GR_Ch3_R, Gfilter, GR_Ch2_R, nGSize, nW, nH); MirrorPadding(GR_Ch3_G, Gfilter, GR_Ch2_G, nGSize, nW, nH); MirrorPadding(GR_Ch3_B, Gfilter, GR_Ch2_B, nGSize, nW, nH);

	MirrorPadding(WL_Ch3, Gfilter, WL_Ch2, nGSize, nW, nH); MirrorPadding(WR_Ch3, Gfilter, WR_Ch2, nGSize, nW, nH);

	for (y = 0; y<nH; y++)
		for (x = 0; x<nW; x++)
		{
			BL_Ch3_R[y][x] = GL_Ch2_R[y][x];// -GL_Ch3_R[y][x];
			BL_Ch3_G[y][x] = GL_Ch2_G[y][x];// -GL_Ch3_G[y][x];
			BL_Ch3_B[y][x] = GL_Ch2_B[y][x];// -GL_Ch3_B[y][x];

			BR_Ch3_R[y][x] = GR_Ch2_R[y][x];// -GR_Ch3_R[y][x];
			BR_Ch3_G[y][x] = GR_Ch2_G[y][x];// -GR_Ch3_G[y][x];
			BR_Ch3_B[y][x] = GR_Ch2_B[y][x];// -GR_Ch3_B[y][x];

			val = (BL_Ch3_R[y][x] * WL_Ch3[y][x] + BR_Ch3_R[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
			Out_Ch3_R[y][x] = val;

			val = (BL_Ch3_G[y][x] * WL_Ch3[y][x] + BR_Ch3_G[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
			Out_Ch3_G[y][x] = val;

			val = (BL_Ch3_B[y][x] * WL_Ch3[y][x] + BR_Ch3_B[y][x] * WR_Ch3[y][x]) / (WL_Ch3[y][x] + WR_Ch3[y][x]);
			Out_Ch3_B[y][x] = val;
		}

	for (y = 0; y < nH; y++)
		for (x = 0; x < nW; x++)
		{
			if (WR_Ch3[y][x] == 0)
			{
				WR_Ch3[y][x] = 1;
			}
			else
				WR_Ch3[y][x] = 0;
		}

	// 블렌딩 합치기
	for (int y = 0; y<nH; y++)
		for (int x = 0; x<nW; x++)
		{
			val = Out_Ch1_R[y][x] + Out_Ch2_R[y][x] + Out_Ch3_R[y][x];
			val = val < 0 ? 0 : val;
			val = val > 255 ? 255 : val;
			Result_R[y][x] = val;

			val = Out_Ch1_G[y][x] + Out_Ch2_G[y][x] + Out_Ch3_G[y][x];
			val = val < 0 ? 0 : val;
			val = val > 255 ? 255 : val;
			Result_G[y][x] = val;

			val = Out_Ch1_B[y][x] + Out_Ch2_B[y][x] + Out_Ch3_B[y][x];
			val = val < 0 ? 0 : val;
			val = val > 255 ? 255 : val;
			Result_B[y][x] = val;
		}


	//메모리 해제
	free_dmatrix(Gfilter, nGSize, nGSize);

	free_dmatrix(WL_Ch1, nH, nW); free_dmatrix(WL_Ch2, nH, nW); free_dmatrix(WL_Ch3, nH, nW);
	free_dmatrix(WR_Ch1, nH, nW); free_dmatrix(WR_Ch2, nH, nW); free_dmatrix(WR_Ch3, nH, nW);

	free_dmatrix(iWL_Ch1, nH, nW); free_dmatrix(iWL_Ch2, nH, nW); free_dmatrix(iWL_Ch3, nH, nW);
	free_dmatrix(iWR_Ch1, nH, nW); free_dmatrix(iWR_Ch2, nH, nW); free_dmatrix(iWR_Ch3, nH, nW);

	free_dmatrix(GL_Ch1_R, nH, nW); free_dmatrix(GL_Ch1_G, nH, nW); free_dmatrix(GL_Ch1_B, nH, nW);
	free_dmatrix(GR_Ch1_R, nH, nW); free_dmatrix(GR_Ch1_G, nH, nW); free_dmatrix(GR_Ch1_B, nH, nW);

	free_dmatrix(GL_Ch2_R, nH, nW); free_dmatrix(GL_Ch2_G, nH, nW); free_dmatrix(GL_Ch2_B, nH, nW);
	free_dmatrix(GR_Ch2_R, nH, nW); free_dmatrix(GR_Ch2_G, nH, nW); free_dmatrix(GR_Ch2_B, nH, nW);

	free_dmatrix(GL_Ch3_R, nH, nW); free_dmatrix(GL_Ch3_G, nH, nW); free_dmatrix(GL_Ch3_B, nH, nW);
	free_dmatrix(GR_Ch3_R, nH, nW); free_dmatrix(GR_Ch3_G, nH, nW); free_dmatrix(GR_Ch3_B, nH, nW);

	free_dmatrix(BL_Ch1_R, nH, nW); free_dmatrix(BL_Ch1_G, nH, nW); free_dmatrix(BL_Ch1_B, nH, nW);
	free_dmatrix(BR_Ch1_R, nH, nW); free_dmatrix(BR_Ch1_G, nH, nW); free_dmatrix(BR_Ch1_B, nH, nW);

	free_dmatrix(BL_Ch2_R, nH, nW); free_dmatrix(BL_Ch2_G, nH, nW); free_dmatrix(BL_Ch2_B, nH, nW);
	free_dmatrix(BR_Ch2_R, nH, nW); free_dmatrix(BR_Ch2_G, nH, nW); free_dmatrix(BR_Ch2_B, nH, nW);

	free_dmatrix(BL_Ch3_R, nH, nW); free_dmatrix(BL_Ch3_G, nH, nW); free_dmatrix(BL_Ch3_B, nH, nW);
	free_dmatrix(BR_Ch3_R, nH, nW); free_dmatrix(BR_Ch3_G, nH, nW); free_dmatrix(BR_Ch3_B, nH, nW);

	free_dmatrix(Out_Ch1_R, nH, nW); free_dmatrix(Out_Ch1_G, nH, nW); free_dmatrix(Out_Ch1_B, nH, nW);
	free_dmatrix(Out_Ch2_R, nH, nW); free_dmatrix(Out_Ch2_G, nH, nW); free_dmatrix(Out_Ch2_B, nH, nW);
	free_dmatrix(Out_Ch3_R, nH, nW); free_dmatrix(Out_Ch3_G, nH, nW); free_dmatrix(Out_Ch3_B, nH, nW);
}

void CImageStitchBasic::MakeWeightSeam(double** weightSeam, int nH, int nW)
{
	int i, j;
	double dCenH, dCenW;
	double dTmp;

	dCenH = double(nH) / 2.;
	dCenW = double(nW) / 2.;

	for (i = 0; i < nH; i++)
		for (j = 0; j < nW; j++)
		{
			dTmp = 1. - (1. / dCenH)*(abs(dCenH - double(i)));
			dTmp = dTmp - (dTmp / dCenW)*(abs(dCenW - double(j)));

			if (dTmp > 0)
			{
				dTmp = dTmp;
			}

			weightSeam[i][j] = dTmp;
		}
}

int CImageStitchBasic::BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW)
{
	int result;
	double dX1, dX2;
	double dY1, dY2;
	double IA, IB, IC, ID;
	double Itmp1, Itmp2;
	CvPoint A;

	A.x = int(point.x);
	A.y = int(point.y);

	if (A.x >= nW - 1 || A.y >= nH - 1)
	{
		result = image[int(point.y)][int(point.x)];
		return result;
	}


	IA = image[A.y][A.x];		IB = image[A.y][A.x + 1];
	IC = image[A.y + 1][A.x];	ID = image[A.y + 1][A.x + 1];

	dX1 = point.x - A.x;		dX2 = A.x + 1 - point.x;
	dY1 = point.y - A.y;		dY2 = A.y + 1 - point.y;

	// I'1,2 값 구하기
	Itmp1 = ((dX1 * IB) + (dX2 * IA));
	Itmp2 = ((dX1 * ID) + (dX2 * IC));

	// result 값 구하기
	result = int(((dY1 * Itmp2) + (dY2 * Itmp1)));

	return result;
}