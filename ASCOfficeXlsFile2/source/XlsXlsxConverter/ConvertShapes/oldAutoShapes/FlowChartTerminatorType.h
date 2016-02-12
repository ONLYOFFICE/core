#pragma once
#include "../PPTShape.h"

// 116
class CFlowChartTerminatorType : public CPPTShape
{
public:
	CFlowChartTerminatorType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinRound;

        m_strPath = _T("m3475,qx,10800,3475,21600l18125,21600qx21600,10800,18125,xe");

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("1018,3163,20582,18437"));
    }
};
