#pragma once
#include "../PPTShape.h"

// 84
class CBevelType : public CPPTShape
{
public:
	CBevelType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l,21600r21600,l21600,xem@0@0nfl@0@2@1@2@1@0xem,nfl@0@0em,21600nfl@0@2em21600,21600nfl@1@2em21600,nfl@1@0e");
		
		AddGuide(_T("val #0"));
		AddGuide(_T("sum width 0 #0"));
		AddGuide(_T("sum height 0 #0"));
		AddGuide(_T("prod width 1 2"));
		AddGuide(_T("prod height 1 2"));
		AddGuide(_T("prod #0 1 2"));
		AddGuide(_T("prod #0 3 2"));
		AddGuide(_T("sum @1 @5 0"));
		AddGuide(_T("sum @2 @5 0"));

		m_arAdjustments.push_back(2700);

		LoadConnectorsList(_T("0,@4;@0,@4;@3,21600;@3,@2;21600,@4;@1,@4;@3,0;@3,@0"));
		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		LoadTextRect(_T("@0,@0,@1,@2"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,topLeft");
		oHandle1.switchHandle = _T("true");
		oHandle1.xrange = _T("0,10800");
		m_arHandles.push_back(oHandle1);

		m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};