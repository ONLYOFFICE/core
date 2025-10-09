#include "CLine.h"

namespace SVG
{
CLine::CLine(CSvgReader& oReader, CRenderedObject* pParent)
    : CPath(oReader, pParent, false)
	{	
		SvgDigit oX1;
		SvgDigit oY1;
		SvgDigit oX2;
		SvgDigit oY2;

		START_READ_ATTRIBUTES(oReader)
		{
			if ("x1" == sAttributeName)
				oX1.SetValue(oReader.GetText());
			else if ("y1" == sAttributeName)
				oY1.SetValue(oReader.GetText());
			else if ("x2" == sAttributeName)
				oX2.SetValue(oReader.GetText());
			else if ("y2" == sAttributeName)
				oY2.SetValue(oReader.GetText());
		}
		END_READ_ATTRIBUTES(oReader)

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
