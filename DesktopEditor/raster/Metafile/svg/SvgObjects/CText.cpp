#include "CText.h"

#include "../CSvgParser.h"
#include "../SvgUtils.h"
#include "../CSvgFile.h"
#include "CContainer.h"
#include "CFont.h"
#include "CStyle.h"

#ifndef MININT8
#define MAXUINT8    ((unsigned char)~((unsigned char)0))
#define MAXINT8     ((char)(MAXUINT8 >> 1))
#define MININT8     ((char)~MAXINT8)
#endif

#define ISZERO(value) (std::abs(value) < 0.0000001)

namespace SVG
{
	#define DEFAULT_TSPAN_FONT_SIZE 16
	#define DefaultFontFamily L"Times New Roman"
	#define MIN_FONT_SIZE 5
	#define MAX_FONT_SIZE 100
	#define MIN_SCALE     0.05
	#define MAX_SCALE     100

	CTSpan::CTSpan(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent, NSFonts::IFontManager* pFontManager, bool bCheckText)
		: CRenderedObject(oNode, pParent), m_pFontManager(pFontManager)
	{
		m_oFont.UpdateSize(DEFAULT_TSPAN_FONT_SIZE,DEFAULT_TSPAN_FONT_SIZE);

		// const int nDepth = oReader.GetDepth();
		// XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
		// while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
		// {
		// 	if (eNodeType == XmlUtils::XmlNodeType_Text ||
		// 	    eNodeType == XmlUtils::XmlNodeType_Whitespace ||
		// 	    eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE ||
		// 	    eNodeType == XmlUtils::XmlNodeType_CDATA)
		// 	{
		// 		const char* pValue = oReader.GetTextChar();
		// 		std::wstring wsValue;

		// 		if('\0' != pValue[0])
		// 		{
		// 			NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen(pValue), wsValue);
		// 			m_arP.push_back(new CParaText(L"____", wsValue, 0, nCharShapeID));
		// 		}
		// 	}
		// 	else if (eNodeType == XmlUtils::XmlNodeType_Element)
		// 	{
		// 		const std::string sChildNodeName{oReader.GetName()};

		// 		if (GetNodeName(ENode::LineBreak, eType) == sChildNodeName)
		// 			m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::LINE_BREAK));
		// 		else if (GetNodeName(ENode::Tab, eType) == sChildNodeName)
		// 			m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::TABULATION));
		// 		else if (GetNodeName(ENode::Hyphen, eType) == sChildNodeName)
		// 			m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_HYPHEN));
		// 		else if (GetNodeName(ENode::NbSpace, eType) == sChildNodeName ||
		// 		         GetNodeName(ENode::FwSpace, eType) == sChildNodeName)
		// 			m_arP.push_back(new CCtrlCharacter(L"   _", ECtrlCharType::HARD_SPACE));
		// 	}
		// }


		if (bCheckText)
			m_wsText = StrUtils::TrimExtraEnding(oNode.GetText());
		else
			m_wsText = oNode.GetText();

		m_oX.SetValue(oNode.GetAttribute(L"x"));
		m_oY.SetValue(oNode.GetAttribute(L"y"));

		if (NULL != pParent)
		{
			TBounds oBounds = pParent->GetBounds();

			CTSpan *pTSpan = dynamic_cast<CTSpan*>(pParent);

			if (NULL == pTSpan)
				return;

			m_oStyles = pTSpan->m_oStyles;

			if (m_oX.Empty())
			{
				if (!pTSpan->m_arObjects.empty())
					pTSpan = pTSpan->m_arObjects.back();

				m_oX = pTSpan->m_oX.ToDouble(NSCSS::Pixel, oBounds.m_dRight - oBounds.m_dLeft) + pTSpan->GetWidth();
			}

			if (m_oY.Empty())
				m_oY = pTSpan->m_oY;
		}
	}

	CTSpan::CTSpan(const std::wstring &wsText, const Point &oPosition, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager, bool bCheckText)
		: CRenderedObject(NSCSS::CNode(L"tspan", L"", L""), pParent), m_pFontManager(pFontManager), m_wsText(wsText)
	{
		m_oFont.UpdateSize(DEFAULT_TSPAN_FONT_SIZE, DEFAULT_TSPAN_FONT_SIZE);

		if (bCheckText)
			m_wsText = StrUtils::TrimExtraEnding(m_wsText);

		m_oX = oPosition.dX;
		m_oY = oPosition.dY;
	}

