#pragma once
#include "../PPTShape.h"

// 78
class CRightArrowCalloutType : public CPPTShape
{
public:
	CRightArrowCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l,21600@0,21600@0@5@2@5@2@4,21600,10800@2@1@2@3@0@3@0,x"); 

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

        LoadConnectorsList(_T("@6,0;0,10800;@6,21600;21600,10800"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("0,0,@0,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,@2");
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("bottomRight,#1");
        oHandle2.yrange = _T("0,@3");
        m_arHandles.Add(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#2,#3");
        oHandle3.xrange = _T("@0,21600");
        oHandle3.yrange = _T("@1,10800");
        m_arHandles.Add(oHandle3);
    }
};


