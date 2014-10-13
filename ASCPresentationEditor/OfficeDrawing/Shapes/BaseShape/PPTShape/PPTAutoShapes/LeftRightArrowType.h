#pragma once
#include "../PPTShape.h"

// 69
class CLeftRightArrowType : public CPPTShape
{
public:
	CLeftRightArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,10800l@0,21600@0@3@2@3@2,21600,21600,10800@2,0@2@1@0@1@0,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("prod #0 #1 10800"));
        AddGuide(_T("sum #0 0 @4"));
        AddGuide(_T("sum 21600 0 @5"));

		m_arAdjustments.push_back(4320);
		m_arAdjustments.push_back(5400);

		LoadConnectorsList(_T("@2,0;10800,@1;@0,0;0,10800;@0,21600;10800,@3;@2,21600;21600,10800"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@5,@1,@6,@3"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,#1");
        oHandle1.xrange = _T("0,10800");
        oHandle1.yrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);

    }
};
