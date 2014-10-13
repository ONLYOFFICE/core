#pragma once
#include "../PPTShape.h"

// 101
class CUturnArrowType : public CPPTShape
{
public:
	CUturnArrowType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m15662,14285l21600,8310r-2970,qy9250,,,8485l,21600r6110,l6110,8310qy8907,5842l9725,5842qx12520,8310l9725,8310xe");

        LoadConnectorsList(_T("9250,0;3055,21600;9725,8310;15662,14285;21600,8310"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("0,8310,6110,21600"));
    }
};