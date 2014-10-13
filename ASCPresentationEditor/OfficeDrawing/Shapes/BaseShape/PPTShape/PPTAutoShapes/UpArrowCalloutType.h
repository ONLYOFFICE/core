#pragma once
#include "../PPTShape.h"

// 79
class CUpArrowCalloutType : public CPPTShape
{
public:
	CUpArrowCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m0@0l@3@0@3@2@1@2,10800,0@4@2@5@2@5@0,21600@0,21600,21600,,21600xe"); 

        AddGuide(_T("val #0"));     
        AddGuide(_T("val #1"));     
        AddGuide(_T("val #2"));     
        AddGuide(_T("val #3"));     
        AddGuide(_T("sum 21600 0 #1"));     
        AddGuide(_T("sum 21600 0 #3"));     
        AddGuide(_T("sum #0 21600 0"));     
        AddGuide(_T("prod @6 1 2"));

		m_arAdjustments.push_back(7200);
		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(3600);
		m_arAdjustments.push_back(8100);

        LoadConnectorsList(_T("10800,0;0,@7;10800,21600;21600,@7"));

        m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("0,@0,21600,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("@2,21600");
        m_arHandles.push_back(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("#1,topLeft");
        oHandle2.xrange = _T("0,@3");
        m_arHandles.push_back(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#3,#2");
        oHandle3.xrange = _T("@1,10800");
        oHandle3.yrange = _T("0,@0");
        m_arHandles.push_back(oHandle3);
    }
};


