#pragma once
#include "../PPTShape.h"

// 125
class CFlowChartCollateType : public CPPTShape
{
public:
	CFlowChartCollateType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,21600l,21600,21600,,,xe");
        LoadConnectorsList(_T("10800,0;10800,10800;10800,21600"));
        LoadTextRect(_T("5400,5400,16200,16200"));
    }
};


