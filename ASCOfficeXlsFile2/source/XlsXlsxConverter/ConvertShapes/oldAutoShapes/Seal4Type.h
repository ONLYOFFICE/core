#pragma once
#include "../PPTShape.h"

// 187
class CSeal4Type : public CPPTShape
{
public:
	CSeal4Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800l@2@3,10800,0@3@3,,10800@3@2,10800,21600@2@2xe");

        AddGuide(_T("sum 10800 0 #0"));     
        AddGuide(_T("prod @0 23170 32768"));     
        AddGuide(_T("sum @1 10800 0"));     
        AddGuide(_T("sum 10800 0 @1"));

        m_arAdjustments.push_back(8100);

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("@3,@3,@2,@2"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);
    }
};


