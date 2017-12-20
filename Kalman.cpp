// Kalman.cpp: implementation of the CKalman class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "KalmanView.h"
#include "Kalman.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKalman::CKalman()
{

}

CKalman::~CKalman()
{

}

void CKalman::Init(CPoint ptInit, double MN, double PN)
{
	m_xHat[0] = ptInit.x;
	m_xHat[1] = ptInit.y;
	m_xHat[2] = 0;
	m_xHat[3] = 0;

	int r, c;

	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
			m_Post[r][c] = 1;

	m_measureNoise = MN;
	m_processNoise = PN;
}

void CKalman::Correct(CPoint ptMeasure)
{
	double Temp44[4][4], Temp24[2][4];
	double G[4][2];
	double A[4][2], B[2][2];
	double H[2][4] = { 1, 0, 0, 0, 0, 1, 0, 0 };
	double HT[4][2] = { 1, 0, 0, 1, 0, 0, 0, 0 };
	double GP[4];
	double HxP[2];

	int r, c, k;

	for (r = 0; r < 4; r++)
		for (c = 0; c < 2; c++)
		{
			A[r][c] = 0;
			for (k = 0; k < 4; k++)
				A[r][c] += m_PostPredict[r][k] * HT[k][c];
		}

	for (r = 0; r < 2; r++)
		for (c = 0; c < 4; c++)
		{
			Temp24[r][c] = 0;
			for (k = 0; k < 4; k++)
				Temp24[r][c] += H[r][k] * m_PostPredict[k][c];
		}

	for (r = 0; r < 2; r++)
		for (c = 0; c < 2; c++)
		{
			B[r][c] = 0;
			for (k = 0; k < 4; k++)
				B[r][c] += Temp24[r][k] * HT[k][c];
		}

	// measureNoise
	for (r = 0; r < 2; r++)
		for (c = 0; c < 2; c++)
			if (c == r) B[r][c] += m_measureNoise;

	double aE, bE, cE, dE, D;
	D = B[0][0] * B[1][1] - B[0][1] * B[1][0];
	aE = B[0][0];
	bE = B[0][1];
	cE = B[1][0];
	dE = B[1][1];

	B[0][0] = dE / D;
	B[0][1] = -bE / D;
	B[1][0] = -cE / D;
	B[1][1] = aE / D;

	for (r = 0; r < 4; r++)
		for (c = 0; c < 2; c++)
		{
			G[r][c] = 0;
			for (k = 0; k < 2; k++)
				G[r][c] += A[r][k] * B[k][c];
		}

	for (r = 0; r < 2; r++)
	{
		HxP[r] = 0;
		for (k = 0; k < 4; k++)
			HxP[r] += H[r][k] * m_xHatPredict[k];
	}

	HxP[0] = ptMeasure.x - HxP[0];
	HxP[1] = ptMeasure.y - HxP[1];

	for (r = 0; r < 4; r++)
	{
		GP[r] = 0;
		for (k = 0; k < 2; k++)
			GP[r] += G[r][k] * HxP[k];
	}

	m_xHat[0] = m_xHatPredict[0] + GP[0];
	m_xHat[1] = m_xHatPredict[1] + GP[1];
	m_xHat[2] = m_xHatPredict[2] + GP[2];
	m_xHat[3] = m_xHatPredict[3] + GP[3];

	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
		{
			Temp44[r][c] = 0;
			for (k = 0; k < 2; k++)
				Temp44[r][c] += G[r][k] * H[k][c];
		}

	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
		{
			if (r == c) Temp44[r][c] = 1 - Temp44[r][c];
			else
				Temp44[r][c] *= -1;
		}

	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
		{
			m_Post[r][c] = 0;
			for (k = 0; k < 4; k++)
				m_Post[r][c] += Temp44[r][k] * m_PostPredict[k][c];
		}

}

CPoint CKalman::Predict()
{
	double Temp44[4][4];
	double F[4][4] = { 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 };
	double FT[4][4] = { 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1 };

	int r, c, k;
	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
		{
			Temp44[r][c] = 0;
			for (k = 0; k < 4; k++)
				Temp44[r][c] += F[r][k] * m_Post[k][c];
		}

	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
		{
			m_PostPredict[r][c] = 0;
			for (k = 0; k < 4; k++)
				m_PostPredict[r][c] += Temp44[r][k] * FT[k][c];
		}


	// Process noise
	for (r = 0; r < 4; r++)
		for (c = 0; c < 4; c++)
			if (c == r) m_PostPredict[r][c] += m_processNoise;

	m_xHatPredict[0] = m_xHat[0] + m_xHat[2];
	m_xHatPredict[1] = m_xHat[1] + m_xHat[3];
	m_xHatPredict[2] = m_xHat[2];
	m_xHatPredict[3] = m_xHat[3];

	return CPoint((int)m_xHatPredict[0], (int)m_xHatPredict[1]);
}
