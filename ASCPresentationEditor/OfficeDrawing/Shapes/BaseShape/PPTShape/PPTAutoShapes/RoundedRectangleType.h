#pragma once
#include "../PPTShape.h"

// 2
class CRoundedRectangleType : public CPPTShape
{
public:
	CRoundedRectangleType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m0@0qy@0,0l@1,0qx21600@0l21600@1qy@1,21600l@0,21600qx0@1xe");

        AddGuide(_T("val #0"));
		AddGuide(_T("sum 21600 0 #0"));

		m_arAdjustments.Add(5400);
        
        LoadTextRect(_T("0,0,21600,21600"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.Add(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};