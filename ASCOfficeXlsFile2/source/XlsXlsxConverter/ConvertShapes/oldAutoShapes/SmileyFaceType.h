#pragma once
#include "../PPTShape.h"

// 96
class CSmileyFaceType : public CPPTShape
{
public:
	CSmileyFaceType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinRound;

        m_strPath = _T("m10800,qx,10800,10800,21600,21600,10800,10800,xem7340,6445qx6215,7570,7340,8695,8465,7570,7340,6445xnfem14260,6445qx13135,7570,14260,8695,15385,7570,14260,6445xnfem4960@0c8853@3,12747@3,16640@0nfe");

        AddGuide(_T("sum 33030 0 #0"));
        AddGuide(_T("prod #0 4 3"));
        AddGuide(_T("prod @0 1 3"));
        AddGuide(_T("sum @1 0 @2"));

		m_arAdjustments.push_back(17520);

        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163"));
        LoadTextRect(_T("3163,3163,18437,18437"));

        CHandle_ oHandle1;
        oHandle1.position = _T("center,#0");
        oHandle1.yrange = _T("15510,17520");
        m_arHandles.push_back(oHandle1);
    }
};