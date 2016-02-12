#pragma once
#include "../PPTShape.h"

// 127
class CFlowChartExtractType : public CPPTShape
{
public:
	CFlowChartExtractType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l21600,21600,,21600xe");
        LoadConnectorsList(_T("10800,0;5400,10800;10800,21600;16200,10800"));
        LoadTextRect(_T("5400,10800,16200,21600"));
    }
};


