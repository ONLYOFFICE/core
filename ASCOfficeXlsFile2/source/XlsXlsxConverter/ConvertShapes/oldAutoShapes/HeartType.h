#pragma once
#include "../PPTShape.h"

// 74
class CHeartType : public CPPTShape
{
public:
	CHeartType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m10860,2187c10451,1746,9529,1018,9015,730,7865,152,6685,,5415,,4175,152,2995,575,1967,1305,1150,2187,575,3222,242,4220,,5410,242,6560,575,7597l10860,21600,20995,7597v485,-1037,605,-2187,485,-3377c21115,3222,20420,2187,19632,1305,18575,575,17425,152,16275,,15005,,13735,152,12705,730v-529,288,-1451,1016,-1845,1457xe");

        LoadConnectorsList(_T("10860,2187;2928,10800;10860,21600;18672,10800"));

		m_arConnectorAngles.push_back(270);
		m_arConnectorAngles.push_back(180);
		m_arConnectorAngles.push_back(90);
		m_arConnectorAngles.push_back(0);

        LoadTextRect(_T("5037,2277,16557,13677"));
    }
};





    
