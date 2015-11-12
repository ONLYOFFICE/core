#pragma once
#include "../PPTShape.h"

// 57
class CNoSmokingType : public CPPTShape
{
public:
	CNoSmokingType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,10800qy10800,,21600,10800,10800,21600,,10800xar@0@0@16@16@12@14@15@13xar@0@0@16@16@13@15@14@12xe");

		AddGuide(_T("val #0"));
        AddGuide(_T("prod @0 2 1"));
        AddGuide(_T("sum 21600 0 @1"));
        AddGuide(_T("prod @2 @2 1"));
        AddGuide(_T("prod @0 @0 1"));
        AddGuide(_T("sum @3 0 @4"));
        AddGuide(_T("prod @5 1 8"));
        AddGuide(_T("sqrt @6"));
        AddGuide(_T("prod @4 1 8"));
        AddGuide(_T("sqrt @8"));
        AddGuide(_T("sum @7 @9 0"));
        AddGuide(_T("sum @7 0 @9"));
        AddGuide(_T("sum @10 10800 0"));
        AddGuide(_T("sum 10800 0 @10"));
        AddGuide(_T("sum @11 10800 0"));
        AddGuide(_T("sum 10800 0 @11"));
        AddGuide(_T("sum 21600 0 @0"));

		m_arAdjustments.push_back(2700);

        LoadConnectorsList(_T("10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163"));

        LoadTextRect(_T("3163,3163,18437,18437"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,7200");
        m_arHandles.push_back(oHandle1);
    }
};

