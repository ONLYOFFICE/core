// Pen.h: interface for the Pen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_PEN_H__F0CFF76B_84AD_4247_860B_789EE833ACED__INCLUDED_)
#define Aggplus_PEN_H__F0CFF76B_84AD_4247_860B_789EE833ACED__INCLUDED_

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"
#include "ap_Color.h"

namespace Aggplus
{
	class Brush;

	class Pen
	{
	public:
		Pen(const Color& color, REAL width = 1.0f);
		Pen(const Brush* brush, REAL width = 1.0f);
		virtual ~Pen();
		Pen *Clone(void);

		inline void SetLineJoin(LineJoin newLineJoin) { m_LineJoin=newLineJoin; }
		inline void SetDashStyle(DashStyle newDashStyle) { m_DashStyle=newDashStyle; }
		inline DashStyle GetDashStyle(void) const { return(m_DashStyle); }
		inline LineJoin GetLineJoin(void) const { return(m_LineJoin); }
		inline Brush *GetBrush(void) const { return(m_pBrush); } //No! Gdi+ returns Clone of Brush
		Status SetBrush(const Brush* brush);
		Status GetColor(Color* color) const;
		Status SetColor(const Color &color);
		inline REAL GetWidth(void) const { return(m_fWidth); }

	protected:
		Brush *m_pBrush;
		REAL m_fWidth;
		LineJoin m_LineJoin;
		DashStyle m_DashStyle;

	};


}
#endif // !defined(Aggplus_PEN_H__F0CFF76B_84AD_4247_860B_789EE833ACED__INCLUDED_)
