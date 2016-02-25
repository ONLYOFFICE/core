#pragma once
#include "../CustomShape.h"

// 177
namespace NSCustomShapesConvert
{
class CFlowChartOffpageConnectorType : public CCustomShape
{
public:
	CFlowChartOffpageConnectorType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l21600,r,17255l10800,21600,,17255xe"); 
        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("0,0,21600,17255"));
    }
};

}