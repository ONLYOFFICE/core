#pragma once
#include "../PPTShape.h"

// 133
class CFlowChartMagneticDrumType : public CPPTShape
{
public:
	CFlowChartMagneticDrumType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m21600,10800qy18019,21600l3581,21600qx,10800,3581,l18019,qx21600,10800xem18019,21600nfqx14438,10800,18019,e");
        LoadConnectorsList(_T("10800,0;0,10800;10800,21600;14438,10800;21600,10800"));
        
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("3581,0,14438,21600"));
    }
};


