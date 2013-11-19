#pragma once
#include "../PPTShape.h"

// 67
class CDownArrowType : public CPPTShape
{
public:
	CDownArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m0@0l@1@0@1,0@2,0@2@0,21600@0,10800,21600xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("sum height 0 #1"));
        AddGuide(_T("sum 10800 0 #1"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("prod @4 @3 10800"));
        AddGuide(_T("sum width 0 @5"));

        m_arAdjustments.Add(16200);
		m_arAdjustments.Add(5400);

        LoadConnectorsList(_T("10800,0;0,@0;10800,21600;21600,@0"));

        m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);
        
		LoadTextRect(_T("@1,0,@2,@6"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#1,#0");
        oHandle1.xrange = _T("0,10800");
        oHandle1.yrange = _T("0,21600");
        m_arHandles.Add(oHandle1);
    }
};
