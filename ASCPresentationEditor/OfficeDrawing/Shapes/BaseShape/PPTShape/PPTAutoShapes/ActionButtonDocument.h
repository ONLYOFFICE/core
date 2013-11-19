#pragma once
#include "../PPTShape.h"

// 198
class CActionButtonDocType : public CPPTShape
{
public:
	CActionButtonDocType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        
		m_strPath = _T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0em@12@9nfl@12@10@13@10@13@14@15@9xem@15@9nfl@15@14@13@14e");

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
		AddGuide(_T("prod #0 3 4")); 
		AddGuide(_T("sum @3 @11 6075"));  
		AddGuide(_T("sum @3 6075 @11"));  
		AddGuide(_T("sum @4 @5 4050"));  
		AddGuide(_T("sum @13 @5 4050"));  
		AddGuide(_T("sum @9 @5 0")); 
		AddGuide(_T("sum @10 @5 0"));  
		AddGuide(_T("sum @12 @5 0"));  
		AddGuide(_T("sum @13 @5 0"));  
		AddGuide(_T("sum @14 @5 0"));  
		AddGuide(_T("sum @15 @5 0"));
		
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