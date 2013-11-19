#pragma once
#include "../PPTShape.h"

// 72
class CIrregularSealTwo : public CPPTShape
{
public:
	CIrregularSealTwo()

    {
        m_bConcentricFill = false;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;

        m_strPath = _T("m11462,4342l9722,1887,8550,6382,4502,3625r870,4192l1172,8270r2763,3322l,12877r3330,2493l1285,17825r3520,415l4917,21600,7527,18125r1173,1587l9872,17370r1740,1472l12180,15935r2762,1435l14640,14350r4237,1282l16380,12310r1890,-1020l16985,9402,21600,6645,16380,6532,18007,3172,14525,5777,14790,xe");

        LoadConnectorsList(_T("9722,1887;0,12877;11612,18842;21600,6645"));

        m_arConnectorAngles.Add(270);
		m_arConnectorAngles.Add(180);
		m_arConnectorAngles.Add(90);
		m_arConnectorAngles.Add(0);

        LoadTextRect(_T("5372,6382,14640,15935"));

    }
};
