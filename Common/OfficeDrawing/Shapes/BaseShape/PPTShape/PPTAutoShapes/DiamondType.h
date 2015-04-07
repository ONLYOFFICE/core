#pragma once
#include "../PPTShape.h"

// 4
class CDiamondType : public CPPTShape
{
public:
	CDiamondType()

    {
        m_bConcentricFill = true;

        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10800,l,10800,10800,21600,21600,10800xe");

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("5400,5400,16200,16200"));

    }
};