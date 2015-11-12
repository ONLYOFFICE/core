#pragma once
#include "../PPTShape.h"

// 91
class CBentArrowType : public CPPTShape
{
public:
	CBentArrowType()

    {
        m_bConcentricFill = false;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,6079l@0,0@0@1,12427@1qx,12158l,21600@4,21600@4,12158qy12427@2l@0@2@0,12158xe");
		
		AddGuide(_T("val #0"));
		AddGuide(_T("val #1"));
		AddGuide(_T("sum 12158 0 #1"));
		AddGuide(_T("sum @2 0 #1"));
		AddGuide(_T("prod @3 32768 32059"));
		AddGuide(_T("prod @4 1 2"));
		AddGuide(_T("sum 21600 0 #0"));
		AddGuide(_T("prod @6 #1 6079"));
		AddGuide(_T("sum @7 #0 0"));

		m_arAdjustments.push_back(18000);
		m_arAdjustments.push_back(3000);

		LoadConnectorsList(_T("@0,0;@0,12158;@5,21600;21600,6079"));
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		LoadTextRect(_T("12427,@1,@8,@2;0,12158,@4,21600"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,#1");
		oHandle1.xrange = _T("12427,21600");
		oHandle1.yrange = _T("0,6079");
		m_arHandles.push_back(oHandle1);
    }
};
