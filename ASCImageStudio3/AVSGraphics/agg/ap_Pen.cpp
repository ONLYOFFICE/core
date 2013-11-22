//----------------------------------------------------------------------------
// Pen class  - Version 0.1.1 - 28-10-2004
// Marlon Mircevski
//----------------------------------------------------------------------------
//#include "stdafx.h"
#include "ap_Pen.h"
#include "ap_Brush.h"

namespace Aggplus
{
		Pen::Pen(const Color& color, REAL width) : m_DashStyle(DashStyleSolid), m_LineJoin(LineJoinMiter)
		{
			m_pBrush=new BrushSolid(color);
			m_fWidth=width;
		}

		Pen::Pen(const Brush* brush, REAL width) : m_DashStyle(DashStyleSolid), m_LineJoin(LineJoinMiter)
		{
			m_pBrush=brush->Clone();
			m_fWidth=width;
		}

		Pen::~Pen()
		{
			delete(m_pBrush);
		}

		Pen *Pen::Clone(VOID)
		{
			Pen *pRet=new Pen(m_pBrush, m_fWidth);
			pRet->m_DashStyle=m_DashStyle;
			pRet->m_LineJoin=m_LineJoin;
			return(pRet);
		}

		Status Pen::SetBrush(const Brush* brush)
		{
			delete(m_pBrush);
			m_pBrush=brush->Clone();
			return Ok;
		}

		Status Pen::GetColor(Color* color) const
		{
			if(m_pBrush->GetType() == BrushTypeSolidColor)
			{
				((BrushSolid *)m_pBrush)->GetColor(color);
				return Ok;
			}

			return WrongState;
		}

		Status Pen::SetColor(const Color &color)
		{
			if(m_pBrush->GetType() == BrushTypeSolidColor)
			{
				((BrushSolid *)m_pBrush)->SetColor(color);
			}
			else
			{
				delete(m_pBrush);
				m_pBrush=new BrushSolid(color);
			}
		return Ok;
		}



}
