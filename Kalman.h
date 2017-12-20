// Kalman.h: interface for the CKalman class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KALMAN_H__2EA86616_F2DA_4FAA_B36C_8C54B4DB9C10__INCLUDED_)
#define AFX_KALMAN_H__2EA86616_F2DA_4FAA_B36C_8C54B4DB9C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKalman
{
public:
	CKalman();
	virtual ~CKalman();

	double m_xHatPredict[4];
	double m_xHat[4];
	double m_PostPredict[4][4];
	double m_Post[4][4];

	double m_measureNoise;
	double m_processNoise;

	void Init(CPoint ptInit, double MN, double PN);
	CPoint Predict();
	void Correct(CPoint ptMeasure);

};

#endif // !defined(AFX_KALMAN_H__2EA86616_F2DA_4FAA_B36C_8C54B4DB9C10__INCLUDED_)
