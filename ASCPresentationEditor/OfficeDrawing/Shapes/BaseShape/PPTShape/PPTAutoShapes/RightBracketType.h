#pragma once
#include "../PPTShape.h"

// 86
class CRightBracketType : public CPPTShape
{
public:
	CRightBracketType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinRound;
        //Endcaps: Flat

        m_strPath = _T("m,qx21600@0l21600@1qy,21600e");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("prod #0 9598 32768"));
        AddGuide(_T("sum 21600 0 @2"));

        m_arAdjustments.Add(1800);

        LoadConnectorsList(_T("0,0;0,21600;21600,10800"));
        LoadTextRect(_T("0,@2,15274,@3"));

        CHandle_ oHandle1;
        oHandle1.position = _T("bottomRight,#0");
        oHandle1.yrange = _T("0,10800");
        m_arHandles.Add(oHandle1);
    }
};
