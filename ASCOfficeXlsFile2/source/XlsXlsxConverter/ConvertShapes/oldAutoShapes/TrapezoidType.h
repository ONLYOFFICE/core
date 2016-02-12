#pragma once
#include "../PPTShape.h"

// 8
class CTrapezoidType : public CPPTShape
{
public:
	CTrapezoidType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l@0,21600@1,21600,21600,xe");

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

        m_arAdjustments.push_back(5400);

        LoadConnectorsList(_T("@3,10800;10800,21600;@2,10800;10800,0"));
        LoadTextRect(_T("1800,1800,19800,19800;4500,4500,17100,17100;7200,7200,14400,14400"));

        CHandle_ oHandle1;
        oHandle1.position = _T("#0,bottomRight");
        oHandle1.xrange = _T("0,10800");
        m_arHandles.push_back(oHandle1);
    }
};
