#include "CText.h"

#include "../CSvgParser.h"
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

	CTSpan::CTSpan(XmlUtils::CXmlNode& oNode, CTSpan* pParent, NSFonts::IFontManager* pFontManager)
	    : CSvgGraphicsObject(oNode, pParent), m_pFontManager(pFontManager)
	{
		m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());

		if (!m_oX.SetValue(oNode.GetAttribute(L"x"), 1, true))
		{
			TBounds oBounds = pParent->GetBounds();
			m_oX = pParent->m_oX + oBounds.m_dRight - oBounds.m_dLeft;
		}

		if (!m_oY.SetValue(oNode.GetAttribute(L"y"), 1, true))
			m_oY = pParent->m_oY;
	}

	CTSpan::~CTSpan()
	{}

	CTSpan *CTSpan::Create(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
	{
		CTSpan *pTSpanParent = dynamic_cast<CTSpan*>(pParent);

		if (NULL == pTSpanParent)
			return NULL;

		return new CTSpan(oNode, pTSpanParent, pFontManager);
	}

	void CTSpan::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);

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

	bool CTSpan::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_wsText.empty() || bIsClip)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dX = m_oX.ToDouble(NSCSS::Pixel, oBounds.m_dRight  - oBounds.m_dLeft);
		double dY = m_oY.ToDouble(NSCSS::Pixel, oBounds.m_dBottom - oBounds.m_dTop);

		StartPath(pRenderer, pDefs, bIsClip);

		ApplyFont(pRenderer, dX, dY);

		pRenderer->CommandDrawText(m_wsText, dX, dY, 0, 0);

		for (const CSvgGraphicsObject* pTSpan : m_arObjects)
			pTSpan->Draw(pRenderer, pDefs, bIsClip);

		EndPath(pRenderer,pDefs, bIsClip);

		return true;
	}

	bool CTSpan::AddObject(CSvgGraphicsObject *pObject)
	{
		if (NULL == dynamic_cast<CTSpan*>(pObject))
			return false;

		m_arObjects.push_back(pObject);

		return true;
	}

	CTSpan *CTSpan::Copy() const
	{
		return new CTSpan(*this);
	}

	void CTSpan::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
	}

	void CTSpan::ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const
	{
		std::wstring wsFontFamily = (!m_oFont.GetFamily().Empty()) ? m_oFont.GetFamily().ToWString() : DefaultFontFamily;
		double dFontSize = (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.;

		wsFontFamily = DefaultFontFamily;

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

	TBounds CTSpan::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + GetWidth();
		oBounds.m_dBottom = oBounds.m_dTop + ((!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.);

		if (!m_arObjects.empty())
		{
			TBounds oTempBounds;
			for (const CSvgGraphicsObject* pObject : m_arObjects)
			{
				oTempBounds = pObject->GetBounds();
				oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
				oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
				oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
				oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
			}
		}

		return oBounds;
	}

	double CTSpan::GetWidth() const
	{
		std::wstring wsName = (!m_oFont.GetFamily().Empty()) ? m_oFont.GetFamily().ToWString() : DefaultFontFamily;
		double dSize = (!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.;

		m_pFontManager->LoadFontByName(wsName, dSize, 0, 72, 72);

		m_pFontManager->LoadString1(m_wsText, 0, 0);
		TBBox oBox = m_pFontManager->MeasureString2();
		double dWidth = 25.4 / 72.0 * (oBox.fMaxX - oBox.fMinX);

		return dWidth;
	}

	void CTSpan::Normalize()
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

	CText::CText(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
	    : CTSpan(oNode, NULL, pFontManager)
	{
		m_pParent = pParent;

		m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());
	}

	CText *CText::Create(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
	{
		if (NULL != dynamic_cast<CText*>(pParent))
			return NULL;

		return new CText(oNode, pParent, pFontManager);
	}

	bool CText::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || NULL == pRenderer)
			return false;

		CTSpan::Draw(pRenderer, pDefs, bIsClip);

		for (const CSvgGraphicsObject* pTSpan : m_arObjects)
			pTSpan->Draw(pRenderer, pDefs, bIsClip);

		return true;
	}

	bool CText::AddObject(CSvgGraphicsObject *pObject)
	{
		CTSpan *pTSpan = dynamic_cast<CTSpan*>(pObject);
		if (NULL != pTSpan)
		{
			m_arObjects.push_back(pTSpan);
			return true;
		}
		return false;
	}
}
