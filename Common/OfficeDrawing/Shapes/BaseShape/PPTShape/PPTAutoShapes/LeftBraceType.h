#pragma once
#include "../PPTShape.h"

// 87
class CLeftBraceType : public CPPTShape
{
public:
	CLeftBraceType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Endcaps: Flat

        m_strPath = _T("m21600,qx10800@0l10800@2qy0@11,10800@3l10800@1qy21600,21600e");

		AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("sum #1 0 #0"));
        AddGuide(_T("sum #1 #0 0"));
        AddGuide(_T("prod #0 9598 32768"));
        AddGuide(_T("sum 21600 0 @4"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("min #1 @6"));
        AddGuide(_T("prod @7 1 2"));
        AddGuide(_T("prod #0 2 1"));
        AddGuide(_T("sum 21600 0 @9"));
        AddGuide(_T("val #1"));
        
		m_arAdjustments.Add(1800);
		m_arAdjustments.Add(10800);

		LoadConnectorsList(_T("21600,0;0,10800;21600,21600"));
        LoadTextRect(_T("13963,@4,21600,@5"));

        CHandle_ oHandle1;
        CHandle_ oHandle2;
        
		oHandle1.position = _T("center,#0");
        oHandle1.yrange = _T("0,@8");
        oHandle2.position = _T("topLeft,#1");
        oHandle2.yrange = _T("@9,@10");
        
		m_arHandles.Add(oHandle1);
        m_arHandles.Add(oHandle2);
    }
};