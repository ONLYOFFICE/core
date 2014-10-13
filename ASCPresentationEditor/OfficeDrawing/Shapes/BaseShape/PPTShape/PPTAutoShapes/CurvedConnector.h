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
        
		//m_strPathLimoX = _T("m0,0qx@0,10800qy21600,21600nfe");
		//m_strPathLimoY = _T("m0,0qy@0,10800qx21600,21600nfe");

		m_strPath = _T("m,c@0,0@1,5400@1,10800@1,16200@2,21600,21600,21600e");//m_strPathLimoX;

		AddGuide(_T("mid #0 0"));
		AddGuide(_T("val #0"));
		AddGuide(_T("mid #0 21600"));

		//AddGuide(_T("val #0"));
		m_arAdjustments.push_back(10800);

		LoadConnectorsList(_T("0,0;21600,21600"));
    }
};