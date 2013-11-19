#pragma once
#include "../PPTShape.h"

// 182
class CLeftRightUpArrow : public CPPTShape
{
public:
	CLeftRightUpArrow()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l@0@2@1@2@1@6@7@6@7@5,0@8@7,21600@7@9@10@9@10,21600,21600@8@10@5@10@6@4@6@4@2@3@2xe");

		AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("val #2"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("prod @0 21600 @3"));
        AddGuide(_T("prod @1 21600 @3"));
        AddGuide(_T("prod @2 @3 21600"));
        AddGuide(_T("prod 10800 21600 @3"));
        AddGuide(_T("prod @4 21600 @3"));
        AddGuide(_T("sum 21600 0 @7"));
        AddGuide(_T("sum @5 0 @8"));
        AddGuide(_T("sum @6 0 @8"));
        AddGuide(_T("prod @12 @7 @11"));
        AddGuide(_T("sum 21600 0 @13"));
        AddGuide(_T("sum @0 0 10800"));
        AddGuide(_T("sum @1 0 10800"));
        AddGuide(_T("prod @2 @16 @15"));

		m_arAdjustments.Add(6480);
		m_arAdjustments.Add(8640);
		m_arAdjustments.Add(6171);

        LoadConnectorsList(_T("10800,0;0,@8;10800,@9;21600,@8"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("@13,@6,@14,@9;@1,@17,@4,@9"));
       
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("@2,@1");
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("#1,#2");
        oHandle2.xrange = _T("@0,10800");
        oHandle2.yrange = _T("0,@5"); 
        m_arHandles.Add(oHandle2);
    }
};
