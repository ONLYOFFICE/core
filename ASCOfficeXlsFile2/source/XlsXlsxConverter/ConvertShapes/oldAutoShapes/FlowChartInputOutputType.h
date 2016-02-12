#pragma once
#include "../PPTShape.h"

// 111
class CFlowChartInputOutputType : public CPPTShape
{
public:
	CFlowChartInputOutputType()

    {
        m_bConcentricFill = true;

        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m4321,l21600,,17204,21600,,21600xe");

        LoadConnectorsList(_T("2961,0;10800,0;2161,10800;8602,21600;10800,21600;19402,10800"));
        LoadTextRect(_T("4321,0,17204,21600"));
    }
};
