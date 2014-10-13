#pragma once
#include "../PPTShape.h"

// 62
class CWedgeRoundedRectCalloutType : public CPPTShape
{
public:
	CWedgeRoundedRectCalloutType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m3600,qx,3600l0@8@12@24,0@9,,18000qy3600,21600l@6,21600@15@27@7,21600,18000,21600qx21600,18000l21600@9@18@30,21600@8,21600,3600qy18000,l@7,0@21@33@6,xe");

        AddGuide(_T("sum 10800 0 #0"));
        AddGuide(_T("sum 10800 0 #1"));
        AddGuide(_T("sum #0 0 #1"));
        AddGuide(_T("sum @0 @1 0"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("if @0 3600 12600"));
        AddGuide(_T("if @0 9000 18000"));
        AddGuide(_T("if @1 3600 12600"));
        AddGuide(_T("if @1 9000 18000"));
        AddGuide(_T("if @2 0 #0"));
        AddGuide(_T("if @3 @10 0"));
        AddGuide(_T("if #0 0 @11"));
        AddGuide(_T("if @2 @6 #0"));
        AddGuide(_T("if @3 @6 @13"));
        AddGuide(_T("if @5 @6 @14"));
        AddGuide(_T("if @2 #0 21600"));
        AddGuide(_T("if @3 21600 @16"));
        AddGuide(_T("if @4 21600 @17"));
        AddGuide(_T("if @2 #0 @6"));
        AddGuide(_T("if @3 @19 @6"));
        AddGuide(_T("if #1 @6 @20"));
        AddGuide(_T("if @2 @8 #1"));
        AddGuide(_T("if @3 @22 @8"));
        AddGuide(_T("if #0 @8 @23"));
        AddGuide(_T("if @2 21600 #1"));
        AddGuide(_T("if @3 21600 @25"));
        AddGuide(_T("if @5 21600 @26"));
        AddGuide(_T("if @2 #1 @8"));
        AddGuide(_T("if @3 @8 @28"));
        AddGuide(_T("if @4 @8 @29"));
        AddGuide(_T("if @2 #1 0"));
        AddGuide(_T("if @3 @31 0"));
        AddGuide(_T("if #1 0 @32"));
        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));

		m_arAdjustments.push_back(1350);
		m_arAdjustments.push_back(25920);
        
        LoadConnectorsList(_T("10800,0;0,10800;10800,21600;21600,10800;@34,@35"));
        LoadTextRect(_T("791,791,20809,20809"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        m_arHandles.push_back(oHandle1);
    }
};
