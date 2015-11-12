#pragma once
#include "../PPTShape.h"

// 118
class CFlowChartManualInputType : public CPPTShape
{
public:
	CFlowChartManualInputType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,4292l21600,r,21600l,21600xe");

        LoadConnectorsList(_T("10800,2146;0,10800;10800,21600;21600,10800"));
        LoadTextRect(_T("0,4291,21600,21600"));

    }
};