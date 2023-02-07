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

	bool CText::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer || m_wsText.empty())
			return false;

		double dM11, dM12, dM21, dM22, dRx, dRy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		double dParentWidth = 0, dParentHeight = 0;
		CContainer *pContainer = dynamic_cast<CContainer*>(m_pParent);

		if (NULL != pContainer)
		{
			dParentWidth  = pContainer->GetWidth().ToDouble(NSCSS::Pixel);
			dParentHeight = pContainer->GetHeight().ToDouble(NSCSS::Pixel);
		}

		double dX = m_oX.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY = m_oY.ToDouble(NSCSS::Pixel, dParentHeight);

		GetWidth();

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->CommandDrawText(m_wsText, dX, dY, 0, 0);

//		for (CTspan* pTspan : m_arChildrens)
//			pTspan->Draw(pRenderer, pBaseStyle);

		return true;
	}

	void CText::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		ApplyFont(pRenderer);
		ApplyTransform(pRenderer);
	}

	void CText::ApplyFont(IRenderer* pRenderer) const
	{
		pRenderer->put_FontName((!m_oFont.GetFamily().Empty()) ? m_oFont.GetFamily().ToWString() : DefaultFontFamily);
		pRenderer->put_FontSize((!m_oFont.GetSize().Empty()) ? m_oFont.GetSize().ToDouble(NSCSS::Pixel) * 72. / 25.4 : 18.);

		int nStyle = 0;

		if (m_oFont.GetWeight().ToWString() == L"bold")
			nStyle |= 0x01;
		if (m_oFont.GetStyle() .ToWString() == L"italic")
			nStyle |= 0x02;
//			if (pFont->IsUnderline())
//				nStyle |= (1 << 2);
//			if (pFont->IsStrikeOut())
//				nStyle |= (1 << 7);

		pRenderer->put_FontStyle(nStyle);
		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(m_oFill.ToInt());
		pRenderer->put_BrushAlpha1(255);
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

	bool CTspan::Draw(IRenderer *pRenderer) const
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

	void CTspan::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
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
