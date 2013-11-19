#pragma once
#include "../PPTShape.h"

// 185
class CBracketPairType : public CPPTShape
{
public:
	CBracketPairType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinRound;
        //Encaps: Flat

        m_strPath = _T("m@0,nfqx0@0l0@2qy@0,21600em@1,nfqx21600@0l21600@2qy@1,21600em@0,nsqx0@0l0@2qy@0,21600l@1,21600qx21600@2l21600@0qy@1,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("prod @0 2929 10000"));
        AddGuide(_T("sum width 0 @3"));
        AddGuide(_T("sum height 0 @3"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("prod height 1 2"));
        
		m_arAdjustments.Add(3600);

        LoadConnectorsList(_T("@8,0;0,@9;@8,@7;@6,@9"));
        LoadTextRect(_T("@3,@3,@4,@5"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.switchHandle = _T("true");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.Add(oHandle1);

        m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
