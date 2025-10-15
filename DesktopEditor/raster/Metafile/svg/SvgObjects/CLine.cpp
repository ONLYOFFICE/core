#include "CLine.h"

namespace SVG
{
	CLine::CLine(CSvgReader& oReader, CRenderedObject* pParent)
		: CPath(oReader, pParent)
	{
		AddElement(new CMoveElement(Point{0, 0}));
		AddElement(new CLineElement(Point{0, 0}));
	}

	void CLine::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		//TODO:: не нравится, подумать как можно сделать иначе
		if ("x1" == sName)
		{
			CMoveElement* pMoveElement{dynamic_cast<CMoveElement*>(operator[](0))};

			if (NULL != pMoveElement)
				pMoveElement->m_oPoint.dX = oReader.GetDouble();
		}
		else if ("y1" == sName)
		{
			CMoveElement* pMoveElement{dynamic_cast<CMoveElement*>(operator[](0))};

			if (NULL != pMoveElement)
				pMoveElement->m_oPoint.dY = oReader.GetDouble();
		}
		else if ("x2" == sName)
		{
			CLineElement* pLineElement{dynamic_cast<CLineElement*>(operator[](1))};

			if (NULL != pLineElement)
				pLineElement->m_oPoint.dX = oReader.GetDouble();
		}
		else if ("y2" == sName)
		{
			CLineElement* pLineElement{dynamic_cast<CLineElement*>(operator[](1))};

			if (NULL != pLineElement)
				pLineElement->m_oPoint.dY = oReader.GetDouble();
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
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
