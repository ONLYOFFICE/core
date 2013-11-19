#pragma once
#include "../PPTShape.h"

// 135
class CFlowChartDelayType : public CPPTShape
{
public:
	CFlowChartDelayType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,qx21600,10800,10800,21600l,21600,,xe");
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("0,3163,18437,18437"));
    }
};


