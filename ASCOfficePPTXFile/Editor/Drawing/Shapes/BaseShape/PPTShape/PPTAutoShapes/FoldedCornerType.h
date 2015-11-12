#pragma once
#include "../PPTShape.h"

// 65
class CFoldedCornerType : public CPPTShape
{
public:
	CFoldedCornerType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,l,21600@0,21600,21600@0,21600,xem@0,21600nfl@3@5c@7@9@11@13,21600@0e");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 @0"));
        AddGuide(_T("prod @1 8481 32768"));
        AddGuide(_T("sum @2 @0 0"));
        AddGuide(_T("prod @1 1117 32768"));
        AddGuide(_T("sum @4 @0 0"));
        AddGuide(_T("prod @1 11764 32768"));
        AddGuide(_T("sum @6 @0 0"));
        AddGuide(_T("prod @1 6144 32768"));
        AddGuide(_T("sum @8 @0 0"));
        AddGuide(_T("prod @1 20480 32768"));
        AddGuide(_T("sum @10 @0 0"));
        AddGuide(_T("prod @1 6144 32768"));
        AddGuide(_T("sum @12 @0 0"));

		m_arAdjustments.push_back(18900);

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("0,0,21600,@13"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,bottomRight");
        oHandle1.xrange = _T("10800,21600");
        m_arHandles.push_back(oHandle1);
        
    }
};