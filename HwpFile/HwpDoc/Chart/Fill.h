#ifndef FILL_H
#define FILL_H

#include "Brush.h"
#include "Gradient.h"
#include "VtPicture.h"

namespace HWP { namespace CHART
{
class CFill
{
	CBrush m_oBrush;
	// union
	// {
	// 	CGradient m_oGradient;
	// 	CVtPicture m_oPicture;
	// } m_oGradient;
	int m_nStyle;
	CVtPicture m_oVtPicture;
public:
	CFill();
};
}}

#endif // FILL_H
