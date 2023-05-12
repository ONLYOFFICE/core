#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
		: CDefObject(oNode, pParent), m_pImage(NULL)
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

		if (m_oWindow.m_oWidth.Empty() && !m_oViewBox.m_oWidth.Empty())
			m_oWindow.m_oWidth = m_oViewBox.m_oWidth;
		else if (!m_oWindow.m_oWidth.Empty() && m_oViewBox.m_oWidth.Empty())
			m_oViewBox.m_oWidth = m_oWindow.m_oWidth;

		if (m_oWindow.m_oHeight.Empty() && !m_oViewBox.m_oHeight.Empty())
			m_oWindow.m_oHeight = m_oViewBox.m_oHeight;
		else if (!m_oWindow.m_oHeight.Empty() && m_oViewBox.m_oHeight.Empty())
			m_oViewBox.m_oHeight = m_oWindow.m_oHeight;

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

		double dWidth  = m_oWindow.m_oWidth .ToDouble(NSCSS::Pixel);
		double dHeight = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		double dVBWidth  = std::min(m_oViewBox.m_oWidth .ToDouble(NSCSS::Pixel), dWidth);
		double dVBHeight = std::min(m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel), dHeight);

		double dKoef = std::min(dWidth + dVBWidth, dHeight + dVBHeight) / 2.;

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		double dMMtoPx = 96. / 25.4;

		int nWidth  = dKoef * dMMtoPx + 1;
		int nHeight = dKoef * dMMtoPx + 1;

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

		pGrRenderer->CreateFromBgraFrame(&oFrame);
		pGrRenderer->put_Width ((dWidth + dVBWidth)   / 2. * dMMtoPx);
		pGrRenderer->put_Height((dHeight + dVBHeight) / 2. * dMMtoPx);

		pGrRenderer->SetSwapRGB(false);
		pGrRenderer->BeginCommand(c_nImageType);

		pGrRenderer->SetTransform(dMMtoPx, 0., 0., dMMtoPx, 0., 0.);

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

		double dWidth  = m_oWindow.m_oWidth .ToDouble(NSCSS::Pixel);
		double dHeight = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		if (dWidth > dHeight)
			dWidth = dHeight;
		else if (dHeight > dWidth)
			dHeight = dWidth;

		double dVBWidth  = m_oViewBox.m_oWidth .ToDouble(NSCSS::Pixel);
		double dVBHeight = m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		double dDeductible = std::min((dVBWidth  - dWidth), (dVBHeight - dHeight)) / 2.;

		for (const Point& oPoint : arPoints)
		{
			Point oNewPoint(oPoint);

			oNewPoint.dX -= m_oWindow.m_oX.ToDouble(NSCSS::Pixel) - dDeductible;
			oNewPoint.dY -= m_oWindow.m_oY.ToDouble(NSCSS::Pixel) - dDeductible;

			pRenderer->DrawImage((IGrObject*)m_pImage, oNewPoint.dX, oNewPoint.dY, dWidth, dHeight);
		}
	}
}
