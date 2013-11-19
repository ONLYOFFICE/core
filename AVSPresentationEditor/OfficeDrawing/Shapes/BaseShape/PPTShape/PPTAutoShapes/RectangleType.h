#pragma once
#include "../PPTShape.h"

// 1
class CRectangleType : public CPPTShape
{
public:
	CRectangleType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l,21600r21600,l21600,xe");
     }
};
