#pragma once
#include "../PPTShape.h"

// 32
class CStraightConnectorType : public CPPTShape
{
public:
	CStraightConnectorType()

    {
        m_strPath = _T("m,l21600,21600e");
        LoadConnectorsList(_T("0,0;21600,21600"));
    }
};
