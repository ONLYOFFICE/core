#pragma once
#include "../PPTShape.h"

// 88
class CRightBracetype : public CPPTShape
{
public: 
	CRightBracetype()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,qx10800@0l10800@2qy21600@11,10800@3l10800@1qy,21600e");

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

		m_arAdjustments.push_back(1800);
		m_arAdjustments.push_back(10800);

        LoadConnectorsList(_T("0,0;21600,@11;0,21600"));
        LoadTextRect(_T("0,@4,7637,@5"));

        CHandle_ oHandle1;
        CHandle_ oHandle2;
        oHandle1.position = _T("center,#0");
        oHandle1.yrange = _T("0,@8");
        oHandle2.position = _T("bottomRight,#1");
        oHandle2.yrange = _T("@9,@10");
        m_arHandles.push_back(oHandle1);
        m_arHandles.push_back(oHandle2);
    }
};
