#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"

CSvgFile::CSvgFile()
    : m_pContainer(NULL)
{}

CSvgFile::~CSvgFile()
{
	if (NULL != m_pContainer)
		delete m_pContainer;
}

bool CSvgFile::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	return false;
}

bool CSvgFile::OpenFromFile(const std::wstring &wsFile)
{
	if (NULL != m_pContainer)
		m_pContainer->Clear();

	m_oDefs.Clear();
	m_oSvgCalculator.Clear();
	return m_oParser.LoadFromFile(wsFile, m_pContainer, this);
}

bool CSvgFile::Load(const std::wstring &wsContent)
{
	return false;
}

void CSvgFile::Close()
{

}

bool CSvgFile::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight) const
{
	if (NULL == m_pContainer || m_pContainer->Empty())
		return false;

	SVG::TRect oWindow = m_pContainer->GetWindow();

	dX      = oWindow.m_oX     .ToDouble(NSCSS::Pixel);
	dY      = oWindow.m_oY     .ToDouble(NSCSS::Pixel);
	dWidth  = oWindow.m_oWidth .ToDouble(NSCSS::Pixel);
	dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

	if (0. == dWidth)
		dWidth = (!m_pContainer->GetViewBox().m_oWidth.Empty()) ? m_pContainer->GetViewBox().m_oWidth.ToDouble(NSCSS::Pixel) : 300;
	if (0. == dHeight)
		dHeight = (!m_pContainer->GetViewBox().m_oHeight.Empty()) ? m_pContainer->GetViewBox().m_oHeight.ToDouble(NSCSS::Pixel) : 150;

	return true;
}

const SVG::CSvgCalculator *CSvgFile::GetSvgCalculator() const
{
	return &m_oSvgCalculator;
}

void CSvgFile::SetFontManager(NSFonts::IFontManager *pFontManager)
{
	m_oParser.SetFontManager(pFontManager);
}

void CSvgFile::AddStyles(const std::wstring &wsStyles)
{
	m_oSvgCalculator.AddStyles(wsStyles);
}

void CSvgFile::AddDefs(XmlUtils::CXmlNode &oNode)
{
	m_oParser.ReadDefs(oNode, &m_oDefs, this);
}

bool CSvgFile::Draw(IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	if (NULL == pRenderer || NULL == m_pContainer || m_pContainer->Empty())
		return false;

	SVG::TRect oWindow  = m_pContainer->GetWindow();
	SVG::TRect oViewBox = m_pContainer->GetViewBox();

	if (oWindow.m_oWidth.Zero())
	{
		if (oViewBox.m_oWidth.Zero())
			oWindow.m_oWidth =  300;
		else
		{
			oWindow.m_oWidth = oViewBox.m_oWidth;
			oViewBox.m_oWidth.Clear();
		}
	}

	if (oWindow.m_oHeight.Zero())
	{
		if (oViewBox.m_oHeight.Zero())
			oWindow.m_oHeight =  150;
		else
		{
			oWindow.m_oHeight = oViewBox.m_oHeight;
			oViewBox.m_oHeight.Clear();
		}
	}

	double dWindowWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
	double dWindowHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

	double dViewBoxWidth  = oViewBox.m_oWidth.ToDouble(NSCSS::Pixel);
	double dViewBoxHeight = oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);

	double oldTransform[6];
	oldTransform[0] = oldTransform[3] = 1;
	oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;

	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
	pRenderer->ResetTransform();

	double dM11 = 1;
	double dM22 = 1;

	if (!oWindow.m_oWidth.Zero())
		dM11 = dWidth / dWindowWidth;

	if (!oWindow.m_oHeight.Zero())
		dM22 = dHeight / dWindowHeight;

	double dScaleX = 1, dScaleY = 1;

	if (!oWindow.m_oWidth.Zero() && !oViewBox.m_oWidth.Zero())
		dScaleX = dWindowWidth / dViewBoxWidth;

	if (!oWindow.m_oHeight.Zero() && !oViewBox.m_oHeight.Zero())
		dScaleY = dWindowHeight / dViewBoxHeight;

	double dScale = std::min(dScaleX, dScaleY);

	double dSkipX = -oViewBox.m_oX.ToDouble(NSCSS::Pixel) * dScale * dM11;
	double dSkipY = -oViewBox.m_oY.ToDouble(NSCSS::Pixel) * dScale * dM22;

//	if (dViewBoxHeight > dViewBoxWidth)
//		dSkipX += (dViewBoxHeight - dViewBoxWidth) * (dScale + 1.);

//	if (dViewBoxHeight < dViewBoxWidth)
//		dSkipY += (dViewBoxWidth - dViewBoxHeight) * (dScale + 1.);

	pRenderer->SetTransform(dM11 * dScale, 0, 0, dM22 * dScale, dSkipX, dSkipY);

	bool bResult = m_pContainer->Draw(pRenderer, &m_oDefs);

	pRenderer->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

	return bResult;
}
