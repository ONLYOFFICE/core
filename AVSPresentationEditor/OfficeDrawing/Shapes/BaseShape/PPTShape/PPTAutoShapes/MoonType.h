#pragma once
#include "../PPTShape.h"

// 184
class CMoonType : public CPPTShape
{
public:
	CMoonType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,qx,10800,21600,21600wa@0@10@6@11,21600,21600,21600,xe");

		AddGuide(_T("val #0"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("prod #0 #0 @1"));
        AddGuide(_T("prod 21600 21600 @1"));
        AddGuide(_T("prod @3 2 1"));
        AddGuide(_T("sum @4 0 @2"));
        AddGuide(_T("sum @5 0 #0"));
        AddGuide(_T("prod @5 1 2"));
        AddGuide(_T("sum @7 0 #0"));
        AddGuide(_T("prod @8 1 2"));
        AddGuide(_T("sum 10800 0 @9"));
        AddGuide(_T("sum @9 10800 0"));
        AddGuide(_T("prod #0 9598 32768"));
        AddGuide(_T(" sum 21600 0 @12"));
        AddGuide(_T("ellipse @13 21600 10800"));
        AddGuide(_T("sum 10800 0 @14"));
        AddGuide(_T("sum @14 10800 0"));

		m_arAdjustments.Add(10800);

		LoadConnectorsList(_T("21600,0;0,10800;21600,21600;@0,10800"));

		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("@12,@15,@0,@16"));

		CHandle_ oHandle1;
        oHandle1.position = _T("#0,center");
        oHandle1.xrange = _T("0,18900");
        m_arHandles.Add(oHandle1);
    }
};
