#pragma once
#include "../PPTShape.h"

// 54
class CRibbonUpType : public CPPTShape
{
public:
	CRibbonUpType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m0@29l@3@29qx@4@19l@4@10@5@10@5@19qy@6@29l@28@29@26@22@28@23@9@23@9@24qy@8,l@1,qx@0@24l@0@23,0@23,2700@22xem@4@19nfqy@3@20l@1@20qx@0@21@1@10l@4@10em@5@19nfqy@6@20l@8@20qx@9@21@8@10l@5@10em@0@21nfl@0@23em@9@21nfl@9@23e");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum @0 675 0"));
        AddGuide(_T("sum @1 675 0"));
        AddGuide(_T("sum @2 675 0"));
        AddGuide(_T("sum @3 675 0"));
        AddGuide(_T("sum width 0 @4"));
        AddGuide(_T("sum width 0 @3"));
        AddGuide(_T("sum width 0 @2"));
        AddGuide(_T("sum width 0 @1"));
        AddGuide(_T("sum width 0 @0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("prod @10 1 4"));
        AddGuide(_T("prod @10 1 2"));
        AddGuide(_T("prod @10 3 4"));
        AddGuide(_T("prod height 3 4"));
        AddGuide(_T("prod height 1 2"));
        AddGuide(_T("prod height 1 4"));
        AddGuide(_T("prod height 3 2"));
        AddGuide(_T("prod height 2 3"));
        AddGuide(_T("sum @11 @14 0"));
        AddGuide(_T("sum @12 @15 0"));
        AddGuide(_T("sum @13 @16 0"));
        AddGuide(_T("sum @17 0 @20"));
        AddGuide(_T("sum height 0 @10"));
        AddGuide(_T("sum height 0 @19"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("sum width 0 2700"));
        AddGuide(_T("sum @25 0 2700"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));

		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(18900);
       
        LoadConnectorsList(_T("@25,0;2700,@22;@25,@10;@26,@22"));

        m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@0,0,@9,@10"));

        CHandle_ oHandle1;
        CHandle_ oHandle2;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("2700,8100");
        oHandle2.position = _T("center,#1");
        oHandle2.yrange = _T("14400,21600");
        m_arHandles.push_back(oHandle1);
        m_arHandles.push_back(oHandle2);
    }
};
