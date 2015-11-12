#pragma once
#include "../PPTShape.h"

// 117
class CFlowChartPreparationType : public CPPTShape
{
public:
	CFlowChartPreparationType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m4353,l17214,r4386,10800l17214,21600r-12861,l,10800xe");

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("4353,0,17214,21600"));
    }
};