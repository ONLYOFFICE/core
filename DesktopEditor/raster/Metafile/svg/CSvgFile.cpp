#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"
#include "SvgObjects/CFont.h"

#include "CSvgParser.h"

#define SVG_FILE_WIDTH  300
#define SVG_FILE_HEIGHT 150

CSvgFile::CSvgFile()
	: m_pContainer(NULL), m_pFontManager(NULL)
{}

CSvgFile::~CSvgFile()
{
	Clear();
}

bool CSvgFile::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	Clear();
	return false;
}

bool CSvgFile::ReadFromWString(const std::wstring &wsContext)
{
	Clear();

	SVG::CSvgParser oSvgParser(m_pFontManager);

	return oSvgParser.LoadFromString(wsContext, m_pContainer, this);
}

bool CSvgFile::OpenFromFile(const std::wstring &wsFile)
{
	Clear();

	m_wsWorkingDirectory = NSFile::GetDirectoryName(wsFile);

	SVG::CSvgParser oSvgParser(m_pFontManager);

	return oSvgParser.LoadFromFile(wsFile, m_pContainer, this);
}

bool CSvgFile::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight) const
{
	if (NULL == m_pContainer || m_pContainer->Empty())
		return false;

	SVG::TRect oWindow = m_pContainer->GetWindow();

	dX = oWindow.m_oX.ToDouble(NSCSS::Pixel, SVG_FILE_WIDTH);
	dY = oWindow.m_oY.ToDouble(NSCSS::Pixel, SVG_FILE_HEIGHT);

	dWidth  = 0.;
	dHeight = 0.;

	if (!oWindow.m_oWidth.Empty() && !oWindow.m_oWidth.Zero())
	{
		if (NSCSS::Percent == oWindow.m_oWidth.GetUnitMeasure())
		{
			if (!m_pContainer->GetViewBox().m_oWidth.Empty() && !m_pContainer->GetViewBox().m_oWidth.Zero())
				dWidth = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, m_pContainer->GetViewBox().m_oWidth.ToDouble(NSCSS::Pixel));
			else
				dWidth = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, SVG_FILE_WIDTH);
		}
		else
			dWidth = oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
	}
	else
		dWidth = m_pContainer->GetViewBox().m_oWidth.ToDouble(NSCSS::Pixel);

	if (!oWindow.m_oHeight.Empty() && !oWindow.m_oHeight.Zero())
	{
		if (NSCSS::Percent == oWindow.m_oHeight.GetUnitMeasure())
		{
			if (!m_pContainer->GetViewBox().m_oHeight.Empty() && !m_pContainer->GetViewBox().m_oHeight.Zero())
				dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, m_pContainer->GetViewBox().m_oHeight.ToDouble(NSCSS::Pixel));
			else
				dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, SVG_FILE_WIDTH);
		}
		else
			dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel);
	}
	else
		dHeight = m_pContainer->GetViewBox().m_oHeight.ToDouble(NSCSS::Pixel);

	if (DBL_EPSILON > dWidth)
		dWidth  = SVG_FILE_WIDTH;
	if (DBL_EPSILON > dHeight)
		dHeight = SVG_FILE_HEIGHT;

	return true;
}

const SVG::CSvgCalculator *CSvgFile::GetSvgCalculator() const
{
	return &m_oSvgCalculator;
}

void CSvgFile::SetFontManager(NSFonts::IFontManager *pFontManager)
{
	m_pFontManager = pFontManager;
}

void CSvgFile::SetWorkingDirectory(const std::wstring &wsWorkingDirectory)
{
	m_wsWorkingDirectory = wsWorkingDirectory;
}

