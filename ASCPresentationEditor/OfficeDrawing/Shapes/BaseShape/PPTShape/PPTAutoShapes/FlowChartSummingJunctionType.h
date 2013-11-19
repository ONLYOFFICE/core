#pragma once
#include "../PPTShape.h"

// 123
class CFlowChartSummingJunctionType : public CPPTShape
{
public:
	CFlowChartSummingJunctionType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,qx,10800,10800,21600,21600,10800,10800,xem3163,3163nfl18437,18437em3163,18437nfl18437,3163e"); 
        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163"));
        LoadTextRect(_T("3163,3163,18437,18437"));
    }
};


