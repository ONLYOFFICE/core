#include "CText.h"

#include "../SvgUtils.h"
#include "CContainer.h"
#include "CStyle.h"

#ifndef MININT8
#define MAXUINT8    ((unsigned char)~((unsigned char)0))
#define MAXINT8     ((char)(MAXUINT8 >> 1))
#define MININT8     ((char)~MAXINT8)
#endif

namespace SVG
{
#define DefaultFontFamily L"Times New Roman"

	CText::CText(CObjectBase *pParent, NSFonts::IFontManager* pFontManager)
	    : CObjectBase(pParent), m_pFontManager(pFontManager)/*, m_oCoord({0, 0})*/
	{}

	CText::~CText()
	{
		for (CTspan* pTspan : m_arChildrens)
			delete pTspan;
	}

	void CText::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		//FONT
		if (mAttributes.end() != mAttributes.find(L"font"))
			m_oFont.SetValue(mAttributes.at(L"font"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-size"))
			m_oFont.SetSize(mAttributes.at(L"font-size"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-size-adjust"))
			m_oFont.SetSize(mAttributes.at(L"font-size-adjust"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-stretch"))
			m_oFont.SetStretch(mAttributes.at(L"font-stretch"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-style"))
			m_oFont.SetStyle(mAttributes.at(L"font-style"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-variant"))
			m_oFont.SetVariant(mAttributes.at(L"font-variant"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-weight"))
			m_oFont.SetWeight(mAttributes.at(L"font-weight"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"font-family"))
			m_oFont.SetFamily(mAttributes.at(L"font-family"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"line-height"))
			m_oFont.SetLineHeight(mAttributes.at(L"line-height"), ushLevel, bHardMode);

		//TEXT
		if (mAttributes.end() != mAttributes.find(L"text-anchor"))
			m_oText.SetAlign(mAttributes.at(L"text-anchor"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"text-decoration"))
			m_oText.SetDecoration(mAttributes.at(L"text-decoration"), ushLevel, bHardMode);

		Normalize();
	}

	bool CText::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid() || NULL != dynamic_cast<CText*>(m_pParent))
			return false;

		m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());

		SaveNodeData(oNode);

		XmlUtils::CXmlNodes arChilds;

		oNode.GetChilds(arChilds);

		for (unsigned int unIndex = 0; unIndex < arChilds.GetCount(); ++unIndex)
		{
			XmlUtils::CXmlNode oChild;
			arChilds.GetAt(unIndex, oChild);

			if (L"tspan" == oChild.GetName())
			{
				CTspan *pTSpan = new CTspan(this);

				if (pTSpan->ReadFromXmlNode(oChild))
					m_arChildrens.push_back(pTSpan);
			}
		}

		return true;
	}

	bool CText::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		if (NULL == pRenderer || m_wsText.empty())
			return false;

		double dParentWidth = 0, dParentHeight = 0;
		CContainer *pContainer = dynamic_cast<CContainer*>(m_pParent);

		if (NULL != pContainer)
		{
			dParentWidth  = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
			dParentHeight = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
		}

		double dX = m_oX.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY = m_oY.ToDouble(NSCSS::Pixel, dParentHeight);

		Aggplus::CMatrix oOldMatrix(1., 0., .0, 1., 0., 0.);

		int nPlug = 0;

		ApplyStyle(pRenderer,pDefs, nPlug, oOldMatrix);

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyFont(pRenderer, dX, dY);

		pRenderer->CommandDrawText(m_wsText, dX, dY, 0, 0);

//		for (CTspan* pTspan : m_arChildrens)
//			pTspan->Draw(pRenderer, pBaseStyle);

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());

		return true;
	}

	void CText::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
	}

	void CText::ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const
	{
		std::wstring wsFontFamily = (!m_oFont.GetFamily().Empty()) ? m_oFont.GetFamily().ToWString() : DefaultFontFamily;
		double dFontSize = (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.;

		pRenderer->put_FontName(wsFontFamily);
		pRenderer->put_FontSize(dFontSize);

		int nStyle = 0;

		if (m_oFont.GetWeight().ToWString() == L"bold")
			nStyle |= 0x01;
		if (m_oFont.GetStyle() .ToWString() == L"italic")
			nStyle |= 0x02;
		if (m_oText.Underline())
			nStyle |= (1 << 2);

		// Вычиления размеров текста
		m_pFontManager->LoadFontByName(wsFontFamily, dFontSize, nStyle, 96., 96.);
		m_pFontManager->SetCharSpacing(0);

		double dKoef     = 25.4 / 96.;
		double dFHeight  = dFontSize;

		NSFonts::IFontFile* pFontFile = m_pFontManager->GetFile();

		if (pFontFile)
			dFHeight  *= pFontFile->GetHeight() / pFontFile->Units_Per_Em() * dKoef;

		float fW, fUndX1, fUndY1, fUndX2, fUndY2, fUndSize;

		m_pFontManager->LoadString1(m_wsText, 0, 0);
		TBBox oBox = m_pFontManager->MeasureString2();
		fW = (float)dKoef * (oBox.fMaxX - oBox.fMinX);

		// Просчитаем положение подчеркивания
		m_pFontManager->GetUnderline(&fUndX1, &fUndY1, &fUndX2, &fUndY2, &fUndSize);
		fUndX1   *= (float)dKoef;
		fUndY1   *= (float)dKoef;
		fUndX2   *= (float)dKoef;
		fUndY2   *= (float)dKoef;
		fUndSize *= (float)dKoef / 2;

		fUndY1 -= dFHeight / 4;
		fUndY2 -= dFHeight / 4;

		if (L"left" == m_oText.GetAlign().ToWString())
			dX += -fW;
		else if (L"center" == m_oText.GetAlign().ToWString())
			dX += -fW / 2;

		if (m_oText.Underline() || m_oText.LineThrough() || m_oText.Overline())
		{
			pRenderer->put_PenSize((double)fUndSize);
			pRenderer->put_PenLineEndCap(0);
			pRenderer->put_PenLineStartCap(0);

			pRenderer->BeginCommand(c_nPathType);
			pRenderer->PathCommandStart();

			if (m_oText.Underline())
			{
				pRenderer->PathCommandMoveTo(fUndX1 + dX, fUndY1 + dY);
				pRenderer->PathCommandLineTo(fUndX2 + dX, fUndY2 + dY);
			}

			if (m_oText.LineThrough())
			{
				pRenderer->PathCommandMoveTo(fUndX1 + dX, fUndY1 + dY - dFHeight / 2);
				pRenderer->PathCommandLineTo(fUndX2 + dX, fUndY2 + dY - dFHeight / 2);
			}

			if (m_oText.Overline())
			{
				pRenderer->PathCommandMoveTo(fUndX1 + dX, fUndY1 + dY - dFHeight * 1.5);
				pRenderer->PathCommandLineTo(fUndX2 + dX, fUndY2 + dY - dFHeight * 1.5);
			}

			pRenderer->DrawPath(c_nStroke);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd();
		}

		pRenderer->put_FontStyle(nStyle);
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(m_oFill.ToInt());
		pRenderer->put_BrushAlpha1(255);
	}

	TBounds CText::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + GetWidth();
		oBounds.m_dBottom = oBounds.m_dTop + (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.;

		return oBounds;
	}

	double CText::GetWidth() const
	{
		std::wstring wsName = (!m_oFont.GetFamily().Empty()) ? m_oFont.GetFamily().ToWString() : DefaultFontFamily;
		double dSize = (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.;

		m_pFontManager->LoadFontByName(wsName, dSize, 0, 72, 72);

		m_pFontManager->LoadString1(m_wsText, 0, 0);
		TBBox oBox = m_pFontManager->MeasureString2();
		double dWidth = 25.4 / 72.0 * (oBox.fMaxX - oBox.fMinX);

		for (const CTspan* pTspan : m_arChildrens)
			dWidth += pTspan->GetWidth();

		return dWidth;
	}

	void CText::Normalize()
	{
		Aggplus::CMatrix oMatrix = m_oTransform.GetMatrix().GetValue();

		double dM11 = oMatrix.sx();
		double dM22 = oMatrix.sy();

		if (dM11 < 0.05 || dM11 > 100)
		{
			m_oX *= dM11;
			dM11 /= std::abs(dM11);
		}

		if (dM22 < 0.05 || dM22 > 100)
		{
			m_oY *= dM22;
			m_oFont.UpdateSize(m_oFont.GetSize().ToDouble(NSCSS::Pixel) * dM22);
			dM22 /= std::abs(dM22);
		}

		oMatrix.SetElements(dM11, oMatrix.shy(), oMatrix.shx(), dM22, oMatrix.tx(), oMatrix.ty());

		m_oTransform.SetMatrix(oMatrix);
	}

	CTspan::CTspan(CObjectBase *pParent) : CObjectBase(pParent), m_oCoord({0, 0})
	{
		if (NULL != pParent)
		{
			CText* pText = dynamic_cast<CText*>(pParent);
			if (NULL != pText)
				InheritData(*pText);
		}
	}

	CTspan::~CTspan()
	{

	}

	void CTspan::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{

	}

	bool CTspan::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		double dX = oNode.GetAttributeDouble(L"x", MININT8);
		double dY = oNode.GetAttributeDouble(L"y", MININT8);

		SaveNodeData(oNode);

		if (dX != MININT8)
			m_oCoord.dX = dX;
		else if (NULL != m_pParent)
		{
			CText* pText = dynamic_cast<CText*>(m_pParent);
			if (NULL != pText)
				m_oCoord.dX += pText->GetWidth();
		}

		if (dY != MININT8)
			m_oCoord.dY = dY;

		dX = oNode.GetAttributeDouble(L"dx");
		dY = oNode.GetAttributeDouble(L"dy");

		m_oCoord.dX += dX;
		m_oCoord.dY += dY;

		m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());

		return true;
	}

	bool CTspan::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		if (NULL == pRenderer || m_wsText.empty())
			return false;

//		double dX = m_oCoord.dX, dY = m_oCoord.dY;

		pRenderer->ResetTransform();

//		Aggplus::CMatrix oMatrix = oStyle.GetTransform();
//		oMatrix.TransformPoint(dX, dY);

//		ApplyFont(pRenderer, oStyle, oMatrix.sx());

//		pRenderer->CommandDrawText(m_wsText, m_oCoord.dX, m_oCoord.dY, 0, 0);

		return true;
	}

	void CTspan::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
	}

	TBounds CTspan::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}

	void CTspan::InheritData(const CText &oText)
	{
		m_pFontManager = oText.m_pFontManager;
//		m_oCoord       = oText.m_oCoord;
	}

	double CTspan::GetWidth() const
	{
		m_pFontManager->LoadFontByName(L"Arial", 1, 0, 72, 72);
//		m_pFontManager->SetCharSpacing(0 * 72 / 25.4);

		m_pFontManager->LoadString1(m_wsText, 0, 0);
		TBBox oBox = m_pFontManager->MeasureString2();
		return (25.4 / 72.0 * (oBox.fMaxX - oBox.fMinX));
	}
}
