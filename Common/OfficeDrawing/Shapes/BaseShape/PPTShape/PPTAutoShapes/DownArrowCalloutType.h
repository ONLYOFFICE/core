#pragma once
#include "../PPTShape.h"

// 80
class CDownArrowCalloutType : public CPPTShape
{
public:
	CDownArrowCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l21600,,21600@0@5@0@5@2@4@2,10800,21600@1@2@3@2@3@0,0@0xe"); 

        AddGuide(_T("val #0"));     
        AddGuide(_T("val #1"));     
        AddGuide(_T("val #2"));     
        AddGuide(_T("val #3"));     
        AddGuide(_T("sum 21600 0 #1"));     
        AddGuide(_T("sum 21600 0 #3"));     
        AddGuide(_T("prod #0 1 2"));

		m_arAdjustments.Add(14400);
		m_arAdjustments.Add(5400);
		m_arAdjustments.Add(18000);
		m_arAdjustments.Add(8100);

		LoadConnectorsList(_T("10800,0;0,@6;10800,21600;21600,@6"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("0,0,21600,@0"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("0,@2");
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("#1,bottomRight");
        oHandle2.xrange = _T("0,@3");
        m_arHandles.Add(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#3,#2");
        oHandle3.xrange = _T("@1,10800");
        oHandle3.yrange = _T("@0,21600");
        m_arHandles.Add(oHandle3); 


    }
};


