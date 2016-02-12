#pragma once
#include "../PPTShape.h"

// 130
class CFlowChartOnlineStorageType : public CPPTShape
{
public:
	CFlowChartOnlineStorageType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m3600,21597c2662,21202,1837,20075,1087,18440,487,16240,75,13590,,10770,75,8007,487,5412,1087,3045,1837,1465,2662,337,3600,l21597,v-937,337,-1687,1465,-2512,3045c18485,5412,18072,8007,17997,10770v75,2820,488,5470,1088,7670c19910,20075,20660,21202,21597,21597xe");
        LoadConnectorsList(_T("10800,0;0,10800;10800,21600;17997,10800"));
        LoadTextRect(_T("3600,0,17997,21600"));
    }
};


