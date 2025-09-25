#include "CLine.h"

namespace SVG
{
	CLine::CLine(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent)
		: CPath(oNode, pParent, false)
	{	
		SvgDigit oX1;
		SvgDigit oY1;
		SvgDigit oX2;
		SvgDigit oY2;

		oX1.SetValue(oNode.GetAttribute(L"x1"));
		oY1.SetValue(oNode.GetAttribute(L"y1"));
		oX2.SetValue(oNode.GetAttribute(L"x2"));
		oY2.SetValue(oNode.GetAttribute(L"y2"));

		AddElement(new CMoveElement(Point{oX1.ToDouble(NSCSS::Pixel), oY1.ToDouble(NSCSS::Pixel)}));
		AddElement(new CLineElement(Point{oX2.ToDouble(NSCSS::Pixel), oY2.ToDouble(NSCSS::Pixel)}));
	}

	void CLine::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetMarker(mAttributes, ushLevel, bHardMode);
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, true, pContexObject))
			nTypePath += c_nStroke;
	}
}
