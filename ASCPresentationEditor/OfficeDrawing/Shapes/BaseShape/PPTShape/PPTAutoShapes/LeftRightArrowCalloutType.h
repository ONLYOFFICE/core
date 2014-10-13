#pragma once
#include "../PPTShape.h"

// 81
class CLeftRightArrowCalloutType : public CPPTShape
{
public:
	CLeftRightArrowCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@0,l@0@3@2@3@2@1,,10800@2@4@2@5@0@5@0,21600@8,21600@8@5@9@5@9@4,21600,10800@9@1@9@3@8@3@8,xe"); 

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
		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(2700);
		m_arAdjustments.push_back(8100);

		LoadConnectorsList(_T("10800,0;0,10800;10800,21600;21600,10800"));
        
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@0,0,@8,21600"));
        
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("@2,10800");
        m_arHandles.push_back(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("topLeft,#1");
        oHandle2.yrange = _T("0,@3");
        m_arHandles.push_back(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#2,#3");
        oHandle3.xrange = _T("0,@0");
        oHandle3.yrange = _T("@1,10800");
        m_arHandles.push_back(oHandle3); 
    }
};


