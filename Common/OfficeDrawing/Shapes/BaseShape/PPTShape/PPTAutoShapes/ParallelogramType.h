#pragma once
#include "../PPTShape.h"

// 7
class CParallelogramType : public CPPTShape
{
public:
	CParallelogramType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m@0,l,21600@1,21600,21600,xe");

        AddGuide(_T("val #0"));
        AddGuide(_T("sum width 0 #0"));
        AddGuide(_T("prod #0 1 2"));
        AddGuide(_T("sum width 0 @2"));
        AddGuide(_T("mid #0 width"));
        AddGuide(_T("mid @1 0"));
        AddGuide(_T("prod height width #0"));
        AddGuide(_T("prod @6 1 2"));
        AddGuide(_T("sum height 0 @7"));
        AddGuide(_T("prod width 1 2"));
        AddGuide(_T("sum #0 0 @9"));
        AddGuide(_T("if @10 @8 0"));
        AddGuide(_T("if @10 @7 height"));

		m_arAdjustments.Add(5400);
        
        LoadConnectorsList(_T("@4,0;10800,@11;@3,10800;@5,21600;10800,@12;@2,10800"));
        LoadTextRect(_T("1800,1800,19800,19800;8100,8100,13500,13500;10800,10800,10800,10800"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,topLeft");
        oHandle1.xrange = _T("0,21600");
        m_arHandles.Add(oHandle1);
	}
};
