#pragma once
#include "../PPTShape.h"

// 63
class CWedgeEllipseCalloutType : public CPPTShape
{
public:
	CWedgeEllipseCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("wr,,21600,21600@15@16@17@18l@21@22xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("sum 10800 0 #0"));
        AddGuide(_T("sum 10800 0 #1"));
        AddGuide(_T("atan2 @2 @3"));
        AddGuide(_T("sumangle @4 11 0"));
        AddGuide(_T("sumangle @4 0 11"));
        AddGuide(_T("cos 10800 @4"));
        AddGuide(_T("sin 10800 @4"));
        AddGuide(_T("cos 10800 @5"));
        AddGuide(_T("sin 10800 @5"));
        AddGuide(_T("cos 10800 @6"));
        AddGuide(_T("sin 10800 @6"));
        AddGuide(_T("sum 10800 0 @7"));
        AddGuide(_T("sum 10800 0 @8"));
        AddGuide(_T("sum 10800 0 @9"));
        AddGuide(_T("sum 10800 0 @10"));
        AddGuide(_T("sum 10800 0 @11"));
        AddGuide(_T("sum 10800 0 @12"));
        AddGuide(_T("mod @2 @3 0"));
        AddGuide(_T("sum @19 0 10800"));
        AddGuide(_T("if @20 #0 @13"));
        AddGuide(_T("if @20 #1 @14"));

		m_arAdjustments.Add(1350);
		m_arAdjustments.Add(25920);

        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163;@21,@22"));
        LoadTextRect(_T("3163,3163,18437,18437"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        m_arHandles.Add(oHandle1);
    }
};