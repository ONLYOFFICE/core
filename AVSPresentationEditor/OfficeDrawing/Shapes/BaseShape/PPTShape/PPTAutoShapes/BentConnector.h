#pragma once
#include "../PPTShape.h"

// 34
class CBentConnectorType : public CPPTShape
{
public:
	CBentConnectorType()
    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinRound;
        
		//m_strPathLimoX = _T("m0,0l@0,0r0,21600l21600,21600nfe");
		//m_strPathLimoY = _T("m0,0l,@0r21600,0l21600,21600nfe");

		m_strPath = _T("m,l@0,0@0,21600,21600,21600e");//m_strPathLimoX;

		AddGuide(_T("val #0"));
		m_arAdjustments.Add(10800);

		LoadConnectorsList(_T("0,0;21600,21600"));
    }
};