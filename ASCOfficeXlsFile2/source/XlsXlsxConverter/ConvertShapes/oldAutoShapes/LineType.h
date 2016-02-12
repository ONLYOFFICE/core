#pragma once
#include "../PPTShape.h"

// 20
class CLineType : public CPPTShape
{
public:
	CLineType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinRound;
        m_strPath = _T("m0,0l21600,21600nfe");

		LoadConnectorsList(_T("0,0;21600,21600;10800,10800"));
    }
};