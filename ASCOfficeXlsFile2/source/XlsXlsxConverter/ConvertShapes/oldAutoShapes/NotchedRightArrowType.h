#pragma once
#include "../PPTShape.h"

// 94
class CNotchedRightArrowType : public CPPTShape
{
public:
	CNotchedRightArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@0,l@0@1,0@1@5,10800,0@2@0@2@0,21600,21600,10800xe"); 

        AddGuide(_T("val #0")); 
        AddGuide(_T("val #1")); 
        AddGuide(_T("sum height 0 #1")); 
        AddGuide(_T("sum 10800 0 #1")); 
        AddGuide(_T("sum width 0 #0")); 
        AddGuide(_T("prod @4 @3 10800")); 
        AddGuide(_T("sum width 0 @5"));

		m_arAdjustments.push_back(16200);
		m_arAdjustments.push_back(5400);

        LoadConnectorsList(_T("@0,0;@5,10800;@0,21600;21600,10800"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@5,@1,@6,@2"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        oHandle1.xrange = _T("0,21600");
        oHandle1.yrange = _T("0,10800"); 
        m_arHandles.push_back(oHandle1);
    }
};
