#pragma once
#include "../PPTShape.h"

// 6
class CRightTriangleType : public CPPTShape
{
public:
	CRightTriangleType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,l,21600r21600,xe");

        LoadConnectorsList(_T("0,0;0,10800;0,21600;10800,21600;21600,21600;10800,10800"));
        LoadTextRect(_T("1800,12600,12600,19800"));
    }
};
