#pragma once
#include "../PPTShape.h"

// 176
class CFlowChartAlternateProcessType : public CPPTShape
{
public:
	CFlowChartAlternateProcessType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,qx0@0l0@2qy@0,21600l@1,21600qx21600@2l21600@0qy@1,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("sum height 0 #0"));
        AddGuide(_T("prod @0 2929 10000"));
        AddGuide(_T("sum width 0 @3"));
        AddGuide(_T("sum height 0 @3"));
        AddGuide(_T("val width"));
        AddGuide(_T("val height"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("prod height 1 2"));

		m_arAdjustments.Add(2700);
        
        LoadConnectorsList(_T("@8,0;0,@9;@8,@7;@6,@9"));
        LoadTextRect(_T("@3,@3,@4,@5"));

        m_lLimoX = 10800;
		m_lLimoY = 10800;
    }
};
