#pragma once
#include "../PPTShape.h"

// 68
class CUpArrowType : public CPPTShape
{
public:
	CUpArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m0@0l@1@0@1,21600@2,21600@2@0,21600@0,10800,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("prod #0 #1 10800"));
        AddGuide(_T("sum #0 0 @3"));

        m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(5400);

        LoadConnectorsList(_T("10800,0;0,@0;10800,21600;21600,@0"));

        m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@1,@4,@2,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#1,#0");
        oHandle1.xrange = _T("0,10800");
        oHandle1.yrange = _T("0,21600");
        m_arHandles.push_back(oHandle1);
    }
};
