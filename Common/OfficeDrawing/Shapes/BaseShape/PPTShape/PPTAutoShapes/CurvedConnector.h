#pragma once
#include "../PPTShape.h"

// 38
class CCurvedConnectorType : public CPPTShape
{
public:
	CCurvedConnectorType()
    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinRound;
        
		m_strPathLimoX = _T("m0,0qx@0,10800qy21600,21600nfe");
		m_strPathLimoY = _T("m0,0qy@0,10800qx21600,21600nfe");

		m_strPath = m_strPathLimoX;

		AddGuide(_T("val #0"));
		m_arAdjustments.Add(10800);

		LoadConnectorsList(_T("0,0;21600,21600"));
    }
};