#pragma once
#include "../PPTShape.h"

// 12
class CStarType : public CPPTShape
{
public:
	CStarType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m10800,l8280,8259,,8259r6720,5146l4200,21600r6600,-5019l17400,21600,14880,13405,21600,8259r-8280,xe");

        LoadConnectorsList(_T("10800,0;0,8259;4200,21600;17400,21600;21600,8259"));
        LoadTextRect(_T("6720,8259,14880,15628"));
    }
};
