#pragma once
#include "../PPTShape.h"

// 190
class CActionButtonHomeType : public CPPTShape
{
public:
	CActionButtonHomeType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        
		m_strPath = _T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@3@9nfl@11@4@28@4@28@10@33@10@33@4@12@4@32@26@32@24@31@24@31@25xem@31@25nfl@32@26em@28@4nfl@33@4em@29@10nfl@29@27@30@27@30@10e");

		AddGuide(_T("val #0"));  
		AddGuide(_T("sum width 0 #0"));  
		AddGuide(_T("sum height 0 #0"));  
		AddGuide(_T("prod width 1 2"));  
		AddGuide(_T("prod height 1 2"));  
		AddGuide(_T("prod #0 1 2"));  
		AddGuide(_T("prod #0 3 2"));  
		AddGuide(_T("sum @1 @5 0"));  
		AddGuide(_T("sum @2 @5 0"));  
		AddGuide(_T("sum @0 @4 8100"));  
		AddGuide(_T("sum @2 8100 @4"));  
		AddGuide(_T("sum @0 @3 8100"));  
		AddGuide(_T("sum @1 8100 @3"));  
		AddGuide(_T("sum @10 0 @9"));  
		AddGuide(_T("prod @13 1 16")); 
		AddGuide(_T("prod @13 1 8"));  
		AddGuide(_T("prod @13 3 16"));
		AddGuide(_T("prod @13 5 16"));
		AddGuide(_T("prod @13 7 16"));
		AddGuide(_T("prod @13 9 16"));
		AddGuide(_T("prod @13 11 16"));  
		AddGuide(_T("prod @13 3 4"));  
		AddGuide(_T("prod @13 13 16"));  
		AddGuide(_T("prod @13 7 8"));  
		AddGuide(_T("sum @9 @14 0"));  
		AddGuide(_T("sum @9 @16 0")); 
		AddGuide(_T("sum @9 @17 0"));  
		AddGuide(_T("sum @9 @21 0"));  
		AddGuide(_T("sum @11 @15 0"));  
		AddGuide(_T("sum @11 @18 0"));  
		AddGuide(_T("sum @11 @19 0"));  
		AddGuide(_T("sum @11 @20 0"));  
		AddGuide(_T("sum @11 @22 0"));  
		AddGuide(_T("sum @11 @23 0"));  
		AddGuide(_T("sum @3 @5 0"));  
		AddGuide(_T("sum @4 @5 0"));  
		AddGuide(_T("sum @9 @5 0"));  
		AddGuide(_T("sum @10 @5 0"));  
		AddGuide(_T("sum @11 @5 0"));  
		AddGuide(_T("sum @12 @5 0"));  
		AddGuide(_T("sum @24 @5 0"));  
		AddGuide(_T("sum @25 @5 0"));  
		AddGuide(_T("sum @26 @5 0"));  
		AddGuide(_T("sum @27 @5 0"));  
		AddGuide(_T("sum @28 @5 0"));  
		AddGuide(_T("sum @29 @5 0"));  
		AddGuide(_T("sum @30 @5 0"));  
		AddGuide(_T("sum @31 @5 0"));  
		AddGuide(_T("sum @32 @5 0"));  
		AddGuide(_T("sum @33 @5 0"));
		
		m_arAdjustments.push_back(1350);
		LoadConnectorsList(_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0"));
		LoadTextRect(_T("@0,@0,@1,@2"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,topLeft");
		oHandle1.switchHandle = _T("true");
		oHandle1.xrange = _T("0,5400");
		m_arHandles.push_back(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};