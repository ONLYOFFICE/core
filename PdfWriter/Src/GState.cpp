#include "GState.h"

namespace PdfWriter
{
	const static char* c_sBlendModeNames[] = 
	{
		"Normal",
		"Multiply",
		"Screen",
		"Overlay",
		"Darken",
		"Lighten",
		"ColorDodge",
		"ColorBurn",
		"HardLight",
		"SoftLight",
		"Difference",
		"Exclusion"
	};
	//----------------------------------------------------------------------------------------
	// CGrState
	//----------------------------------------------------------------------------------------
	CGrState::CGrState(CGrState* pCurrent)
	{
		if (pCurrent)
		{
			m_oMatrix        = pCurrent->m_oMatrix;
			m_dLineWidth     = pCurrent->m_dLineWidth;
			m_eLineCap       = pCurrent->m_eLineCap;
			m_eLineJoin      = pCurrent->m_eLineJoin;
			m_dMiterLimit    = pCurrent->m_dMiterLimit;
			m_oDashMode      = pCurrent->m_oDashMode;
			m_dFlatness      = pCurrent->m_dFlatness;

			m_dCharSpace     = pCurrent->m_dCharSpace;
			m_dWordSpace     = pCurrent->m_dWordSpace;
			m_dHScalling     = pCurrent->m_dHScalling;
			m_dTextLeading   = pCurrent->m_dTextLeading;
			m_eRenderingMode = pCurrent->m_eRenderingMode;
			m_dTextRise      = pCurrent->m_dTextRise;

			m_oFillColor     = pCurrent->m_oFillColor;
			m_oStrokeColor   = pCurrent->m_oStrokeColor;

			m_pFont          = pCurrent->m_pFont;
			m_dFontSize      = pCurrent->m_dFontSize;

			m_pPrev          = pCurrent;
			m_unDepth        = pCurrent->m_unDepth + 1;
		}
		else
		{
			m_dLineWidth     = DEF_LINEWIDTH;
			m_eLineCap       = DEF_LINECAP;
			m_eLineJoin      = DEF_LINEJOIN;
			m_dMiterLimit    = DEF_MITERLIMIT;
			m_dFlatness      = DEF_FLATNESS;

			m_dCharSpace     = DEF_CHARSPACE;
			m_dWordSpace     = DEF_WORDSPACE;
			m_dHScalling     = DEF_HSCALING;
			m_dTextLeading   = DEF_LEADING;
			m_eRenderingMode = DEF_RENDERING_MODE;
			m_dTextRise      = DEF_RISE;

			m_pFont          = NULL;
			m_dFontSize      = 0;

			m_pPrev          = NULL;
			m_unDepth        = 1;
		}
	}
	CGrState::~CGrState()
	{
	}
	//----------------------------------------------------------------------------------------
	// CExtGrState
	//----------------------------------------------------------------------------------------
	CExtGrState::CExtGrState(CXref* pXref)
	{
		pXref->Add(this);
		Add("Type", "ExtGState");

		m_eBlendMode   = blendmode_Unknown;
		m_dAlphaFill   = 1;
		m_dAlphaStroke = 1;
		m_bStrokeAdj   = true;
	}
	void CExtGrState::SetAlphaStroke     (double dValue)
	{
		dValue = min(1, max(0, dValue));
	    Add("CA", dValue);
	}
	void CExtGrState::SetAlphaFill       (double dValue)
	{
		dValue = min(1, max(0, dValue));
	    Add("ca", dValue);
	}	
	void CExtGrState::SetBlendMode       (EBlendMode eBlendMode)
	{
		eBlendMode = min(blendmode_Max, max(blendmode_Min, eBlendMode));
		Add("BM", c_sBlendModeNames[(int)eBlendMode]);
	}	
	void CExtGrState::SetStrokeAdjustment(bool bValue)
	{
	    Add("SA", bValue);
	}
	double     CExtGrState::GetAlphaStroke()
	{
		return m_dAlphaStroke;
	}
	double     CExtGrState::GetAlphaFill()
	{
		return m_dAlphaFill;
	}
	EBlendMode CExtGrState::GetBlendMode()
	{
		return m_eBlendMode;
	}
	bool       CExtGrState::GetStrokeAdjustment()
	{
		return m_bStrokeAdj;
	}
}
