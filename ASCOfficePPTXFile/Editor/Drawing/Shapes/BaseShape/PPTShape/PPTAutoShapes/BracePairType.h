#pragma once
#include "../PPTShape.h"

// 186
class CBracePairType : public CPPTShape
{
public:
	CBracePairType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m@9,nfqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600em@10,nfqx@5@0l@5@7qy21600@4@5@8l@5@6qy@10,21600em@9,nsqx@0@0l@0@7qy0@4@0@8l@0@6qy@9,21600l@10,21600qx@5@6l@5@8qy21600@4@5@7l@5@0qy@10,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("prod height 1 2"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("sum @4 0 #0"));
        AddGuide(_T("sum @4 #0 0"));
        AddGuide(_T("prod #0 2 1"));
        AddGuide(_T("sum width 0 @9"));
        AddGuide(_T("prod #0 9598 32768"));
        AddGuide(_T("sum height 0 @11"));
        AddGuide(_T("sum @11 #0 0"));
        AddGuide(_T("sum width 0 @13"));

        m_arAdjustments.push_back(1800);

        LoadConnectorsList(_T("@3,0;0,@4;@3,@2;@1,@4"));
        LoadTextRect(_T("@13,@11,@14,@12"));

        CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.switchHandle = _T("true");
        oHandle1.yrange = _T("0,5400");
        m_arHandles.push_back(oHandle1);

        m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
