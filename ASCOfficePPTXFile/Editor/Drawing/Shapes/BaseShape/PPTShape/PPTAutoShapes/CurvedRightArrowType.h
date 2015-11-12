#pragma once
#include "../PPTShape.h"

// 102
class CCurvedRightArrowType : public CPPTShape
{
public:
	CCurvedRightArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("ar,0@23@3@22,,0@4,0@15@23@1,0@7@2@13l@2@14@22@8@2@12wa,0@23@3@2@11@26@17,0@15@23@1@26@17@22@15xear,0@23@3,0@4@26@17nfe");

        AddGuide(_T("val #0")); 
        AddGuide(_T("val #1")); 
        AddGuide(_T("val #2")); 
        AddGuide(_T("sum #0 width #1")); 
        AddGuide(_T("prod @3 1 2")); 
        AddGuide(_T("sum #1 #1 width")); 
        AddGuide(_T("sum @5 #1 #0")); 
        AddGuide(_T("prod @6 1 2")); 
        AddGuide(_T("mid width #0")); 
        AddGuide(_T("sum height 0 #2")); 
        AddGuide(_T("ellipse @9 height @4")); 
        AddGuide(_T("sum @4 @10 0")); 
        AddGuide(_T("sum @11 #1 width")); 
        AddGuide(_T("sum @7 @10 0")); 
        AddGuide(_T("sum @12 width #0")); 
        AddGuide(_T("sum @5 0 #0")); 
        AddGuide(_T("prod @15 1 2")); 
        AddGuide(_T("mid @4 @7")); 
        AddGuide(_T("sum #0 #1 width")); 
        AddGuide(_T("prod @18 1 2")); 
        AddGuide(_T("sum @17 0 @19")); 
        AddGuide(_T("val width")); 
        AddGuide(_T("val height")); 
        AddGuide(_T("prod height 2 1")); 
        AddGuide(_T("sum @17 0 @4")); 
        AddGuide(_T("ellipse @24 @4 height")); 
        AddGuide(_T("sum height 0 @25")); 
        AddGuide(_T("sum @8 128 0")); 
        AddGuide(_T("prod @5 1 2")); 
        AddGuide(_T("sum @5 0 128")); 
        AddGuide(_T("sum #0 @17 @12")); 
        AddGuide(_T("ellipse @20 @4 height")); 
        AddGuide(_T("sum width 0 #0")); 
        AddGuide(_T("prod @32 1 2")); 
        AddGuide(_T("prod height height 1")); 
        AddGuide(_T("prod @9 @9 1")); 
        AddGuide(_T("sum @34 0 @35")); 
        AddGuide(_T("sqrt @36")); 
        AddGuide(_T("sum @37 height 0")); 
        AddGuide(_T("prod width height @38")); 
        AddGuide(_T("sum @39 64 0")); 
        AddGuide(_T("prod #0 1 2")); 
        AddGuide(_T("ellipse @33 @41 height")); 
        AddGuide(_T("sum height 0 @42")); 
        AddGuide(_T("sum @43 64 0")); 
        AddGuide(_T("prod @4 1 2")); 
        AddGuide(_T("sum #1 0 @45")); 
        AddGuide(_T("prod height 4390 32768"));
        AddGuide(_T("prod height 28378 32768"));

		m_arAdjustments.push_back(12960);
		m_arAdjustments.push_back(19440);
		m_arAdjustments.push_back(14400);
        
        LoadConnectorsList(_T("0,@17;@2,@14;@22,@8;@2,@12;@22,@16"));
        
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);
		m_arConnectorAngles.push_back(0);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("@47,@45,@48,@46"));
       
        CHandle_ oHandle1;
        oHandle1.position = _T("bottomRight,#0");
        oHandle1.yrange = _T("@40,@29"); 
        m_arHandles.push_back(oHandle1);
        
        CHandle_ oHandle2;
        oHandle1.position = _T("bottomRight,#1"); 
        oHandle1.yrange = _T("@27,@21");  
        m_arHandles.push_back(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("#2,bottomRight");
        oHandle3.xrange = _T("@44,@22");
        m_arHandles.push_back(oHandle3);
    }
};
