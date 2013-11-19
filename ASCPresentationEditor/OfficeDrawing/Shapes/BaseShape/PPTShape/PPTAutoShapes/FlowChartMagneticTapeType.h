#pragma once
#include "../PPTShape.h"

// 131
class CFlowChartMagneticTapeType : public CPPTShape
{
public:
	CFlowChartMagneticTapeType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("ar,,21600,21600,18685,18165,10677,21597l20990,21597r,-3432xe");
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("3163,3163,18437,18437"));
    }
};


