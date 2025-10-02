#include "GraphicUnit.h"
#include "../../../OOXML/Base/Unit.h"

namespace OFD
{
CGraphicUnit::CGraphicUnit(CXmlReader& oLiteReader)
	: m_bVisible(true), m_unDrawParam(0), m_oPenSettings(oLiteReader)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::wstring wsAttributeName;

	do
	{
		wsAttributeName = oLiteReader.GetName();

		if (L"Boundary" == wsAttributeName)
			m_oBoundary.Read(oLiteReader.GetTextA());
		else if (L"Name" == wsAttributeName)
			m_wsName = oLiteReader.GetText();
		else if (L"Visible" == wsAttributeName)
			m_bVisible = oLiteReader.GetBoolean(true);
		else if (L"CTM" == wsAttributeName)
			m_oCTM.Read(oLiteReader.GetTextA());
		else if (L"DrawParam" == wsAttributeName)
			m_unDrawParam = oLiteReader.GetUInteger(true);
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

void CGraphicUnit::Apply(IRenderer* pRenderer, TMatrix& oOldTransform) const
{
	if (nullptr == pRenderer)
		return;

	m_oPenSettings.Apply(pRenderer);

	pRenderer->GetTransform(&oOldTransform.m_dM11, &oOldTransform.m_dM12, &oOldTransform.m_dM21, &oOldTransform.m_dM22, &oOldTransform.m_dDx, &oOldTransform.m_dDy);

	Aggplus::CMatrix oTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
	const Aggplus::CMatrix oCurrentTransform(m_oCTM.m_dM11, m_oCTM.m_dM12, m_oCTM.m_dM21, m_oCTM.m_dM22, m_oBoundary.m_dX + m_oCTM.m_dDx, m_oBoundary.m_dY + m_oCTM.m_dDy);

	oTransform.Multiply(&oCurrentTransform);

	// Clipping
	pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);

	pRenderer->BeginCommand(c_nResetClipType);
	pRenderer->EndCommand(c_nResetClipType);

	if (!m_oBoundary.Empty())
	{
		pRenderer->BeginCommand(c_nClipType);
		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(m_oBoundary.m_dX, m_oBoundary.m_dY);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX + m_oBoundary.m_dWidth, m_oBoundary.m_dY);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX + m_oBoundary.m_dWidth, m_oBoundary.m_dY + m_oBoundary.m_dHeight);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX, m_oBoundary.m_dY + m_oBoundary.m_dHeight);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX, m_oBoundary.m_dY);

		pRenderer->EndCommand(c_nPathType);
		pRenderer->EndCommand(c_nClipType);
		pRenderer->PathCommandEnd();
	}

	pRenderer->SetTransform(oTransform.sx(), oTransform.shy(), oTransform.shx(), oTransform.sy(), oTransform.tx(), oTransform.ty());
}

TBox CGraphicUnit::GetBoundary() const
{
	return m_oBoundary;
}

}
