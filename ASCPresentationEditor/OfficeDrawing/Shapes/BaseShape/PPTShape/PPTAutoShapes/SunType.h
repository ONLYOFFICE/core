#pragma once
#include "../PPTShape.h"

// 183
class CSunType : public CPPTShape
{
public:
	CSunType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800l@15@14@15@18xem18436,3163l@17@12@16@13xem10800,l@14@10@18@10xem3163,3163l@12@13@13@12xem,10800l@10@18@10@14xem3163,18436l@13@16@12@17xem10800,21600l@18@15@14@15xem18436,18436l@16@17@17@16xem10800@19qx@19,10800,10800@20@20,10800,10800@19xe");

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
        AddGuide(_T("prod @5 3 4"));
        AddGuide(_T("prod @6 3 4"));
        AddGuide(_T("sum @10 791 0"));
        AddGuide(_T("sum @11 791 0"));
        AddGuide(_T("sum @11 2700 0"));
        AddGuide(_T("sum 21600 0 @10"));
        AddGuide(_T("sum 21600 0 @12"));
        AddGuide(_T("sum 21600 0 @13"));
        AddGuide(_T("sum 21600 0 @14"));
        AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #0"));
        
        m_arAdjustments.push_back(5400);

        LoadTextRect(_T("@9,@9,@8,@8"));

		CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("2700,10125");
        m_arHandles.push_back(oHandle1);
    }
};
