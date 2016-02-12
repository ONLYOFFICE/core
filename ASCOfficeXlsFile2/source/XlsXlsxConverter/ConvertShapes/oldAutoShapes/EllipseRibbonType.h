#pragma once
#include "../PPTShape.h"

// 107
class CEllipceRibbonType : public CPPTShape
{
public:
	CEllipceRibbonType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        //Encaps: Flat

        m_strPath = _T("ar@9@38@8@37,0@27@0@26@9@13@8@4@0@25@22@25@9@38@8@37@22@26@3@27l@7@40@3,wa@9@35@8@10@3,0@21@33@9@36@8@1@21@31@20@31@9@35@8@10@20@33,,l@5@40xewr@9@36@8@1@20@31@0@32nfl@20@33ear@9@36@8@1@21@31@22@32nfl@21@33em@0@26nfl@0@32em@22@26nfl@22@32e");

		AddGuide(_T("val #0"));
		AddGuide(_T("val #1"));  
		AddGuide(_T("val #2"));  
		AddGuide(_T("val width"));  
		AddGuide(_T("val height"));  
		AddGuide(_T("prod width 1 8"));  
		AddGuide(_T("prod width 1 2"));  
		AddGuide(_T("prod width 7 8"));  
		AddGuide(_T("prod width 3 2"));  
		AddGuide(_T("sum 0 0 @6"));  
		AddGuide(_T("sum height 0 #2"));  
		AddGuide(_T("prod @10 30573 4096"));  
		AddGuide(_T("prod @11 2 1"));  
		AddGuide(_T("sum height 0 @12"));  
		AddGuide(_T("sum @11 #2 0"));  
		AddGuide(_T("sum @11 height #1"));  
		AddGuide(_T("sum height 0 #1"));  
		AddGuide(_T("prod @16 1 2"));  
		AddGuide(_T("sum @11 @17 0"));  
		AddGuide(_T("sum @14 #1 height"));  
		AddGuide(_T("sum #0 @5 0"));  
		AddGuide(_T("sum width 0 @20"));  
		AddGuide(_T("sum width 0 #0"));  
		AddGuide(_T("sum @6 0 #0"));  
		AddGuide(_T("ellipse @23 width @11"));  
		AddGuide(_T("sum @24 height @11"));  
		AddGuide(_T("sum @25 @11 @19"));  
		AddGuide(_T("sum #2 @11 @19"));  
		AddGuide(_T("prod @11 2391 32768"));  
		AddGuide(_T("sum @6 0 @20"));  
		AddGuide(_T("ellipse @29 width @11"));  
		AddGuide(_T("sum #1 @30 @11"));  
		AddGuide(_T("sum @25 #1 height"));  
		AddGuide(_T("sum height @30 @14"));  
		AddGuide(_T("sum @11 @14 0"));  
		AddGuide(_T("sum height 0 @34"));  
		AddGuide(_T("sum @35 @19 @11"));  
		AddGuide(_T("sum @10 @15 @11")); 
		AddGuide(_T("sum @35 @15 @11"));  
		AddGuide(_T("sum @28 @14 @18"));  
		AddGuide(_T("sum height 0 @39"));  
		AddGuide(_T("sum @19 0 @18"));  
		AddGuide(_T("prod @41 2 3"));  
		AddGuide(_T("sum #1 0 @42"));  
		AddGuide(_T("sum #2 0 @42"));  
		AddGuide(_T("min @44 20925"));  
		AddGuide(_T("prod width 3 8"));  
		AddGuide(_T("sum @46 0 4"));

        m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(5400);
		m_arAdjustments.push_back(18900);

        LoadConnectorsList(_T("@6,0;@5,@36;@6,@1;@7,@36"));
        LoadTextRect(_T("@0,@22,@19,@1"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

		CHandle_ oHandle1;
        oHandle1.position = _T("#0,bottomRight");
        oHandle1.xrange = _T("@5,@47");
        m_arHandles.push_back(oHandle1);

		CHandle_ oHandle2;
        oHandle2.position = _T("center,#1");
        oHandle2.yrange = _T("@10,@43");
        m_arHandles.push_back(oHandle2);

		CHandle_ oHandle3;
        oHandle3.position = _T("topLeft,#2");
        oHandle3.yrange = _T("@27,@45");
        m_arHandles.push_back(oHandle3);
    }
};