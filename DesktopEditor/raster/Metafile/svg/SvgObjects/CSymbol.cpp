#include "CSymbol.h"

namespace SVG
{
	CSymbol::CSymbol(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager)
		: CGraphicsContainer(oNode)
	{}

	bool CSymbol::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer)
			return false;

		double dScaleX = 1, dScaleY = 1;
		double dTraslateX = m_oWindow.m_oX.ToDouble(NSCSS::Pixel), dTranslateY = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);

		if (!m_oWindow.m_oWidth.Empty() && !m_oViewBox.m_oWidth.Empty())
			dScaleX = m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel);

		if (!m_oWindow.m_oHeight.Empty() && !m_oViewBox.m_oHeight.Empty())
			dScaleY = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		double dM11, dM12, dM21, dM22, dDx, dDy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oMatrix(dM11, dM12, dM21, dM22, dDx, dDy);

		oMatrix.Translate(dTraslateX, dTranslateY);
		oMatrix.Scale(dScaleX, dScaleY);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());

		CGraphicsContainer::Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);

		return true;
	}
}

