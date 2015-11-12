#pragma once
#include "../PPTShape.h"

// 64
class CWaveType : public CPPTShape
{
public:
	CWaveType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("m@28@0c@27@1@26@3@25@0l@21@4c@22@5@23@6@24@4xe");

		AddGuide(_T("val #0"));  
		AddGuide(_T("prod @0 41 9"));
		AddGuide(_T("prod @0 23 9"));  
		AddGuide(_T("sum 0 0 @2"));  
		AddGuide(_T("sum 21600 0 #0"));  
		AddGuide(_T("sum 21600 0 @1"));  
		AddGuide(_T("sum 21600 0 @3"));  
		AddGuide(_T("sum #1 0 10800"));  
		AddGuide(_T("sum 21600 0 #1"));  
		AddGuide(_T("prod @8 2 3"));  
		AddGuide(_T("prod @8 4 3"));  
		AddGuide(_T("prod @8 2 1"));  
		AddGuide(_T("sum 21600 0 @9"));  
		AddGuide(_T("sum 21600 0 @10"));  
		AddGuide(_T("sum 21600 0 @11"));
		AddGuide(_T("prod #1 2 3"));  
		AddGuide(_T("prod #1 4 3"));  
		AddGuide(_T("prod #1 2 1"));  
		AddGuide(_T("sum 21600 0 @15"));
		AddGuide(_T("sum 21600 0 @16"));  
		AddGuide(_T("sum 21600 0 @17"));
		AddGuide(_T("if @7 @14 0"));  
		AddGuide(_T("if @7 @13 @15"));  
		AddGuide(_T("if @7 @12 @16"));  
		AddGuide(_T("if @7 21600 @17"));  
		AddGuide(_T("if @7 0 @20"));  
		AddGuide(_T("if @7 @9 @19"));  
		AddGuide(_T("if @7 @10 @18")); 
		AddGuide(_T("if @7 @11 21600"));  
		AddGuide(_T("sum @24 0 @21"));  
		AddGuide(_T("sum @4 0 @0"));  
		AddGuide(_T("max @21 @25"));  
		AddGuide(_T("min @24 @28"));  
		AddGuide(_T("prod @0 2 1"));  
		AddGuide(_T("sum 21600 0 @33"));  
		AddGuide(_T("mid @26 @27"));  
		AddGuide(_T("mid @24 @28"));  
		AddGuide(_T("mid @22 @23"));  
		AddGuide(_T("mid @21 @25"));

        m_arAdjustments.push_back(2809);
		m_arAdjustments.push_back(10800);

        LoadConnectorsList(_T("@35,@0;@38,10800;@37,@4;@36,10800"));
        LoadTextRect(_T("@31,@33,@32,@34"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		CHandle_ oHandle1;
        oHandle1.position = _T("topLeft,#0");
        oHandle1.yrange = _T("0,4459");
        m_arHandles.push_back(oHandle1);

		CHandle_ oHandle2;
        oHandle2.position = _T("bottomRight,#1");
        oHandle2.xrange = _T("8640,12960");
        m_arHandles.push_back(oHandle2);
    }
};