#pragma once
#include "../PPTShape.h"

// 46
class CAccentCallout3Type : public CPPTShape
{
public:
	CAccentCallout3Type()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m@0@1l@2@3@4@5@6@7nfem@6,l@6,21600nfem,l21600,r,21600l,21600nsxe");

		AddGuide(_T("val #0"));  
		AddGuide(_T("val #1"));
		AddGuide(_T("val #2"));  
		AddGuide(_T("val #3"));
		AddGuide(_T("val #4"));  
		AddGuide(_T("val #5"));
		AddGuide(_T("val #6"));  
		AddGuide(_T("val #7")); 

        m_arAdjustments.push_back(23400);
		m_arAdjustments.push_back(24400);
		m_arAdjustments.push_back(25200);
		m_arAdjustments.push_back(21600);
		m_arAdjustments.push_back(25200);
		m_arAdjustments.push_back(4050);
		m_arAdjustments.push_back(23400);
		m_arAdjustments.push_back(4050);

        LoadConnectorsList(_T("@0,@1;10800,0;10800,21600;0,10800;21600,10800"));

		CHandle_ oHandle1;
		oHandle1.position = _T("#0,#1");
        m_arHandles.push_back(oHandle1);

		CHandle_ oHandle2;
		oHandle2.position = _T("#2,#3");
        m_arHandles.push_back(oHandle2);

		CHandle_ oHandle3;
		oHandle3.position = _T("#4,#5");
        m_arHandles.push_back(oHandle3);

		CHandle_ oHandle4;
		oHandle4.position = _T("#6,#7");
        m_arHandles.push_back(oHandle4);
    }
};