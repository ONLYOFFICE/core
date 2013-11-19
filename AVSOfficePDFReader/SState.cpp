#include "stdafx.h"

#include <string.h>
#include "MemoryUtils.h"
#include "SPattern.h"
#include "SScreen.h"
#include "SClip.h"
#include "SBitmap.h"
#include "SState.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SState
//-------------------------------------------------------------------------------------------------------------------------------
SState::SState(int nWidth, int nHeight, BOOL bVectorAntialias, SScreenParams *pScreenParams) 
{
	m_arrdMatrix[0] = 1;  m_arrdMatrix[1] = 0;
	m_arrdMatrix[2] = 0;  m_arrdMatrix[3] = 1;
	m_arrdMatrix[4] = 0;  m_arrdMatrix[5] = 0;

	SColor pColor;
	memset( &pColor, 0, sizeof(SColor) );
	m_pStrokePattern = new SSolidColor( pColor );
	m_pFillPattern   = new SSolidColor( pColor );

	m_pScreen = new SScreen( pScreenParams );

	m_pBlendFunction = NULL;
	m_dStrokeAlpha   = 1;
	m_dFillAlpha     = 1;
	m_dLineWidth     = 0;
	m_nLineCap       = LineCapButt;
	m_nLineJoin      = LineJoinMiter;
	m_dMiterLimit    = 10;
	m_dFlatness      = 1;
	m_pdLineDash     = NULL;
	m_nLineDashCount = 0;
	m_dLineDashPhase = 0;
	m_bStrokeAdjust  = FALSE;

	m_pClip = new SClip( 0, 0, nWidth - 0.001, nHeight - 0.001, bVectorAntialias );

	m_pSoftMask       = NULL;
	m_bDeleteSoftMask = FALSE;

	m_bInNonIsolatedGroup = FALSE;

	m_pNext = NULL;
}

SState::SState(int nWidth, int nHeight, BOOL bVectorAntialias, SScreen *pScreen) 
{
	m_arrdMatrix[0] = 1;  m_arrdMatrix[1] = 0;
	m_arrdMatrix[2] = 0;  m_arrdMatrix[3] = 1;
	m_arrdMatrix[4] = 0;  m_arrdMatrix[5] = 0;

	SColor pColor;
	memset( &pColor, 0, sizeof(SColor) );
	m_pStrokePattern = new SSolidColor(pColor);
	m_pFillPattern   = new SSolidColor(pColor);

	m_pScreen = pScreen->Copy();

	m_pBlendFunction = NULL;
	m_dStrokeAlpha   = 1;
	m_dFillAlpha     = 1;
	m_dLineWidth     = 0;
	m_nLineCap       = LineCapButt;
	m_nLineJoin      = LineJoinMiter;
	m_dMiterLimit    = 10;
	m_dFlatness      = 1;
	m_pdLineDash     = NULL;
	m_nLineDashCount = 0;
	m_dLineDashPhase = 0;
	m_bStrokeAdjust  = FALSE;

	m_pClip = new SClip( 0, 0, nWidth - 0.001, nHeight - 0.001, bVectorAntialias );

	m_pSoftMask       = NULL;
	m_bDeleteSoftMask = FALSE;

	m_bInNonIsolatedGroup = FALSE;

	m_pNext = NULL;
}

SState::SState(SState *pState) 
{
	memcpy( m_arrdMatrix, pState->m_arrdMatrix, 6 * sizeof(double) );

	m_pStrokePattern = pState->m_pStrokePattern->Copy();
	m_pFillPattern   = pState->m_pFillPattern->Copy();
	m_pScreen        = pState->m_pScreen->Copy();
	m_pBlendFunction = pState->m_pBlendFunction;
	m_dStrokeAlpha   = pState->m_dStrokeAlpha;
	m_dFillAlpha     = pState->m_dFillAlpha;
	m_dLineWidth     = pState->m_dLineWidth;
	m_nLineCap       = pState->m_nLineCap;
	m_nLineJoin      = pState->m_nLineJoin;
	m_dMiterLimit    = pState->m_dMiterLimit;
	m_dFlatness      = pState->m_dFlatness;

	if ( pState->m_pdLineDash ) 
	{
		m_nLineDashCount = pState->m_nLineDashCount;
		m_pdLineDash = (double *)MemUtilsMallocArray( m_nLineDashCount, sizeof(double));
		memcpy( m_pdLineDash, pState->m_pdLineDash, m_nLineDashCount * sizeof(double));
	} 
	else 
	{
		m_pdLineDash     = NULL;
		m_nLineDashCount = 0;
	}
	m_dLineDashPhase = pState->m_dLineDashPhase;
	m_bStrokeAdjust  = pState->m_bStrokeAdjust;

	m_pClip = pState->m_pClip->Copy();

	m_pSoftMask       = pState->m_pSoftMask;
	m_bDeleteSoftMask = FALSE;

	m_bInNonIsolatedGroup = pState->m_bInNonIsolatedGroup;

	m_pNext = NULL;
}

SState::~SState() 
{
	if ( m_pStrokePattern )
	{
		delete m_pStrokePattern;
	}
	if ( m_pFillPattern )
	{
		delete m_pFillPattern;
	}
	if ( m_pScreen )
		delete m_pScreen;

	MemUtilsFree( m_pdLineDash );
	
	if ( m_pClip )
		delete m_pClip;

	if ( m_bDeleteSoftMask && m_pSoftMask ) 
		delete m_pSoftMask;
}

void SState::SetStrokePattern(SPattern *pStrokePattern) 
{
	if ( m_pStrokePattern )
		delete m_pStrokePattern;
	m_pStrokePattern = pStrokePattern;
}

void SState::SetFillPattern(SPattern *pFillPattern) 
{
	if ( m_pFillPattern )
		delete m_pFillPattern;
	m_pFillPattern = pFillPattern;
}

void SState::SetScreen(SScreen *pScreen) 
{
	if ( m_pScreen )
		delete m_pScreen;
	m_pScreen = pScreen;
}

void SState::SetLineDash(double *pdLineDash, int nLineDashCount, double dLineDashPhase) 
{
	MemUtilsFree( m_pdLineDash );
	m_nLineDashCount = nLineDashCount;

	if ( m_nLineDashCount > 0 ) 
	{
		m_pdLineDash = (double *)MemUtilsMallocArray( m_nLineDashCount, sizeof(double) );
		memcpy( m_pdLineDash, pdLineDash, m_nLineDashCount * sizeof(double) );
	} 
	else 
	{
		m_pdLineDash = NULL;
	}
	m_dLineDashPhase = dLineDashPhase;
}

void SState::SetSoftMask(SBitmap *pSoftMask) 
{
	if ( m_bDeleteSoftMask && m_pSoftMask ) 
	{
		delete m_pSoftMask;
	}
	m_pSoftMask = pSoftMask;
	m_bDeleteSoftMask = TRUE;
}
