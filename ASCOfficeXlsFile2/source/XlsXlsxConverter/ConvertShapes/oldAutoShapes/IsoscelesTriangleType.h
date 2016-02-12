#pragma once
#include "../PPTShape.h"

// 5
class CIsoscelesTriangleType : public CPPTShape
{
public:
	CIsoscelesTriangleType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,l,21600r21600,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("prod #0 1 2"));
        AddGuide(_T("sum @1 10800 0"));

        m_arAdjustments.push_back(10800);

        LoadConnectorsList(_T("@0,0;@1,10800;0,21600;10800,21600;21600,21600;@2,10800"));

        LoadTextRect(_T("0,10800,10800,18000;5400,10800,16200,18000;10800,10800,21600,18000;0,7200,7200,21600;7200,7200,14400,21600;14400,7200,21600,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,21600");
        m_arHandles.push_back(oHandle1);
    }
};