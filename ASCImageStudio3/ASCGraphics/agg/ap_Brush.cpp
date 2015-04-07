//----------------------------------------------------------------------------
// Brush class               - Version 0.1.2 - 01-04-2006
// BrushLinearGradient class 
// BrushTexture class        
// Marlon Mircevski class
//----------------------------------------------------------------------------
//#include "stdafx.h"
#include "ap_Brush.h"
#include "ap_Image.h"

namespace Aggplus
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BrushLinearGradient::BrushLinearGradient(/*const RectF &rect, */
										 const Color &color1, const Color &color2, 
										 REAL angle/*, BOOL isAngleScalable*/) :
												Brush(BrushTypeLinearGradient)
{
		m_rotate_angle_radians=angle;
		AddColor(color1);
		AddColor(color2);
// dont forget fix  Clone
		AddColor(Color(0,255,0));
		AddColor(Color(255,0,0));

		AddColor(Color(50,50,50));
		AddColor(Color(12,100,50));
		AddColor(Color(0,0,45));

}

Brush* BrushLinearGradient::Clone() const 
{ 
	return new BrushLinearGradient( m_colors[0], m_colors[1], m_rotate_angle_radians ); 
}
    
void  BrushLinearGradient::AddColor(const Color &clr) 
{
	if (m_colors.size()==0)
		m_colors.push_back(clr);
	else
	{
		size_t last_col_ind = m_colors.size()-1;
		m_colors.resize(m_colors.size()+50);
		agg::rgba8 last_col = m_colors[last_col_ind].GetAggColor();
		agg::rgba8 new_col = clr.GetAggColor();
		for (int i=1;i<=50;i++)
		{
			agg::rgba8& tmpCol = last_col.gradient(new_col, double(i) / double(50));
			m_colors[last_col_ind+i] = Color(tmpCol.a,tmpCol.b,tmpCol.g,tmpCol.r);
		}
	}
}
      
BrushTexture::BrushTexture(const Image *pImage, WrapMode wrapMode) : Brush(BrushTypeTextureFill), m_wrapMode(wrapMode)
{
 m_Image.CopyFrom(pImage);
}

Brush *BrushTexture::Clone() const
{
 BrushTexture *pRet=new BrushTexture(&m_Image, m_wrapMode);
 pRet->m_mtx=m_mtx;
 return pRet;
}



}
