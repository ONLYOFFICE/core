#pragma once
#include "../PPTShape.h"

// 9
class CHexagonType : public CPPTShape
{
public:
	CHexagonType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,l,10800@0,21600@1,21600,21600,10800@1,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("prod @0 2929 10000"));
        AddGuide(_T("sum width 0 @3"));
        AddGuide(_T("sum height 0 @3"));

        m_arAdjustments.push_back(5400);
        
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("1800,1800,19800,19800;3600,3600,18000,18000;6300,6300,15300,15300"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);

    }
};
