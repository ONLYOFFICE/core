#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
		: CGraphicsContainer(oNode, pParent), CDefObject(oNode, pParent), m_pImage(NULL)
	{
		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"refX"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"refY"));

		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"markerWidth",  L"3"));
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"markerHeight", L"3"));

		const std::wstring wsViewBox = oNode.GetAttribute(L"viewBox");

		if (!wsViewBox.empty())
		{
			std::vector<double> arValues = StrUtils::ReadDoubleValues(wsViewBox);
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}

//		if (m_oWindow.m_oWidth.Empty() && !m_oViewBox.m_oWidth.Empty())
//			m_oWindow.m_oWidth = m_oViewBox.m_oWidth;
//		else if (!m_oWindow.m_oWidth.Empty() && m_oViewBox.m_oWidth.Empty())
//			m_oViewBox.m_oWidth = m_oWindow.m_oWidth;

//		if (m_oWindow.m_oHeight.Empty() && !m_oViewBox.m_oHeight.Empty())
//			m_oWindow.m_oHeight = m_oViewBox.m_oHeight;
//		else if (!m_oWindow.m_oHeight.Empty() && m_oViewBox.m_oHeight.Empty())
//			m_oViewBox.m_oHeight = m_oWindow.m_oHeight;

		const std::wstring& wsUnits = oNode.GetAttribute(L"markerUnits");

		if (L"strokeWidth" == wsUnits)
			m_enUnits = Marker_StrokeWidth;
		else if (L"userSpaceOnUse" == wsUnits)
			m_enUnits = Marker_UserSpaceOnUse;
	}

	CMarker::~CMarker()
	{
		if (NULL != m_pImage)
			delete m_pImage;
	}

	void CMarker::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	bool CMarker::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		return true;
	}

	void CMarker::Update(const CDefs *pDefs)
	{
		if (NULL != m_pImage || (!m_oWindow.m_oWidth.Empty() && m_oWindow.m_oWidth.Zero()) || (!m_oWindow.m_oHeight.Empty() && m_oWindow.m_oHeight.Zero()) ||
			(!m_oViewBox.m_oWidth.Empty() && m_oViewBox.m_oWidth.Zero()) || (!m_oViewBox.m_oHeight.Empty() && m_oViewBox.m_oHeight.Zero()))
			return;

		double dWidth  = m_oViewBox.m_oWidth .ToDouble(NSCSS::Pixel);
		double dHeight = m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		double dMMtoPx = 96. / 25.4;

		int nWidth  = dWidth  * dMMtoPx + 1;
		int nHeight = dHeight * dMMtoPx + 1;

		if (0 == nWidth || 0 == nHeight)
			return;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];

		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nHeight; i++)
			((unsigned int*)pBgraData)[i] = alfa;

		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		double dMaxValue = std::max(dWidth, dHeight);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->put_Width ((dWidth  + dMaxValue) / 2.);
		pGrRenderer->put_Height((dHeight + dMaxValue) / 2.);

		if (dWidth != dHeight)
		{
			double dSkipX = 0;
			double dSkipY = 0;

			if (dWidth > dHeight)
				dSkipX = (dWidth - dHeight) / 2.;
			else if (dHeight > dWidth)
				dSkipY = (dHeight - dWidth) / 2.;

			pGrRenderer->SetCoordTransformOffset(dSkipX, dSkipY);
		}
		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->BeginCommand(c_nImageType);

		for (const CSvgGraphicsObject* pObject : m_arObjects)
			pObject->Draw(pGrRenderer, pDefs);

		pGrRenderer->EndCommand(c_nImageType);
		RELEASEINTERFACE(pGrRenderer);

		oFrame.put_Data(NULL);

		m_pImage = new Aggplus::CImage;
		m_pImage->Create(pBgraData, oFrame.get_Width(), oFrame.get_Height(), oFrame.get_Stride());
	}

	void CMarker::Draw(IRenderer *pRenderer, const std::vector<Point> &arPoints) const
	{
		if (NULL == m_pImage || arPoints.empty())
			return;

		TBounds oBounds = GetBounds();

		for (const Point& oPoint : arPoints)
		{
			Point oNewPoint(oPoint);

			oNewPoint.dX -= m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
			oNewPoint.dY -= m_oWindow.m_oY.ToDouble(NSCSS::Pixel);

			pRenderer->DrawImage((IGrObject*)m_pImage, oNewPoint.dX + 1.75, oNewPoint.dY + 1.75, oBounds.m_dRight - oBounds.m_dLeft, 8.25);
		}
	}

	TBounds CMarker::GetBounds() const
	{
		TBounds oBounds;

		if (m_oViewBox.m_oWidth.Empty() && m_oViewBox.m_oHeight.Empty())
		{
			TBounds oTempBounds;
			for (const CSvgGraphicsObject* pObject : m_arObjects)
			{
				oTempBounds = pObject->GetBounds();
				oBounds.m_dLeft   = std::min(oBounds.m_dLeft,   oTempBounds.m_dLeft);
				oBounds.m_dTop    = std::min(oBounds.m_dTop,    oTempBounds.m_dTop);
				oBounds.m_dRight  = std::max(oBounds.m_dRight,  oTempBounds.m_dRight);
				oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
			}
		}
		else
		{
			double dWidth  = m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
			double dHeight = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

			double dMaxVbValue = std::max(dWidth, dHeight);

			oBounds.m_dLeft = 0;
			oBounds.m_dTop  = 0;

			oBounds.m_dRight  = oBounds.m_dLeft + dWidth;
			oBounds.m_dBottom = oBounds.m_dTop  + dHeight;
		}


		return oBounds;
	}
}
