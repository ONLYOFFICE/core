#pragma once
#include "../PPTShape.h"

// 83
class CQuadArrowCalloutType : public CPPTShape
{
public:
	CQuadArrowCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@0@0l@3@0@3@2@1@2,10800,0@4@2@5@2@5@0@8@0@8@3@9@3@9@1,21600,10800@9@4@9@5@8@5@8@8@5@8@5@9@4@9,10800,21600@1@9@3@9@3@8@0@8@0@5@2@5@2@4,,10800@2@1@2@3@0@3xe"); 

        AddGuide(_T("val #0")); 
        AddGuide(_T("val #1")); 
        AddGuide(_T("val #2")); 
        AddGuide(_T("val #3")); 
        AddGuide(_T("sum 21600 0 #1")); 
        AddGuide(_T("sum 21600 0 #3")); 
        AddGuide(_T("sum #0 21600 0")); 
        AddGuide(_T("prod @6 1 2")); 
        AddGuide(_T("sum 21600 0 #0")); 
        AddGuide(_T("sum 21600 0 #2"));
        
		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(8100);
		m_arAdjustments.push_back(2700);
		m_arAdjustments.push_back(9450);

		LoadConnectorsList(_T("Rectangle"));

        LoadTextRect(_T("@0,@0,@8,@8"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("@2,@1");
        m_arHandles.push_back(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("#1,topLeft");
        oHandle2.xrange = _T("@0,@3");
        m_arHandles.push_back(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#3,#2");
        oHandle3.xrange = _T("@1,10800");
        oHandle3.yrange = _T("0,@0");
        m_arHandles.push_back(oHandle3);
    }
};


