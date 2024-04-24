/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CMarker.h"

#include "../../../graphics/pro/Graphics.h"
#include "../SvgUtils.h"

namespace SVG
{
	CMarker::CMarker(XmlUtils::CXmlNode &oNode)
		: CObject(oNode), m_pImage(NULL)
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

		double dVBWidth  = m_oViewBox.m_oWidth .ToDouble(NSCSS::Pixel);
		double dVBHeight = m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		double dMaxValue = std::max(dVBWidth, dVBHeight);

		NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();

		double dMMtoPx = 96. / 25.4;

		int nWidth  = dMaxValue * dMMtoPx + 1;
		int nHeight = dMaxValue * dMMtoPx + 1;

		if (0 == nWidth || 0 == nHeight)
			return;

		BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nHeight * 4];

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
		pGrRenderer->put_Width (dMaxValue);
		pGrRenderer->put_Height(dMaxValue);

		pGrRenderer->SetCoordTransformOffset(((dMaxValue - dVBWidth)  / 2. - m_oViewBox.m_oX.ToDouble(NSCSS::Pixel)) * dMMtoPx,
											 ((dMaxValue - dVBHeight) / 2. - m_oViewBox.m_oY.ToDouble(NSCSS::Pixel)) * dMMtoPx);

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

		double dWidth  = m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel) * ((Marker_StrokeWidth == m_enUnits) ? dStrokeWidth : 1.);
		double dHeight = m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel) * ((Marker_StrokeWidth == m_enUnits) ? dStrokeWidth : 1.);

		double dVBWidth  = m_oViewBox.m_oWidth.ToDouble(NSCSS::Pixel);
		double dVBHeight = m_oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

		double dMaxValue = std::max(dVBWidth, dVBHeight);
		double dMaxScale = std::max(dMaxValue / dVBWidth, dMaxValue / dVBHeight);

		double dSkipX  = (m_oWindow.m_oX.ToDouble(NSCSS::Pixel) - m_oViewBox.m_oX.ToDouble(NSCSS::Pixel)) / dMaxScale + (dMaxValue - dVBWidth)  * (dWidth  / (2 * dMaxValue));
		double dSkipY  = (m_oWindow.m_oY.ToDouble(NSCSS::Pixel) - m_oViewBox.m_oY.ToDouble(NSCSS::Pixel)) / dMaxScale + (dMaxValue - dVBHeight) * (dHeight / (2 * dMaxValue));

		for (const Point& oPoint : arPoints)
		{
			Point oNewPoint(oPoint);

			oNewPoint.dX -= dSkipX;
			oNewPoint.dY -= dSkipY;

			pRenderer->DrawImage((IGrObject*)m_pImage, oNewPoint.dX, oNewPoint.dY, dWidth, dHeight);
		}
	}
}
