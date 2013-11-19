#pragma once
#include "../PPTShape.h"

// 89
class CLeftUpArrowType : public CPPTShape
{
public:
	CLeftUpArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m@4,l@0@2@5@2@5@5@2@5@2@0,0@4@2,21600@2@1@1@1@1@2,21600@2xe");

        AddGuide(_T("val #0")); 
        AddGuide(_T("val #1"));  
        AddGuide(_T("val #2"));  
        AddGuide(_T("prod #0 1 2"));  
        AddGuide(_T("sum @3 10800 0"));  
        AddGuide(_T("sum 21600 #0 #1"));  
        AddGuide(_T("sum #1 #2 0"));  
        AddGuide(_T("prod @6 1 2"));  
        AddGuide(_T("prod #1 2 1"));  
        AddGuide(_T("sum @8 0 21600"));  
        AddGuide(_T("sum @5 0 @4"));  
        AddGuide(_T("sum #0 0 @4"));
        AddGuide(_T("prod @2 @10 @11")); 

        m_arAdjustments.Add(9257);
		m_arAdjustments.Add(18514);
		m_arAdjustments.Add(6171);

		LoadConnectorsList(_T("@4,0;@0,@2;@2,@0;0,@4;@2,21600;@7,@1;@1,@7;21600,@2"));

		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("@12,@5,@1,@1;@5,@12,@1,@1"));
       
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("@2,@9"); 
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle2.position = _T("#1,#2");
        oHandle2.xrange = _T("@4,21600");
        oHandle2.yrange = _T("0,@0"); 
        m_arHandles.Add(oHandle2);
    }
};
