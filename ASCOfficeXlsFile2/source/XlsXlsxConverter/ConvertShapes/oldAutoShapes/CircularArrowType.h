#pragma once
#include "../PPTShape.h"

// 99
class CCircularArrowType : public CPPTShape
{
public:
	CCircularArrowType()
	{
		m_bConcentricFill = false;
		m_eJoin = NSOfficeDrawing::lineJoinMiter;
		m_strPath = _T("al10800,10800@8@8@4@6,10800,10800,10800,10800@9@7l@30@31@17@18@24@25@15@16@32@33xe"); 

		AddGuide(_T("val #1")); 
		AddGuide(_T("val #0")); 
		AddGuide(_T("sum #1 0 #0")); 
		AddGuide(_T("val 10800")); 
		AddGuide(_T("sum 0 0 #1")); 
		AddGuide(_T("sumangle @2 360 0")); 
		AddGuide(_T("if @2 @2 @5")); 
		AddGuide(_T("sum 0 0 @6")); 
		AddGuide(_T("val #2")); 
		AddGuide(_T("sum 0 0 #0"));
		AddGuide(_T("sum #2 0 2700")); 
		AddGuide(_T("cos @10 #1"));
		AddGuide(_T("sin @10 #1"));
		AddGuide(_T("cos 13500 #1")); 
		AddGuide(_T("sin 13500 #1"));
		AddGuide(_T("sum @11 10800 0")); 
		AddGuide(_T("sum @12 10800 0"));
		AddGuide(_T("sum @13 10800 0"));
		AddGuide(_T("sum @14 10800 0"));
		AddGuide(_T("prod #2 1 2 "));
		AddGuide(_T("sum @19 5400 0")); 
		AddGuide(_T("cos @20 #1")); 
		AddGuide(_T("sin @20 #1")); 
		AddGuide(_T("sum @21 10800 0"));
		AddGuide(_T("sum @12 @23 @22")); 
		AddGuide(_T("sum @22 @23 @11")); 
		AddGuide(_T("cos 10800 #1")); 
		AddGuide(_T("sin 10800 #1")); 
		AddGuide(_T("cos #2 #1"));
		AddGuide(_T("sin #2 #1"));
		AddGuide(_T("sum @26 10800 0")); 
		AddGuide(_T("sum @27 10800 0")); 
		AddGuide(_T("sum @28 10800 0")); 
		AddGuide(_T("sum @29 10800 0")); 
		AddGuide(_T("sum @19 5400 0 "));
		AddGuide(_T("cos @34 #0"));
		AddGuide(_T("sin @34 #0"));
		AddGuide(_T("mid #0 #1"));
		AddGuide(_T("sumangle @37 180 0"));
		AddGuide(_T("if @2 @37 @38")); 
		AddGuide(_T("cos 10800 @39"));
		AddGuide(_T("sin 10800 @39"));
		AddGuide(_T("cos #2 @39"));
		AddGuide(_T("sin #2 @39"));
		AddGuide(_T("sum @40 10800 0")); 
		AddGuide(_T("sum @41 10800 0"));
		AddGuide(_T("sum @42 10800 0"));
		AddGuide(_T("sum @43 10800 0"));
		AddGuide(_T("sum @35 10800 0"));
		AddGuide(_T("sum @36 10800 0"));

		m_arAdjustments.push_back(-11796480);
		m_arAdjustments.push_back(0);
		m_arAdjustments.push_back(5400);
		
		LoadConnectorsList(_T("@44,@45;@48,@49;@46,@47;@17,@18;@24,@25;@15,@16"));
		LoadTextRect(_T("3163,3163,18437,18437"));

		CHandle_ oHandle1;
		oHandle1.position = _T("@3,#0");
		oHandle1.polar = _T("10800,10800");
		m_arHandles.push_back(oHandle1);

		CHandle_ oHandle2;
		oHandle2.position = _T("#2,#1");
		oHandle2.polar = _T("10800,10800");
		oHandle2.radiusrange = _T("0,10800");
		m_arHandles.push_back(oHandle2);
	}
};


