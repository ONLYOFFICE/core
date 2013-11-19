#pragma once
#include "../PPTShape.h"

// 59
class CSeal16Type : public CPPTShape
{
public:
	CSeal16Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800l@5@10,20777,6667@7@12,18436,3163@8@11,14932,822@6@9,10800,0@10@9,6667,822@12@11,3163,3163@11@12,822,6667@9@10,,10800@9@6,822,14932@11@8,3163,18436@12@7,6667,20777@10@5,10800,21600@6@5,14932,20777@8@7,18436,18436@7@8,20777,14932@5@6xe");

        AddGuide(_T("sum 10800 0 #0")); 
        AddGuide(_T("prod @0 32138 32768")); 
        AddGuide(_T("prod @0 6393 32768")); 
        AddGuide(_T("prod @0 27246 32768")); 
        AddGuide(_T("prod @0 18205 32768")); 
        AddGuide(_T("sum @1 10800 0")); 
        AddGuide(_T("sum @2 10800 0")); 
        AddGuide(_T("sum @3 10800 0")); 
        AddGuide(_T("sum @4 10800 0")); 
        AddGuide(_T("sum 10800 0 @1")); 
        AddGuide(_T("sum 10800 0 @2")); 
        AddGuide(_T("sum 10800 0 @3")); 
        AddGuide(_T("sum 10800 0 @4")); 
        AddGuide(_T("prod @0 23170 32768")); 
        AddGuide(_T("sum @13 10800 0")); 
        AddGuide(_T("sum 10800 0 @13"));

        m_arAdjustments.Add(2700);

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("@15,@15,@14,@14"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.Add(oHandle1);
    }
};


