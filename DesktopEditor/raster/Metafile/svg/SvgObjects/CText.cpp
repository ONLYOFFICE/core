#include "CText.h"

#include "../CSvgParser.h"
#include "../SvgUtils.h"
#include "../CSvgFile.h"
#include "CContainer.h"
#include "CStyle.h"
#include "CPath.h"

#ifndef MININT8
#define MAXUINT8    ((unsigned char)~((unsigned char)0))
#define MAXINT8     ((char)(MAXUINT8 >> 1))
#define MININT8     ((char)~MAXINT8)
#endif

#define ISZERO(value) (std::abs(value) < 0.0000001)

namespace SVG
{
#define DefaultFontFamily L"Times New Roman"

	CTSpan::CTSpan(XmlUtils::CXmlNode& oNode, CTSpan* pParent, NSFonts::IFontManager* pFontManager)
	    : CSvgGraphicsObject(oNode, pParent), m_pFontManager(pFontManager)
	{
		m_oFont.UpdateSize(16);
		m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());

		if (!m_oX.SetValue(oNode.GetAttribute(L"x")) && NULL != dynamic_cast<CText*>(m_pParent))
		{
			TBounds oBounds = pParent->GetBounds();
			m_oX = pParent->m_oX + oBounds.m_dRight - oBounds.m_dLeft;
		}

		if (!m_oY.SetValue(oNode.GetAttribute(L"y")) && NULL != dynamic_cast<CText*>(m_pParent))
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

	CTSpan *CTSpan::Create(const std::wstring &wsValue, Point oPosition, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
	{
		CTSpan *pTSpanParent = dynamic_cast<CTSpan*>(pParent);

		if (NULL == pTSpanParent || wsValue.empty())
			return NULL;

		const std::wstring wsXmlNode = L"<tspan x=\"" + std::to_wstring(oPosition.dX) + L"\" y=\"" + std::to_wstring(oPosition.dY) + L"\">" + wsValue + L"</tspan>";

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlString(wsXmlNode);

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
	}

	bool CTSpan::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_wsText.empty() || bIsClip)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dX = m_oX.ToDouble(NSCSS::Pixel, oBounds.m_dRight  - oBounds.m_dLeft);
		double dY = m_oY.ToDouble(NSCSS::Pixel, oBounds.m_dBottom - oBounds.m_dTop);

		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);
		ApplyStyle(pRenderer, pDefs, nPathType, oOldMatrix);

		ApplyFont(pRenderer, dX, dY);

		pRenderer->CommandDrawText(m_wsText, dX, dY, 0, 0);

		for (const CSvgGraphicsObject* pTSpan : m_arObjects)
			pTSpan->Draw(pRenderer, pDefs, bIsClip);

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());

		return true;
	}

	bool CTSpan::AddObject(CSvgGraphicsObject *pObject)
	{
		if (NULL == dynamic_cast<CTSpan*>(pObject))
			return false;

		m_arObjects.push_back(pObject);

		return true;
	}

	void CTSpan::InheritStyles(const CTSpan *pTSpan)
	{
		if (NULL != pTSpan)
		{
			m_oFill       = pTSpan->m_oFill;
			m_oStroke     = pTSpan->m_oStroke;
			m_oTransform  = pTSpan->m_oTransform;
			m_oClip       = pTSpan->m_oClip;

			m_oFont = pTSpan->m_oFont;
			m_oText = pTSpan->m_oText;
		}
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
		std::wstring wsFontFamily = DefaultFontFamily;
		double dFontSize = m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4;

		Normalize(pRenderer, dX, dY, dFontSize);

		if (!m_oFont.GetFamily().Empty())
		{
			wsFontFamily = m_oFont.GetFamily().ToWString();

			std::vector<std::wstring> arCommonFonFamily({L"serif", L"sans-serif", L"monospace", L"cursive", L"fantasy", L"system-ui", L"emoji", L"math", L"fangsong", L"inherit", L"initial", L"unset"});

			if (arCommonFonFamily.end() != std::find(arCommonFonFamily.begin(), arCommonFonFamily.end(), wsFontFamily))
				wsFontFamily = L"Arial";
		}

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
		m_pFontManager->LoadFontByName(wsFontFamily, dFontSize, nStyle, 72., 72.);
		m_pFontManager->SetCharSpacing(0);

		double dKoef     = 25.4 / 72.;
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
		oBounds.m_dBottom = oBounds.m_dTop  + m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4;

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
		double dSize = m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4;

		m_pFontManager->LoadFontByName(wsName, dSize, 0, 72, 72);

		m_pFontManager->LoadString1(m_wsText, 0, 0);
		TBBox oBox = m_pFontManager->MeasureString2();
		double dWidth = 25.4 / 72.0 * (oBox.fMaxX - oBox.fMinX);

		return dWidth;
	}

	void CTSpan::Normalize(IRenderer *pRenderer, double &dX, double &dY, double &dFontHeight) const
	{
		if (NULL == pRenderer)
			return;

		double dM11, dM12, dM21, dM22, dDx, dDy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		double dXScale = 1., dYScale = 1.;

		double dModuleM11 = std::abs(dM11);
		double dModuleM22 = std::abs(dM22);

		if (!ISZERO(dModuleM11) && (dModuleM11 < 0.05 || dModuleM11 > 100))
			dXScale /= dModuleM11;

		if (!ISZERO(dModuleM22) && (dModuleM22 < 0.05 || dModuleM22 > 100))
			dYScale /= dModuleM22;

		dX          /= dXScale;
		dY          /= dYScale;
		dFontHeight /= dYScale;

		Aggplus::CMatrix oMatrix(dM11, dM12, dM21, dM22, dDx, dDy);

		oMatrix.Scale(dXScale, dYScale);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
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

	CTextPath::CTextPath(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager, const CSvgFile* pFile)
	    : CText(oNode, pParent, pFontManager), m_pPath(NULL)
	{
		if (NULL != pFile)
		{
			std::wstring wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href"));
			size_t unPosition = wsHref.find(L'#');

			if (std::wstring::npos != unPosition)
				wsHref.erase(0, unPosition + 1);

			m_pPath = dynamic_cast<const CPath*>(pFile->GetMarkedObject(wsHref));

			if (NULL != m_pPath)
			{
				IPathElement *pFirstElement = (*m_pPath)[0];
				if (NULL != pFirstElement)
				{
					Point oFirstPoint = (*pFirstElement)[0];
					m_oX = oFirstPoint.dX;
					m_oY = oFirstPoint.dY;
				}

				DevideByTSpan(m_wsText);
			}
		}
	}

	bool CTextPath::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || bIsClip || NULL == m_pPath)
			return false;

		for (const CSvgGraphicsObject* pTSpan : m_arObjects)
			pTSpan->Draw(pRenderer, pDefs, bIsClip);

		return true;
	}

	CTextPath* CTextPath::Create(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager, const CSvgFile* pFile)
	{
		if (NULL == dynamic_cast<CText*>(pParent))
			return NULL;

		return new CTextPath(oNode, pParent, pFontManager, pFile);
	}

	void CTextPath::DevideByTSpan(const std::wstring& wsText)
	{
		TBounds oBounds = GetBounds();

		double dXStep = (oBounds.m_dRight - oBounds.m_dLeft) / wsText.length();
		Point oPosition{m_oX.ToDouble(NSCSS::Pixel), m_oY.ToDouble(NSCSS::Pixel)};

		for (const wchar_t& oWChar : wsText)
		{
			AddObject(CTSpan::Create(std::wstring(1, oWChar), oPosition, this, m_pFontManager));
			oPosition.dX += dXStep;
		}
	}

}
