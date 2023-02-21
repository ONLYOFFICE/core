#include "CStyle.h"

#include "CObjectBase.h"
#include "../SvgUtils.h"

#include "../../../../../Common/3dParty/html/css/src/CElement.h"

namespace SVG
{
	CSvgCalculator::CSvgCalculator()
	    : m_pInternal(new NSCSS::CCssCalculator_Private)
	{
		m_pInternal->SetDpi(96);
		m_pInternal->SetUnitMeasure(NSCSS::UnitMeasure::Pixel);
	}

	CSvgCalculator::~CSvgCalculator()
	{
		if (NULL != m_pInternal)
			delete m_pInternal;
	}

	void CSvgCalculator::AddStyles(const std::wstring &wsStyles)
	{
		m_pInternal->AddStyles(wsStyles);
	}

	void CSvgCalculator::Clear()
	{
		m_pInternal->Clear();
	}
}
