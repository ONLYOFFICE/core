#pragma once
#include "../PPTShape.h"

// 71
class CIrregularSealOneType : public CPPTShape
{
public:
	CIrregularSealOneType()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m10800,5800l8352,2295,7312,6320,370,2295,4627,7617,,8615r3722,3160l135,14587r5532,-650l4762,17617,7715,15627r770,5973l10532,14935r2715,4802l14020,14457r4125,3638l16837,12942r4763,348l17607,10475,21097,8137,16702,7315,18380,4457r-4225,868l14522,xe");

        LoadConnectorsList(_T("14522,0;0,8615;8485,21600;21600,13290"));
		
		m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("4627,6320,16702,13937"));
    }
};
