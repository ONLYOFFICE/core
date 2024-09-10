#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode)
		: CObject(oNode), m_oBounds{0., 0., 0., 0.}, m_pImage(NULL)
	{
		m_oWindow.m_oX     .SetValue(oNode.GetAttribute(L"refX"));
		m_oWindow.m_oY     .SetValue(oNode.GetAttribute(L"refY"));

		m_oWindow.m_oWidth .SetValue(oNode.GetAttribute(L"markerWidth",  L"3"));
		m_oWindow.m_oHeight.SetValue(oNode.GetAttribute(L"markerHeight", L"3"));

		m_oViewBox = m_oWindow;

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

		if (L"userSpaceOnUse" == wsUnits)
			m_enUnits = Marker_UserSpaceOnUse;
		else
			m_enUnits = Marker_StrokeWidth;
	}

	CMarker::~CMarker()
	{
		if (NULL != m_pImage)
			delete m_pImage;
	}

	ObjectType CMarker::GetType() const
	{
		return AppliedObject;
	}

	void CMarker::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	void CMarker::Update(const CSvgFile *pFile)
	{
		if (NULL != m_pImage || (!m_oWindow.m_oWidth.Empty() && m_oWindow.m_oWidth.Zero()) || (!m_oWindow.m_oHeight.Empty() && m_oWindow.m_oHeight.Zero()) ||
			(!m_oViewBox.m_oWidth.Empty() && m_oViewBox.m_oWidth.Zero()) || (!m_oViewBox.m_oHeight.Empty() && m_oViewBox.m_oHeight.Zero()))
			return;

		TBounds oTempBounds;

		for (const CRenderedObject* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds();
			m_oBounds.m_dLeft   = std::min(m_oBounds.m_dLeft, oTempBounds.m_dLeft);
			m_oBounds.m_dTop    = std::min(m_oBounds.m_dTop, oTempBounds.m_dTop);
			m_oBounds.m_dRight  = std::max(m_oBounds.m_dRight, oTempBounds.m_dRight);
			m_oBounds.m_dBottom = std::max(m_oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		const double dMaxScale = std::max((m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel)), (m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel)));
		const double dMMtoPx   = 96. / 25.4;
		const double dWidth    = std::max(std::abs(m_oBounds.m_dRight - m_oBounds.m_dLeft), std::abs(m_oBounds.m_dBottom - m_oBounds.m_dTop)) * dMaxScale;
		const int    nWidth    = dWidth  * dMMtoPx;

		if (0 == nWidth)
			return;

		BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nWidth * 4];

		if (!pBgraData)
			return;

		unsigned int alfa = 0xffffff;
		//дефолтный тон должен быть прозрачным, а не белым
		//memset(pBgraData, 0xff, nWidth * nHeight * 4);
		for (int i = 0; i < nWidth * nWidth; i++)
			((unsigned int*)pBgraData)[i] = alfa;

		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nWidth);
		oFrame.put_Stride(-4 * nWidth);

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->put_Width (dWidth);
		pGrRenderer->put_Height(dWidth);

		pGrRenderer->SetTransform(dMaxScale, 0, 0, dMaxScale, 0, 0);
		pGrRenderer->SetCoordTransformOffset(-((m_oBounds.m_dLeft) * dMaxScale * dMMtoPx), -(m_oBounds.m_dTop * dMaxScale * dMMtoPx));

		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->BeginCommand(c_nImageType);

		for (const CRenderedObject* pObject : m_arObjects)
			pObject->Draw(pGrRenderer, pFile);

		pGrRenderer->EndCommand(c_nImageType);
		RELEASEINTERFACE(pGrRenderer);

		oFrame.put_Data(NULL);

		m_pImage = new Aggplus::CImage;
		m_pImage->Create(pBgraData, oFrame.get_Width(), oFrame.get_Height(), oFrame.get_Stride());
	}

	void CMarker::Draw(IRenderer *pRenderer, const std::vector<Point> &arPoints, double dStrokeWidth) const
	{
		if (NULL == m_pImage || arPoints.empty() || Equals(0., dStrokeWidth))
			return;

		const double dMaxScale = std::max((m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel)), (m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) / m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel)));
		const double dWidth    = std::max(std::abs(m_oBounds.m_dRight - m_oBounds.m_dLeft), std::abs(m_oBounds.m_dBottom - m_oBounds.m_dTop)) * ((Marker_StrokeWidth == m_enUnits) ? dStrokeWidth : 1.) * dMaxScale;

		const double dSkipX    = m_oBounds.m_dLeft * dStrokeWidth * dMaxScale;
		const double dSkipY    = m_oBounds.m_dTop  * dStrokeWidth * dMaxScale;

		for (Point oPoint : arPoints)
		{
			oPoint.dX += dSkipX;
			oPoint.dY += dSkipY;

			pRenderer->DrawImage((IGrObject*)m_pImage, oPoint.dX, oPoint.dY, dWidth, dWidth);
		}
	}
}
