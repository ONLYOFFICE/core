#pragma once
#include "../PPTShape.h"

// 98
class CHorizontalScrollType : public CPPTShape
{
public:
	CHorizontalScrollType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m0@5qy@2@1l@0@1@0@2qy@7,,21600@2l21600@9qy@7@10l@1@10@1@11qy@2,21600,0@11xem0@5nfqy@2@6@1@5@3@4@2@5l@2@6em@1@5nfl@1@10em21600@2nfqy@7@1l@0@1em@0@2nfqy@8@3@7@2l@7@1e");

		AddGuide(_T("sum width 0 #0"));  
		AddGuide(_T("val #0"));  
		AddGuide(_T("prod @1 1 2"));  
		AddGuide(_T("prod @1 3 4"));  
		AddGuide(_T("prod @1 5 4")); 
		AddGuide(_T("prod @1 3 2"));  
		AddGuide(_T("prod @1 2 1"));  
		AddGuide(_T("sum width 0 @2"));  
		AddGuide(_T("sum width 0 @3"));  
		AddGuide(_T("sum height 0 @5"));  
		AddGuide(_T("sum height 0 @1"));  
		AddGuide(_T("sum height 0 @2"));  
		AddGuide(_T("val width"));  
		AddGuide(_T("prod width 1 2"));  
		AddGuide(_T("prod height 1 2"));

        m_arAdjustments.push_back(2700);

        LoadConnectorsList(_T("@13,@1;0,@14;@13,@10;@12,@14"));
        LoadTextRect(_T("@1,@1,@7,@10"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("0,5400");
        m_arHandles.push_back(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};