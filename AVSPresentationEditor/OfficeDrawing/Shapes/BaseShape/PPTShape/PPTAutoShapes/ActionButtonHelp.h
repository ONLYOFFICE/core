#pragma once
#include "../PPTShape.h"

// 191
class CActionButtonHelpType : public CPPTShape
{
public:
	CActionButtonHelpType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        
		m_strPath = _T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@33@27nfqy@3@9@40@27@39@4@37@29l@37@30@36@30@36@29qy@37@28@39@27@3@26@34@27xem@3@31nfqx@35@32@3@10@38@32@3@31xe");

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
		AddGuide(_T("prod @13 1 7"));
		AddGuide(_T("prod @13 3 14"));  
		AddGuide(_T("prod @13 2 7"));  
		AddGuide(_T("prod @13 5 14"));  
		AddGuide(_T("prod @13 11 28"));  
		AddGuide(_T("prod @13 3 7"));  
		AddGuide(_T("prod @13 4 7"));  
		AddGuide(_T("prod @13 17 28"));  
		AddGuide(_T("prod @13 9 14"));  
		AddGuide(_T("prod @13 21 28"));  
		AddGuide(_T("prod @13 11 14"));  
		AddGuide(_T("prod @13 25 28"));  
		AddGuide(_T("sum @9 @14 0"));  
		AddGuide(_T("sum @9 @16 0"));  
		AddGuide(_T("sum @9 @18 0"));  
		AddGuide(_T("sum @9 @21 0"));  
		AddGuide(_T("sum @9 @23 0"));  
		AddGuide(_T("sum @9 @24 0"));  
		AddGuide(_T("sum @9 @25 0"));  
		AddGuide(_T("sum @11 @15 0"));  
		AddGuide(_T("sum @11 @17 0")); 
		AddGuide(_T("sum @11 @18 0"));  
		AddGuide(_T("sum @11 @19 0"));  
		AddGuide(_T("sum @11 @20 0"));  
		AddGuide(_T("sum @11 @21 0"));  
		AddGuide(_T("sum @11 @22 0"));  
		AddGuide(_T("sum @11 @24 0"));  
		AddGuide(_T("sum @3 @5 0"));  
		AddGuide(_T("sum @4 @5 0"));  
		AddGuide(_T("sum @9 @5 0"));  
		AddGuide(_T("sum @10 @5 0"));  
		AddGuide(_T("sum @11 @5 0"));  
		AddGuide(_T("sum @12 @5 0"));  
		AddGuide(_T("sum @26 @5 0"));  
		AddGuide(_T("sum @27 @5 0"));  
		AddGuide(_T("sum @28 @5 0"));  
		AddGuide(_T("sum @29 @5 0"));  
		AddGuide(_T("sum @30 @5 0")); 
		AddGuide(_T("sum @31 @5 0"));
		AddGuide(_T("sum @32 @5 0"));
		AddGuide(_T("sum @33 @5 0")); 
		AddGuide(_T("sum @34 @5 0"));  
		AddGuide(_T("sum @35 @5 0"));  
		AddGuide(_T("sum @36 @5 0"));  
		AddGuide(_T("sum @37 @5 0"));  
		AddGuide(_T("sum @38 @5 0"));  
		AddGuide(_T("sum @39 @5 0"));  
		AddGuide(_T("sum @40 @5 0"));
		
		m_arAdjustments.Add(1350);
		LoadConnectorsList(_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0"));
		LoadTextRect(_T("@0,@0,@1,@2"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,topLeft");
		oHandle1.switchHandle = _T("true");
		oHandle1.xrange = _T("0,5400");
		m_arHandles.Add(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};