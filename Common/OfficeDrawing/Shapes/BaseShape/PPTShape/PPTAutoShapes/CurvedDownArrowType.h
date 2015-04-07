#pragma once
#include "../PPTShape.h"

// 105
class CCurvedDownArrowType : public CPPTShape
{
public:
	CCurvedDownArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("wr,0@3@23,0@22@4,0@15,0@1@23@7,0@13@2l@14@2@8@22@12@2at,0@3@23@11@2@17@26@15,0@1@23@17@26@15@22xewr,0@3@23@4,0@17@26nfe");

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
        AddGuide(_T("sum @12 width #0 "));
        AddGuide(_T("sum @5 0 #0"));
        AddGuide(_T("prod @15 1 2")); 
        AddGuide(_T("mid @4 @7"));
        AddGuide(_T("sum #0 #1 width")); 
        AddGuide(_T("prod @18 1 2"));
        AddGuide(_T("sum @17 0 @19"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));
        AddGuide(_T("prod height 2 1")); 
        AddGuide(_T("sum @17 0 @4 "));
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

        m_arAdjustments.Add(12960);
		m_arAdjustments.Add(19440);
		m_arAdjustments.Add(14400);
        
		LoadConnectorsList(_T("@17,0;@16,@22;@12,@2;@8,@22;@14,@2"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("@45,@47,@46,@48"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,bottomRight");
        oHandle1.xrange = _T("@40,@29");
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle1.position = _T("#1,bottomRight");
        oHandle1.xrange = _T("@27,@21");
        m_arHandles.Add(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("bottomRight,#2");
        oHandle3.yrange = _T("@44,@22");
        m_arHandles.Add(oHandle3);
    }
};
