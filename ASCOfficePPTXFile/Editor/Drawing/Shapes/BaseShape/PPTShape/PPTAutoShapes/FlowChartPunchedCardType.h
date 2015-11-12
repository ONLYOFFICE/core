#pragma once
#include "../PPTShape.h"

// 121
class CFlowChartPunchedCardType : public CPPTShape
{
public:
	CFlowChartPunchedCardType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m4321,l21600,r,21600l,21600,,4338xe"); 
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("0,4321,21600,21600"));
    }
};


