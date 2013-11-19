#pragma once
#include "../PPTShape.h"

//104
class CCurvedUpArrowType : public CPPTShape
{
public:
	CCurvedUpArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("ar0@22@3@21,,0@4@21@14@22@1@21@7@21@12@2l@13@2@8,0@11@2wa0@22@3@21@10@2@16@24@14@22@1@21@16@24@14,xewr@14@22@1@21@7@21@16@24nfe");

        AddGuide(_T("val #0"));  
        AddGuide(_T("val #1")); 
        AddGuide(_T("val #2")); 
        AddGuide(_T("sum #0 width #1")); 
        AddGuide(_T("prod @3 1 2")); 
        AddGuide(_T("sum #1 #1 width "));
        AddGuide(_T("sum @5 #1 #0")); 
        AddGuide(_T("prod @6 1 2")); 
        AddGuide(_T("mid width #0")); 
        AddGuide(_T("ellipse #2 height @4")); 
        AddGuide(_T("sum @4 @9 0 "));
        AddGuide(_T("sum @10 #1 width")); 
        AddGuide(_T("sum @7 @9 0 "));
        AddGuide(_T("sum @11 width #0 "));
        AddGuide(_T("sum @5 0 #0 "));
        AddGuide(_T("prod @14 1 2 "));
        AddGuide(_T("mid @4 @7 "));
        AddGuide(_T("sum #0 #1 width "));
        AddGuide(_T("prod @17 1 2 "));
        AddGuide(_T("sum @16 0 @18 "));
        AddGuide(_T("val width "));
        AddGuide(_T("val height "));
        AddGuide(_T("sum 0 0 height")); 
        AddGuide(_T("sum @16 0 @4 "));
        AddGuide(_T("ellipse @23 @4 height "));
        AddGuide(_T("sum @8 128 0 "));
        AddGuide(_T("prod @5 1 2 "));
        AddGuide(_T("sum @5 0 128 "));
        AddGuide(_T("sum #0 @16 @11 "));
        AddGuide(_T("sum width 0 #0 "));
        AddGuide(_T("prod @29 1 2 "));
        AddGuide(_T("prod height height 1 "));
        AddGuide(_T("prod #2 #2 1 "));
        AddGuide(_T("sum @31 0 @32 "));
        AddGuide(_T("sqrt @33 "));
        AddGuide(_T("sum @34 height 0 "));
        AddGuide(_T("prod width height @35")); 
        AddGuide(_T("sum @36 64 0 "));
        AddGuide(_T("prod #0 1 2 "));
        AddGuide(_T("ellipse @30 @38 height "));
        AddGuide(_T("sum @39 0 64 "));
        AddGuide(_T("prod @4 1 2"));
        AddGuide(_T("sum #1 0 @41 "));
        AddGuide(_T("prod height 4390 32768"));
        AddGuide(_T("prod height 28378 32768"));

        m_arAdjustments.Add(12960);
		m_arAdjustments.Add(19440);
		m_arAdjustments.Add(7200);

		LoadConnectorsList(_T("@8,0;@11,@2;@15,0;@16,@21;@13,@2"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("@41,@43,@42,@44"));
 
        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("@37,@27");
        m_arHandles.Add(oHandle1);

        CHandle_ oHandle2;
        oHandle1.position = _T("#1,topLeft");
        oHandle1.xrange = _T("@25,@20");
        m_arHandles.Add(oHandle2);

        CHandle_ oHandle3;
        oHandle3.position = _T("bottomRight,#2");
        oHandle3.yrange = _T("0,@40");
        m_arHandles.Add(oHandle3);
    }
};
