#pragma once
#include "../PPTShape.h"

// 120
class CFlowChartConnectorType : public CPPTShape
{
public:
	CFlowChartConnectorType()

    {
        m_bConcentricFill = true;

        m_eJoin = NSOfficeDrawing::lineJoinRound;

        m_strPath = _T("m10800,qx,10800,10800,21600,21600,10800,10800,xe");

        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163"));
        LoadTextRect(_T("3163,3163,18437,18437"));
    }
};
