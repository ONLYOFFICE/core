#include "DrawParam.h"

namespace OFD
{
CDrawParam::CDrawParam(CXmlReader& oXmlReader)
	: IOFDElement(oXmlReader), m_oPenSettings(oXmlReader),
	  m_pStrokeColor(nullptr), m_pFillColor(nullptr)
{
	std::string sName;
	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		sName = oXmlReader.GetNameA();

		if ("ofd:FillColor" == sName)
		{
			if (nullptr != m_pFillColor)
				delete m_pFillColor;

			m_pFillColor = new CColor(oXmlReader);
		}
		else if ("ofd:StrokeColor" == sName)
		{
			if (nullptr != m_pStrokeColor)
				delete m_pStrokeColor;

			m_pStrokeColor = new CColor(oXmlReader);
		}
	}
}

CDrawParam::~CDrawParam()
{
	if (nullptr != m_pStrokeColor)
		delete m_pStrokeColor;

	if (nullptr != m_pFillColor)
		delete m_pFillColor;
}

bool CDrawParam::ApplyStrokeColor(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer || nullptr == m_pStrokeColor)
		return false;

	m_oPenSettings.Apply(pRenderer);

	pRenderer->put_PenColor(m_pStrokeColor->ToInt(pPublicRes));
	pRenderer->put_PenAlpha(m_pStrokeColor->GetAlpha());

	return true;
}

bool CDrawParam::ApplyFillColor(IRenderer* pRenderer, const CRes* pPublicRes) const
{
	if (nullptr == pRenderer || nullptr == m_pFillColor)
		return false;

	pRenderer->put_BrushColor1(m_pFillColor->ToInt(pPublicRes));
	pRenderer->put_BrushAlpha1(m_pFillColor->GetAlpha());

	return true;
}
}
