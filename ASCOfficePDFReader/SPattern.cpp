#include "stdafx.h"

#include "SMathExt.h"
#include "SScreen.h"
#include "SPattern.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SPattern
//-------------------------------------------------------------------------------------------------------------------------------

SPattern::SPattern() 
{
}

SPattern::~SPattern() 
{
}

//-------------------------------------------------------------------------------------------------------------------------------
// SSolidColor
//-------------------------------------------------------------------------------------------------------------------------------

SSolidColor::SSolidColor(SColorPointer pColor) 
{
	ColorCopy( m_pColor, pColor );
}

SSolidColor::~SSolidColor() 
{
}

void SSolidColor::GetColor(int nX, int nY, SColorPointer pColor) 
{
	ColorCopy( pColor, m_pColor );
}
