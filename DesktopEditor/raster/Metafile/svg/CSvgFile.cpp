#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"

CSvgFile::CSvgFile()
	: m_oContainer(L"svg")
{}

CSvgFile::~CSvgFile()
{}

bool CSvgFile::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	return false;
}

bool CSvgFile::OpenFromFile(const std::wstring &wsFile)
{
	Clear();

	return m_oParser.LoadFromFile(wsFile, &m_oContainer, this);
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
	if (m_oContainer.Empty())
		return false;

	SVG::TRect oWindow = m_oContainer.GetWindow();

	dX      = oWindow.m_oX     .ToDouble(NSCSS::Pixel);
	dY      = oWindow.m_oY     .ToDouble(NSCSS::Pixel);
	dWidth  = oWindow.m_oWidth .ToDouble(NSCSS::Pixel);
	dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

	if (0. == dWidth)
		dWidth = (!m_oContainer.GetViewBox().m_oWidth.Empty()) ? m_oContainer.GetViewBox().m_oWidth.ToDouble(NSCSS::Pixel) : 300;
	if (0. == dHeight)
		dHeight = (!m_oContainer.GetViewBox().m_oHeight.Empty()) ? m_oContainer.GetViewBox().m_oHeight.ToDouble(NSCSS::Pixel) : 150;

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

bool CSvgFile::MarkObject(SVG::CObject *pObject)
{
	if (NULL == pObject || pObject->GetId().empty())
		return false;

	m_mMarkedObjects[pObject->GetId()] = pObject;

	return true;
}

SVG::CObject *CSvgFile::GetMarkedObject(const std::wstring &wsId) const
{
	if (wsId.empty() || m_mMarkedObjects.empty())
		return NULL;

	std::wstring wsNewId = wsId;

	size_t unFound = wsNewId.find(L'#');

	if (std::wstring::npos != unFound)
		wsNewId.erase(0, unFound + 1);

	MarkedMap::const_iterator oConstIter = m_mMarkedObjects.find(wsNewId);

	if (m_mMarkedObjects.end() != oConstIter)
		return oConstIter->second;

	return NULL;
}

void CSvgFile::AddStyles(const std::wstring &wsStyles)
{
	m_oSvgCalculator.AddStyles(wsStyles);
}

bool CSvgFile::Draw(IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	if (NULL == pRenderer || m_oContainer.Empty())
		return false;

	SVG::TRect oWindow  = m_oContainer.GetWindow();
	SVG::TRect oViewBox = m_oContainer.GetViewBox();

	if (oWindow.m_oWidth.Empty() || oWindow.m_oWidth.Zero())
	{
		if (oViewBox.m_oWidth.Empty() || oViewBox.m_oWidth.Zero())
			oWindow.m_oWidth =  300;
		else
		{
			oWindow.m_oWidth = oViewBox.m_oWidth;
			oViewBox.m_oWidth.Clear();
		}
	}

	if (oWindow.m_oHeight.Empty() || oWindow.m_oHeight.Zero())
	{
		if (oViewBox.m_oHeight.Empty() || oViewBox.m_oHeight.Zero())
			oWindow.m_oHeight =  150;
		else
		{
			oWindow.m_oHeight = oViewBox.m_oHeight;
			oViewBox.m_oHeight.Clear();
		}
	}

	double dViewBoxWidth  = oViewBox.m_oWidth.ToDouble(NSCSS::Pixel);
	double dViewBoxHeight = oViewBox.m_oHeight.ToDouble(NSCSS::Pixel);
	
	double dWindowWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, dViewBoxWidth);
	double dWindowHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, dViewBoxHeight);

	if (0. == dWindowWidth || 0. == dWindowHeight)
		return false;

	double oldTransform[6];
	oldTransform[0] = oldTransform[3] = 1;
	oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;

	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
	pRenderer->ResetTransform();

	double dM11 = 1;
	double dM22 = 1;

	if (!oWindow.m_oWidth.Empty() && !oWindow.m_oWidth.Zero())
		dM11 = dWidth / dWindowWidth;

	if (!oWindow.m_oHeight.Empty() && !oWindow.m_oHeight.Zero())
		dM22 = dHeight / dWindowHeight;

	double dScaleX = 1, dScaleY = 1;

	if (!oWindow.m_oWidth.Empty() && !oWindow.m_oWidth.Zero() && !oViewBox.m_oWidth.Empty() && !oViewBox.m_oWidth.Zero())
		dScaleX = dWindowWidth / dViewBoxWidth;

	if (!oWindow.m_oHeight.Empty() && !oWindow.m_oHeight.Zero() && !oViewBox.m_oHeight.Empty() && !oViewBox.m_oHeight.Zero())
		dScaleY = dWindowHeight / dViewBoxHeight;

	double dMinScale = std::min(dScaleX, dScaleY);

	double dSkipX = -oViewBox.m_oX.ToDouble(NSCSS::Pixel) * dScaleX * dM11;
	double dSkipY = -oViewBox.m_oY.ToDouble(NSCSS::Pixel) * dScaleY * dM22;

	pRenderer->SetTransform(dM11 * dMinScale, 0, 0, dM22 * dMinScale, dSkipX, dSkipY);

	bool bResult = m_oContainer.Draw(pRenderer, this);

	pRenderer->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

	return bResult;
}

void CSvgFile::Clear()
{
	m_oContainer.Clear();
	m_oSvgCalculator.Clear();

	for (MarkedMap::iterator oIter = m_mMarkedObjects.begin(); oIter != m_mMarkedObjects.end(); ++oIter)
	{
		if (SVG::AppliedObject == oIter->second->GetType())
			delete oIter->second;
	}

	m_mMarkedObjects.clear();
}
