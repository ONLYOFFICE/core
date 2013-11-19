#pragma once
#include "../PPTShape.h"

// 93
class CStripedRightArrowType : public CPPTShape
{
public:
	CStripedRightArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@0,l@0@1,3375@1,3375@2@0@2@0,21600,21600,10800xem1350@1l1350@2,2700@2,2700@1xem0@1l0@2,675@2,675@1xe");
        
        AddGuide(_T("val #0")); 
        AddGuide(_T("val #1")); 
        AddGuide(_T("sum height 0 #1")); 
        AddGuide(_T("sum 10800 0 #1")); 
        AddGuide(_T("sum width 0 #0")); 
        AddGuide(_T("prod @4 @3 10800"));
        AddGuide(_T("sum width 0 @5"));

		m_arAdjustments.Add(16200);
		m_arAdjustments.Add(5400);

        LoadConnectorsList(_T("@0,0;0,10800;@0,21600;21600,10800"));

		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);
		

        LoadTextRect(_T("3375,@1,@6,@2"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        oHandle1.xrange = _T("3375,21600");
        oHandle1.yrange = _T("0,10800"); 
        m_arHandles.Add(oHandle1);
    }
};
