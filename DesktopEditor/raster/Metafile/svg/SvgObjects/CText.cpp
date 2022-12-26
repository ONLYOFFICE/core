#include "CText.h"

#include "../SvgUtils.h"

#ifndef MININT8
#define MAXUINT8    ((unsigned char)~((unsigned char)0))
#define MAXINT8     ((char)(MAXUINT8 >> 1))
#define MININT8     ((char)~MAXINT8)
#endif


namespace SVG
{
	CText::CText(CObjectBase *pParent, CGeneralStyle* pBaseStyle, NSFonts::IFontManager* pFontManager)
	    : CObjectBase(pParent, pBaseStyle), m_pFontManager(pFontManager), m_oCoord({0, 0})
	{}

	CText::~CText()
	{
		for (CTspan* pTspan : m_arChildrens)
			delete pTspan;
	}

	bool CText::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		m_oCoord.dX = oNode.GetAttributeDouble(L"x");
		m_oCoord.dY = oNode.GetAttributeDouble(L"y");

		double dX = oNode.GetAttributeDouble(L"dx");
		double dY = oNode.GetAttributeDouble(L"dy");

		m_oCoord.dX += dX;
		m_oCoord.dY += dY;

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

	bool CText::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer || m_wsText.empty())
			return false;

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->CommandDrawText(m_wsText, m_oCoord.dX, m_oCoord.dY, 0, 0);

		for (CTspan* pTspan : m_arChildrens)
			pTspan->Draw(pRenderer);

		return true;
	}

	void CText::ApplyStyle(IRenderer *pRenderer, int& nTypePath)
	{
		//Временный данные для тестов
		pRenderer->put_FontName(L"Times New Roman");
		pRenderer->put_FontSize(12 / 25.4 * 96);

		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(0);
		pRenderer->put_BrushAlpha1(255);
	}

	double CText::GetWidth() const
	{
		m_pFontManager->LoadFontByName(L"Arial", 1, 0, 72, 72);
//		m_pFontManager->SetCharSpacing(0 * 72 / 25.4);

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

	bool CTspan::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		double dX = oNode.GetAttributeDouble(L"x", MININT8);
		double dY = oNode.GetAttributeDouble(L"y", MININT8);

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

	bool CTspan::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer || m_wsText.empty())
			return false;

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->CommandDrawText(m_wsText, m_oCoord.dX, m_oCoord.dY, 0, 0);

		return true;
	}

	void CTspan::ApplyStyle(IRenderer *pRenderer, int& nTypePath)
	{
		//Временный данные для тестов
		pRenderer->put_FontName(L"Arial");
		pRenderer->put_FontSize(1);

		pRenderer->put_BrushType(c_BrushTypeSolid);
		pRenderer->put_BrushColor1(0);
		pRenderer->put_BrushAlpha1(255);
	}

	void CTspan::InheritData(const CText &oText)
	{
		m_pFontManager = oText.m_pFontManager;
		m_oCoord       = oText.m_oCoord;
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
