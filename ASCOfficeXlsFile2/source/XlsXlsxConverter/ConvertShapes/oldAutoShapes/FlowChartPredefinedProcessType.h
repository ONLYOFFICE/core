#pragma once
#include "../CustomShape.h"

// 112
namespace NSCustomShapesConvert
{
class CFlowChartPredefinedProcessType : public CCustomShape
{
public:
	CFlowChartPredefinedProcessType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m,l,21600r21600,l21600,xem2610,nfl2610,21600em18990,nfl18990,21600e");

        LoadConnectorsList(_T("Rectangle"));
        LoadTextRect(_T("2610,0,18990,21600"));
    }
};
}