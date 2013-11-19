#pragma once
#include "../PPTShape.h"

// 58
class CSeal8Type : public CPPTShape
{
public:
	CSeal8Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800l@3@6,18436,3163@4@5,10800,0@6@5,3163,3163@5@6,,10800@5@4,3163,18436@6@3,10800,21600@4@3,18436,18436@3@4xe");

        AddGuide(_T("sum 10800 0 #0")); 
        AddGuide(_T("prod @0 30274 32768")); 
        AddGuide(_T("prod @0 12540 32768")); 
        AddGuide(_T("sum @1 10800 0")); 
        AddGuide(_T("sum @2 10800 0")); 
        AddGuide(_T("sum 10800 0 @1")); 
        AddGuide(_T("sum 10800 0 @2")); 
        AddGuide(_T("prod @0 23170 32768")); 
        AddGuide(_T("sum @7 10800 0"));
        AddGuide(_T("sum 10800 0 @7"));

		m_arAdjustments.Add(2538);

		LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("@9,@9,@8,@8"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.Add(oHandle1);
    }
};


