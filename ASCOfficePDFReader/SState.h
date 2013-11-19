#ifndef _SSTATE_H
#define _SSTATE_H

#include "STypes.h"
#include "SScreen.h"

class SPattern;
class SScreen;
class SClip;
class SBitmap;

//-------------------------------------------------------------------------------------------------------------------------------
// Line cap
//-------------------------------------------------------------------------------------------------------------------------------

#define LineCapButt       0
#define LineCapRound      1
#define LineCapProjecting 2

//-------------------------------------------------------------------------------------------------------------------------------
// Line join
//-------------------------------------------------------------------------------------------------------------------------------

#define LineJoinMiter     0
#define LineJoinRound     1
#define LineJoinBevel     2

//-------------------------------------------------------------------------------------------------------------------------------
// SState
//-------------------------------------------------------------------------------------------------------------------------------

class SState 
{
public:

	SState(int nWidth, int nHeight, BOOL bVectorAntialias, SScreenParams *pScreenParams);
	SState(int nWidth, int nHeight, BOOL bVectorAntialias, SScreen *pScreen);

	SState *Copy() 
	{ 
		return new SState(this); 
	}

	~SState();

	// ”станавливаем StrokePattern, не копиру€ pStrokePattern.
	void SetStrokePattern(SPattern *pStrokePattern);

	// ”станавливаем SPattern, не копиру€ pFillPattern.
	void SetFillPattern(SPattern *pFillPattern);

	// ”станавливаем SScreen, не копиру€ pScreen.
	void SetScreen(SScreen *pScreen);

	// ”станавливаем Line dash pattern, копиру€ массив pdLineDash.
	void SetLineDash(double *pLineDash, int nLineDashCount, double dLineDashPhase);

	// ”станавливаем SoftMask.
	void SetSoftMask(SBitmap *pSoftMask);

private:

	SState(SState *pState);

private:

	double      m_arrdMatrix[6];
	SPattern   *m_pStrokePattern;
	SPattern   *m_pFillPattern;
	SScreen    *m_pScreen;
	SBlendFunc  m_pBlendFunction;
	double      m_dStrokeAlpha;
	double      m_dFillAlpha;
	double      m_dLineWidth;
	int         m_nLineCap;
	int         m_nLineJoin;
	double      m_dMiterLimit;
	double      m_dFlatness;
	double     *m_pdLineDash;
	int         m_nLineDashCount;
	double      m_dLineDashPhase;
	BOOL        m_bStrokeAdjust;
	SClip      *m_pClip;
	SBitmap    *m_pSoftMask;
	BOOL        m_bDeleteSoftMask;
	BOOL        m_bInNonIsolatedGroup;

	SState     *m_pNext;

	friend class SImage;
};

#endif /* _SSTATE_H */
