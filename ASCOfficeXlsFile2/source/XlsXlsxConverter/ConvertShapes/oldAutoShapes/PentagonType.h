#pragma once
#include "../PPTShape.h"

// 56
class CPentagonType : public CPPTShape
{
public:
	CPentagonType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m10800,l,8259,4200,21600r13200,l21600,8259xe");
        LoadConnectorsList(_T("10800,0;0,8259;4200,21600;10800,21600;17400,21600;21600,8259"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("4200,5077,17400,21600"));
    }
};