	CTSpan::~CTSpan()
	{}

	CTSpan *CTSpan::Create(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager)
	{
		CTSpan *pTSpanParent = dynamic_cast<CTSpan*>(pParent);

		if (NULL == pTSpanParent)
			return NULL;

		return new CTSpan(oNode, pTSpanParent, pFontManager);
	}

	CTSpan *CTSpan::Create(const std::wstring &wsValue, const Point& oPosition, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager, bool bCheckText)
	{
		CTSpan *pTSpanParent = dynamic_cast<CTSpan*>(pParent);

		if (NULL == pTSpanParent || wsValue.empty())
			return NULL;

		return new CTSpan(wsValue, oPosition, pParent, pFontManager, bCheckText);
	}

	void CTSpan::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);

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

		//POSITION
		if (mAttributes.end() != mAttributes.find(L"rotate"))
		{
			double dX, dY;
			CalculatePosition(dX, dY);
			m_oTransformation.m_oTransform.RotateAt(NSCSS::NS_STATIC_FUNCTIONS::ReadDouble(mAttributes.at(L"rotate")), dX, dY);
		}
	}

	bool CTSpan::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || (m_wsText.empty() && m_arObjects.empty()))
			return false;

		Aggplus::CMatrix oOldMatrix;

		if (!StartPath(pRenderer, pFile, oOldMatrix, oMode))
			return false;

		double dX, dY;
		CalculatePosition(dX, dY);

		if (!UseExternalFont(pFile, dX, dY, pRenderer, oMode, pOtherStyles, pContexObject))
		{
			if (!m_wsText.empty())
			{
				ApplyFont(pRenderer, dX, dY);
				pRenderer->CommandDrawText(m_wsText, dX, dY, 0, 0);
			}
		}

		for (const CTSpan* pTSpan : m_arObjects)
			pTSpan->Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		EndPath(pRenderer, pFile, oOldMatrix, oMode, pOtherStyles, pContexObject);

		return true;
	}

	bool CTSpan::AddObject(CTSpan *pObject)
	{
		if (NULL == pObject)
			return false;

		m_arObjects.push_back(pObject);

		return true;
	}

	void CTSpan::InheritStyles(const CTSpan *pTSpan)
	{
		if (NULL != pTSpan)
		{
			m_oStyles = pTSpan->m_oStyles;

			m_oFont = pTSpan->m_oFont;
			m_oText = pTSpan->m_oText;
		}
	}

	void CTSpan::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, true, pContexObject))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true, pContexObject))
			nTypePath += c_nWindingFillMode;
	}

	void CTSpan::ApplyFont(IRenderer* pRenderer, double& dX, double& dY) const
	{
		std::wstring wsFontFamily = DefaultFontFamily;
		double dFontSize = m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4;

		Normalize(pRenderer, dX, dY, dFontSize);

		if (!m_oFont.GetFamily().Empty())
		{
			wsFontFamily = m_oFont.GetFamily().ToWString();

			CorrectFontFamily(wsFontFamily);
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
			pRenderer->put_PenColor(m_oStyles.m_oStroke.m_oColor.ToInt());
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
		pRenderer->put_BrushColor1(m_oStyles.m_oFill.ToInt());
		pRenderer->put_BrushAlpha1(255);
	}

	bool CTSpan::UseExternalFont(const CSvgFile *pFile, double dX, double dY, IRenderer *pRenderer, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		std::wstring wsFontFamily = DefaultFontFamily;

		if (!m_oFont.GetFamily().Empty())
		{
			wsFontFamily = m_oFont.GetFamily().ToWString();
			CorrectFontFamily(wsFontFamily);
		}

		CFont *pFont = pFile->GetFont(wsFontFamily);

		if (NULL == pFont)
			return false;

		TSvgStyles oStyle;

		if (NULL != pOtherStyles)
			oStyle = *pOtherStyles;

		oStyle += m_oStyles;

		pFont->Draw(m_wsText, dX, dY, m_oFont.GetSize().ToDouble(NSCSS::Pixel), pRenderer, pFile, oMode, &oStyle, pContexObject);

		return true;
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
			for (const CRenderedObject* pObject : m_arObjects)
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
		if (m_wsText.empty() && m_arObjects.empty())
			return 0.;

		std::wstring wsName = DefaultFontFamily;
		double dSize = m_oFont.GetSize().ToDouble(NSCSS::Pixel);

		if (!m_oFont.GetFamily().Empty())
		{
			wsName = m_oFont.GetFamily().ToWString();

			CorrectFontFamily(wsName);
		}

		int nStyle = 0;

		if (m_oFont.GetWeight().ToWString() == L"bold")
			nStyle |= 0x01;
		if (m_oFont.GetStyle() .ToWString() == L"italic")
			nStyle |= 0x02;
		if (m_oText.Underline())
			nStyle |= (1 << 2);

		m_pFontManager->LoadFontByName(wsName, dSize, nStyle, 72., 72.);

		m_pFontManager->LoadString1(m_wsText, 0., 0.);
		TBBox oBox = m_pFontManager->MeasureString2();
		double dWidth = oBox.fMaxX - oBox.fMinX;

		for (const CTSpan* oTSpan : m_arObjects)
			dWidth += oTSpan->GetWidth();

		return dWidth;
	}

	void CTSpan::CorrectFontFamily(std::wstring &wsFontFamily) const
	{
		//TODO:: необходимо более подробно заняться подбором шрифтов
		if (L"sans-serif" == wsFontFamily)
			wsFontFamily = L"Arial";
		else if (L"serif" == wsFontFamily)
			wsFontFamily = L"Times New Roman";
		else if (L"monospace" == wsFontFamily)
			wsFontFamily = L"Consolas";
		else
		{
			std::vector<std::wstring> arCommonFonFamily({L"cursive", L"fantasy", L"system-ui", L"emoji", L"math", L"fangsong", L"inherit", L"initial", L"unset"});

			if (arCommonFonFamily.end() != std::find(arCommonFonFamily.begin(), arCommonFonFamily.end(), wsFontFamily))
				wsFontFamily = DefaultFontFamily;
		}
	}

	void CTSpan::CalculatePosition(double &dX, double &dY) const
	{
		TBounds oBounds{(NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.}};

		dX = m_oX.ToDouble(NSCSS::Pixel, oBounds.m_dRight - oBounds.m_dLeft);
		dY = m_oY.ToDouble(NSCSS::Pixel, oBounds.m_dBottom - oBounds.m_dTop);
	}

	void CTSpan::Normalize(IRenderer *pRenderer, double &dX, double &dY, double &dFontHeight) const
	{
		if (NULL == pRenderer)
			return;

		Aggplus::CMatrix oCurrentMatrix(m_oTransformation.m_oTransform.GetMatrix().GetFinalValue());

		double dXScale = 1., dYScale = 1.;

		const double dModuleM11 = std::abs(oCurrentMatrix.sx());
		const double dModuleM22 = std::abs(oCurrentMatrix.sy());

		if (!ISZERO(dModuleM11) && (dModuleM11 < MIN_SCALE || dModuleM11 > MAX_SCALE))
			dXScale /= dModuleM11;

		if (!ISZERO(dModuleM22) && (dModuleM22 < MIN_SCALE || dModuleM22 > MAX_SCALE))
			dYScale /= dModuleM22;

		dFontHeight *= dYScale;

		if (!Equals(0., dFontHeight) && dFontHeight < MIN_FONT_SIZE)
		{
			dXScale *= MIN_FONT_SIZE / dFontHeight;
			dYScale *= MIN_FONT_SIZE / dFontHeight;
			dFontHeight = MIN_FONT_SIZE;
		}
		else if (!Equals(0., dFontHeight) && dFontHeight > MAX_FONT_SIZE)
		{
			dXScale *= dFontHeight / MAX_FONT_SIZE;
			dYScale *= dFontHeight / MAX_FONT_SIZE;
			dFontHeight = MAX_FONT_SIZE;
		}

		if (Equals(1., dXScale) && Equals(1., dYScale))
			return;

		dX /= dXScale;
		dY /= dYScale;

		double dM11, dM12, dM21, dM22, dDx, dDy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oMatrix(dM11, dM12, dM21, dM22, dDx, dDy);

		oMatrix.Scale(dXScale, dYScale);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
	}

	void CTSpan::SetPosition(const Point &oPosition)
	{
		m_oX = oPosition.dX;
		m_oY = oPosition.dY;
	}

	std::vector<CTSpan> CTSpan::Split() const
	{
		std::vector<CTSpan> arGlyphs;
		arGlyphs.reserve(m_wsText.length());

		Point oPosition{m_oX.ToDouble(NSCSS::Pixel), m_oY.ToDouble(NSCSS::Pixel)};

		for (unsigned int unIndex = 0; unIndex < m_wsText.length(); ++unIndex)
		{
			arGlyphs.push_back(CTSpan(std::wstring(1, m_wsText[unIndex]), oPosition, m_pParent, m_pFontManager, false));
			oPosition.dX += arGlyphs[unIndex].GetWidth();
		}

		return arGlyphs;
	}

	CText::CText(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager)
	    : CTSpan(oNode, pParent, pFontManager)
	{}

	CText *CText::Create(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager)
	{
		CTSpan* pTSpan = dynamic_cast<CTSpan*>(pParent);

		if (NULL != pTSpan)
			return NULL;

		return new CText(oNode, pParent, pFontManager);
	}

	bool CText::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || NULL == pRenderer)
			return false;

		CTSpan::Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		return true;
	}

	CTextPath::CTextPath(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager, const CSvgFile* pFile)
	    : CText(oNode, pParent, pFontManager), m_pPath(NULL)
	{
		if (NULL != pFile)
		{
			std::wstring wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href"));
			size_t unPosition = wsHref.find(L'#');

			if (std::wstring::npos != unPosition)
				wsHref.erase(0, unPosition + 1);

			m_pPath = dynamic_cast<const CPath*>(pFile->GetMarkedObject(wsHref));
		}

		if (NULL != pParent)
		{
			CTSpan *pTSpan = dynamic_cast<CTSpan*>(pParent);

			if (NULL != pTSpan)
				m_oX = pTSpan->m_oX;
		}
	}

	bool CTextPath::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || CommandeModeClip == oMode || NULL == m_pPath)
			return false;

		CMovingPath oMovingPath(m_pPath);

		oMovingPath.Move(m_oX.ToDouble(NSCSS::Pixel));

		for (CTSpan& oTSpan : Split())
			DrawGlyph(&oTSpan, oMovingPath, pRenderer, pFile, oMode, pContexObject);

		for (const CTSpan* pTSpan : m_arObjects)
		{
			if (!pTSpan->m_oX.Empty())
			{
				oMovingPath.ToStart();
				oMovingPath.Move(pTSpan->m_oX.ToDouble(NSCSS::Pixel));
			}
			for (CTSpan& oGlyphs : pTSpan->Split())
				DrawGlyph(&oGlyphs, oMovingPath, pRenderer, pFile, oMode, pContexObject);
		}

		return true;

	}

	CTextPath* CTextPath::Create(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, NSFonts::IFontManager *pFontManager, const CSvgFile* pFile)
	{
		CTSpan *pTSpan = dynamic_cast<CText*>(pParent);

		if (NULL == pTSpan)
			return NULL;

		return new CTextPath(oNode, pTSpan, pFontManager, pFile);
	}

	void CTextPath::DrawGlyph(CTSpan* pTSpan, CMovingPath &oMovingPath, IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const CRenderedObject* pContexObject) const
	{
		if (NULL == pTSpan)
			return;

		pTSpan->InheritStyles(this);

		double dWidthSpan = pTSpan->GetWidth();

		if (!oMovingPath.Move(dWidthSpan / 2))
			return;

		double dAngle = oMovingPath.GetAngle();
		Point oPoint = oMovingPath.GetPosition() - Point{(dWidthSpan / 2.) * std::cos(dAngle / 180 * M_PI), (dWidthSpan / 2.) * std::sin(dAngle / 180 * M_PI)};

		pTSpan->SetPosition(oPoint);
		pTSpan->SetTransform({std::make_pair(L"transform", L"rotate(" + std::to_wstring(dAngle) + L',' + std::to_wstring(oPoint.dX) + L',' + std::to_wstring(oPoint.dY) + L')')}, 0, true);
		pTSpan->Draw(pRenderer, pFile, oMode, NULL, pContexObject);

		oMovingPath.Move(dWidthSpan / 2);
	}
}
