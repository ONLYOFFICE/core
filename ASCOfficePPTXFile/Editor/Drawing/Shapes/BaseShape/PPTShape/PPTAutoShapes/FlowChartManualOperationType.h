#pragma once
#include "../PPTShape.h"

// 119
class CFlowChartManualOperationType : public CPPTShape
{
public:
	CFlowChartManualOperationType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,l21600,,17240,21600r-12880,xe");

        LoadConnectorsList(_T("10800,0;2180,10800;10800,21600;19420,10800"));
        LoadTextRect(_T("4321,0,17204,21600"));
    }
};
