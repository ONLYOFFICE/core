#pragma once
#include "../CustomShape.h"

// 32
namespace NSCustomShapesConvert
{

class CStraightConnectorType : public CCustomShape
{
public:
	CStraightConnectorType()

    {
        m_strPath = _T("m,l21600,21600e");
        LoadConnectorsList(_T("0,0;21600,21600"));
    }
};
}
