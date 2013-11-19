#pragma once
#include "../PPTShape.h"

// 22
class CCanType : public CPPTShape
{
public:
	CCanType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinRound;
        m_strPath = _T("m10800,qx0@1l0@2qy10800,21600,21600@2l21600@1qy10800,xem0@1qy10800@0,21600@1nfe");

        AddGuide(_T("val #0"));
        AddGuide(_T("prod #0 1 2"));
        AddGuide(_T("sum height 0 @1"));

        m_arAdjustments.Add(5400);

        LoadConnectorsList(_T("10800,@0;10800,0;0,10800;10800,21600;21600,10800"));
        m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("0,@0,21600,@2"));

        CHandle_ oHandle1;
        oHandle1.position = _T("center,#0");
        oHandle1.yrange = _T("0,10800");
        m_arHandles.Add(oHandle1);
    }
};