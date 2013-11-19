#pragma once
#include "../PPTShape.h"

// 16
class CCubeType : public CPPTShape
{
public:
	CCubeType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,l0@0,,21600@1,21600,21600@2,21600,xem0@0nfl@1@0,21600,em@1@0nfl@1,21600e");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("mid height #0"));
        AddGuide(_T("prod @1 1 2"));
        AddGuide(_T("prod @2 1 2"));
        AddGuide(_T("mid width #0"));

        m_arAdjustments.Add(5400);

        LoadConnectorsList(_T("@6,0;@4,@0;0,@3;@4,21600;@1,@3;21600,@5"));

		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("0,@0,@1,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.switchHandle = _T("true");
        oHandle1.yrange = _T("0,21600");
        m_arHandles.Add(oHandle1);

        m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