bool CSvgFile::MarkObject(SVG::CObject *pObject)
{
	if (NULL == pObject || pObject->GetId().empty())
		return false;

	pObject->AddRef();
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

SVG::CFont *CSvgFile::GetFont(const std::wstring &wsFontFamily) const
{
	FontsFaceMap::const_iterator itFound = std::find_if(m_mFontsFace.cbegin(), m_mFontsFace.cend(), [&wsFontFamily](const std::pair<std::wstring, std::wstring>& oValue){ return wsFontFamily == oValue.first; });
	
	if (m_mFontsFace.cend() == itFound)
		return NULL;

	return dynamic_cast<SVG::CFont*>(GetMarkedObject(itFound->second));
}

std::wstring CSvgFile::GetWorkingDirectory() const
{
	return m_wsWorkingDirectory;
}

void CSvgFile::AddStyles(const std::wstring &wsStyles)
{
	m_oSvgCalculator.AddStyles(wsStyles);
}

void CSvgFile::AddFontFace(const SVG::TFontArguments& oArguments, const std::wstring &wsId)
{
	m_mFontsFace.insert(std::make_pair(oArguments.m_wsFontFamily, wsId));
}

bool CSvgFile::Draw(IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	if (NULL == pRenderer || NULL == m_pContainer || m_pContainer->Empty())
		return false;

	SVG::TRect oWindow  = m_pContainer->GetWindow();
	SVG::TRect oViewBox = m_pContainer->GetViewBox();

	if (oWindow.m_oWidth.Empty() || oWindow.m_oWidth.Zero())
	{
		if (oViewBox.m_oWidth.Empty() || oViewBox.m_oWidth.Zero())
			oWindow.m_oWidth =  SVG_FILE_WIDTH;
		else
		{
			oWindow.m_oWidth = oViewBox.m_oWidth;
			oViewBox.m_oWidth.Clear();
		}
	}

	if (oWindow.m_oHeight.Empty() || oWindow.m_oHeight.Zero())
	{
		if (oViewBox.m_oHeight.Empty() || oViewBox.m_oHeight.Zero())
			oWindow.m_oHeight =  SVG_FILE_HEIGHT;
		else
		{
			oWindow.m_oHeight = oViewBox.m_oHeight;
			oViewBox.m_oHeight.Clear();
		}
	}

	double dViewBoxWidth  = oViewBox.m_oWidth.ToDouble(NSCSS::Pixel, SVG_FILE_WIDTH);
	double dViewBoxHeight = oViewBox.m_oHeight.ToDouble(NSCSS::Pixel, SVG_FILE_HEIGHT);

	double dWindowWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, dViewBoxWidth);
	double dWindowHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, dViewBoxHeight);

	if (SVG::Equals(0., dWindowWidth) || SVG::Equals(0., dWindowHeight))
		return false;

	double oldTransform[6];
	oldTransform[0] = oldTransform[3] = 1;
	oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;

	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
	pRenderer->ResetTransform();

	double dM11 = dWidth / dWindowWidth;
	double dM22 = dHeight / dWindowHeight;

	double dScaleX = 1, dScaleY = 1;

	if (!SVG::Equals(0., dViewBoxWidth))
		dScaleX = dWindowWidth / dViewBoxWidth;

	if (!SVG::Equals(0., dViewBoxHeight))
		dScaleY = dWindowHeight / dViewBoxHeight;

	double dMinScale = std::min(dScaleX, dScaleY);

	double dSkipX = -oViewBox.m_oX.ToDouble(NSCSS::Pixel) * dScaleX * dM11;
	double dSkipY = -oViewBox.m_oY.ToDouble(NSCSS::Pixel) * dScaleY * dM22;

	pRenderer->SetTransform(dM11 * dMinScale, 0, 0, dM22 * dMinScale, dSkipX, dSkipY);

	bool bResult = m_pContainer->Draw(pRenderer, this);

	pRenderer->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

	return bResult;
}

void CSvgFile::Clear()
{
	RELEASEOBJECT(m_pContainer);
	m_oSvgCalculator.Clear();

	for (MarkedMap::reference oIter : m_mMarkedObjects)
		RELEASEINTERFACE(oIter.second);

	m_mMarkedObjects.clear();
	m_wsWorkingDirectory.clear();
}
