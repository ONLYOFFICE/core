#pragma once
#include "../PPTShape.h"

// 19
class CArcType : public CPPTShape
{
public:
	CArcType()

    {
        m_bConcentricFill = true;
		m_eJoin = NSOfficeDrawing::lineJoinRound;
        m_strPath = _T("wr-21600,,21600,43200,,,21600,21600nfewr-21600,,21600,43200,,,21600,21600l,21600nsxe");

		AddGuide(_T("val #2"));
		AddGuide(_T("val #3"));
		AddGuide(_T("val #4"));

		m_arAdjustments.Add(-5898240);
		m_arAdjustments.Add(0);
		m_arAdjustments.Add(0);
		m_arAdjustments.Add((LONG)ShapeSize);
		m_arAdjustments.Add((LONG)ShapeSize);

		LoadConnectorsList(_T("0,0;21600,21600;0,21600"));

		CHandle_ oHandle1;
		oHandle1.position = _T("@2,#0");
		oHandle1.polar = _T("@0,@1");
		m_arHandles.Add(oHandle1);

		CHandle_ oHandle2;
		oHandle2.position = _T("@2,#1");
		oHandle2.polar = _T("@0,@1");
		m_arHandles.Add(oHandle2);
    }
};