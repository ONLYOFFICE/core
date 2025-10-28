#include "CStyle.h"
#include "CObjectBase.h"

namespace SVG
{
	CSvgCalculator::CSvgCalculator()
	    : m_pInternal(new NSCSS::CCssCalculator_Private)
	{
		m_pInternal->SetDpi(96);
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

	void CSvgCalculator::SetData(CObject *pSvgObject) const
	{
		if (NULL == pSvgObject)
			return;

		const std::vector<NSCSS::CNode> arSelectors = pSvgObject->GetFullPath();

		std::vector<std::wstring> arNodes = m_pInternal->CalculateAllNodes(arSelectors);
		std::vector<std::wstring> arPrevNodes;

		for (size_t i = 0; i < arSelectors.size(); ++i)
		{
			pSvgObject->SetData(arSelectors[i].m_mAttributes, i + 1);

			for (const NSCSS::CElement* oElement : m_pInternal->FindElements(arNodes, arPrevNodes))
				pSvgObject->SetData(oElement->GetStyle(), i + 1);

			if (!arSelectors[i].m_wsStyle.empty())
				pSvgObject->SetData(arSelectors[i].m_wsStyle, i + 1, true);
		}

		return;
	}
}
