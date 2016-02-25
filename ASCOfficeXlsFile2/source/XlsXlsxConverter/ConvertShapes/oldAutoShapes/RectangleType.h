#pragma once
#include "../CustomShape.h"

// 1
namespace NSCustomShapesConvert
{
class CRectangleType : public CCustomShape
{
public:
	CRectangleType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l,21600r21600,l21600,xe");
     }
};
}
