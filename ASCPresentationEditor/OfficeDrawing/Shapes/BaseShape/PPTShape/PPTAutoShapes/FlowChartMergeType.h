#pragma once
#include "../PPTShape.h"

// 128
class CFlowChartMergeType : public CPPTShape
{
public:
	CFlowChartMergeType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l21600,,10800,21600xe");
        LoadConnectorsList(_T("10800,0;5400,10800;10800,21600;16200,10800"));
        LoadTextRect(_T("5400,0,16200,10800"));
    }
};


