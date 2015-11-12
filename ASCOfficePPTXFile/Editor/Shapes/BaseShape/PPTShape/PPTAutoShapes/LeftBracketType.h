#pragma once
#include "../PPTShape.h"

// 85
class CLeftBracketType : public CPPTShape
{
public:
	CLeftBracketType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinRound;
        //Endcaps: Flat

        m_strPath = _T("m21600,qx0@0l0@1qy21600,21600e");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("prod #0 9598 32768"));
        AddGuide(_T("sum 21600 0 @2"));

		m_arAdjustments.push_back(1800);

        LoadConnectorsList(_T("21600,0;0,10800;21600,21600"));
        LoadTextRect(_T("6326,@2,21600,@3"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);
    }
};
