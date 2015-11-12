#pragma once
#include "../PPTShape.h"

// 92
class CSeal24Type : public CPPTShape
{
public:
	CSeal24Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800l@7@14,21232,8005@9@16,20153,5400@11@18,18437,3163@12@17,16200,1447@10@15,13595,368@8@13,10800,0@14@13,8005,368@16@15,5400,1447@18@17,3163,3163@17@18,1447,5400@15@16,368,8005@13@14,,10800@13@8,368,13595@15@10,1447,16200@17@12,3163,18437@18@11,5400,20153@16@9,8005,21232@14@7,10800,21600@8@7,13595,21232@10@9,16200,20153@12@11,18437,18437@11@12,20153,16200@9@10,21232,13595@7@8xe");

        AddGuide(_T("sum 10800 0 #0")); 
        AddGuide(_T("prod @0 32488 32768")); 
        AddGuide(_T("prod @0 4277 32768")); 
        AddGuide(_T("prod @0 30274 32768")); 
        AddGuide(_T("prod @0 12540 32768")); 
        AddGuide(_T("prod @0 25997 32768")); 
        AddGuide(_T("prod @0 19948 32768")); 
        AddGuide(_T("sum @1 10800 0")); 
        AddGuide(_T("sum @2 10800 0")); 
        AddGuide(_T("sum @3 10800 0")); 
        AddGuide(_T("sum @4 10800 0")); 
        AddGuide(_T("sum @5 10800 0")); 
        AddGuide(_T("sum @6 10800 0")); 
        AddGuide(_T("sum 10800 0 @1")); 
        AddGuide(_T("sum 10800 0 @2")); 
        AddGuide(_T("sum 10800 0 @3")); 
        AddGuide(_T("sum 10800 0 @4")); 
        AddGuide(_T("sum 10800 0 @5")); 
        AddGuide(_T("sum 10800 0 @6")); 
        AddGuide(_T("prod @0 23170 32768")); 
        AddGuide(_T("sum @19 10800 0"));
        AddGuide(_T("sum 10800 0 @19"));

		m_arAdjustments.push_back(2700);

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("@21,@21,@20,@20"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);
    }
};


