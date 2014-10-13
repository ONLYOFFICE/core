#pragma once
#include "../PPTShape.h"

// 76
class CQuadArrowType : public CPPTShape
{
public:
	CQuadArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l@0@2@1@2@1@1@2@1@2@0,,10800@2@3@2@4@1@4@1@5@0@5,10800,21600@3@5@4@5@4@4@5@4@5@3,21600,10800@5@0@5@1@4@1@4@2@3@2xe");

		AddGuide(_T("val #0"));
        AddGuide(_T("val #1"));
        AddGuide(_T("val #2"));
        AddGuide(_T("sum 21600 0 #0"));
        AddGuide(_T("sum 21600 0 #1"));
        AddGuide(_T("sum 21600 0 #2"));
        AddGuide(_T("sum #0 0 10800"));
        AddGuide(_T("sum #1 0 10800"));
        AddGuide(_T("prod @7 #2 @6"));
        AddGuide(_T("sum 21600 0 @8"));

		m_arAdjustments.push_back(6480);
		m_arAdjustments.push_back(8640);
		m_arAdjustments.push_back(4320);

		LoadConnectorsList(_T("Rectangle"));

        LoadTextRect(_T("@8,@1,@9,@4;@1,@8,@4,@9"));
       
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("@2,@1");
        m_arHandles.push_back(oHandle1);
        
		CHandle_ oHandle2;
        oHandle2.position = _T("#1,#2");
        oHandle2.xrange = _T("@0,10800");
        oHandle2.yrange = _T("0,@0"); 
        m_arHandles.push_back(oHandle2);
    }
};
