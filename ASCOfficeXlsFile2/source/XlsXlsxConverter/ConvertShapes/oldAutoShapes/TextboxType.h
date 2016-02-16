#pragma once
#include "../CustomShape.h"

/////////   !!!!!!!!!!!!!!!!!!!!  /////////////
// 202
class CTextboxType : public CCustomShape
{
public:
	CTextboxType()

    {
        m_bConcentricFill = true;
        m_eJoin = NSOfficeDrawing::lineJoinMiter;
        m_strPath = _T("m,l,21600r21600,l21600,xe");
    }
};
