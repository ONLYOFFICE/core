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
	return CalculateFinalSize(true, dX, dY, dWidth, dHeight);
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

	double dFileX, dFileY, dFileWidth, dFileHeight;

	if (!CalculateFinalSize(false, dFileX, dFileY, dFileWidth, dFileHeight))
		return false;

	double oldTransform[6];

	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
	pRenderer->ResetTransform();

	const double dM11 = dWidth  / dFileWidth;
	const double dM22 = dHeight / dFileHeight;

	double dScaleX = 1.,
	       dScaleY = 1.;
	double dTranslateX = dFileX * dM11,
	       dTranslateY = dFileY * dM22;

	const SVG::TRect oViewBox{m_pContainer->GetViewBox()};

	if (!oViewBox.m_oWidth.Empty() && !oViewBox.m_oHeight.Empty())
	{
		dScaleX = dFileWidth  / oViewBox.m_oWidth .ToDouble(NSCSS::Pixel, SVG_FILE_WIDTH );
		dScaleY = dFileHeight / oViewBox.m_oHeight.ToDouble(NSCSS::Pixel, SVG_FILE_HEIGHT);

		dTranslateX -= oViewBox.m_oX.ToDouble(NSCSS::Pixel) * dScaleX * dM11;
		dTranslateY -= oViewBox.m_oY.ToDouble(NSCSS::Pixel) * dScaleY * dM22;
	}

	const double dMinScale = std::min(dScaleX, dScaleY);
	pRenderer->SetTransform(dM11 * dMinScale, 0, 0, dM22 * dMinScale, dTranslateX, dTranslateY);

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

bool CSvgFile::CalculateFinalSize(bool bUseViewBox, double& dX, double& dY, double& dWidth, double& dHeight) const
{
	if (NULL == m_pContainer || m_pContainer->Empty())
		return false;

	const SVG::TRect oWindow  = m_pContainer->GetWindow();
	const SVG::TRect oViewBox = m_pContainer->GetViewBox();

	const double dViewBoxWidth {oViewBox.m_oWidth .ToDouble()};
	const double dViewBoxHeigth{oViewBox.m_oHeight.ToDouble()};

	dX = 0.;
	dY = 0.;

	dWidth  = 0.;
	dHeight = 0.;

	#define GET_PREV_WIDTH_VALUE ((NSCSS::UnitMeasure::Em == oWindow.m_oWidth.GetUnitMeasure() || NSCSS::UnitMeasure::Rem == oWindow.m_oWidth.GetUnitMeasure()) ? DEFAULT_FONT_SIZE : ((!SVG::Equals(0., dViewBoxWidth )) ? dViewBoxWidth : SVG_FILE_WIDTH))
	#define GET_PREV_HEIGHT_VALUE ((NSCSS::UnitMeasure::Em == oWindow.m_oHeight.GetUnitMeasure() || NSCSS::UnitMeasure::Rem == oWindow.m_oHeight.GetUnitMeasure()) ? DEFAULT_FONT_SIZE : ((!SVG::Equals(0., dViewBoxHeigth )) ? dViewBoxHeigth : SVG_FILE_HEIGHT))

	if (!oWindow.m_oWidth.Empty() && !oWindow.m_oHeight.Empty())
	{
		dWidth  = oWindow.m_oWidth .ToDouble(NSCSS::Pixel, GET_PREV_WIDTH_VALUE);
		dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, GET_PREV_HEIGHT_VALUE);
		return true;
	}

	if (bUseViewBox)
	{
		if (!oViewBox.m_oWidth.Empty() && !oViewBox.m_oHeight.Empty())
		{
			if (!oWindow.m_oWidth.Empty())
			{
				dWidth = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, GET_PREV_WIDTH_VALUE);
				const double dAspectRatio{dViewBoxWidth / dViewBoxHeigth};
				dHeight = dWidth / dAspectRatio;
				return true;
			}

			if (!oWindow.m_oHeight.Empty())
			{
				dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, GET_PREV_HEIGHT_VALUE);
				const double dAspectRatio{dViewBoxWidth / dViewBoxHeigth};
				dWidth = dHeight * dAspectRatio;
				return true;
			}

			dX = oViewBox.m_oX.ToDouble();
			dY = oViewBox.m_oY.ToDouble();

			dWidth  = dViewBoxWidth;
			dHeight = dViewBoxHeigth;

			return true;
		}
	}

	if (!oWindow.m_oWidth.Empty() || !oWindow.m_oHeight.Empty())
	{
		if (!oViewBox.m_oWidth.Empty() && !oViewBox.m_oHeight.Empty())
		{
			const double dAspectRatio{dViewBoxWidth / dViewBoxHeigth};

			if (!oWindow.m_oWidth.Empty())
			{
				dWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, GET_PREV_WIDTH_VALUE);
				dHeight = dWidth / dAspectRatio;
			}
			else if (!oWindow.m_oHeight.Empty())
			{
				dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, GET_PREV_HEIGHT_VALUE);
				dWidth  = dHeight * dAspectRatio;
			}

			return true;
		}

		if (NULL != m_pContainer && !m_pContainer->Empty())
		{
			SVG::SvgMatrix oTransform;
			const SVG::TBounds oBounds{m_pContainer->GetBounds(&oTransform)};

			const double dContentWidth {oBounds.m_dRight  - oBounds.m_dLeft};
			const double dContentHeight{oBounds.m_dBottom - oBounds.m_dTop };

			const double dAspectRatio{dContentWidth / dContentHeight};

			if (!oWindow.m_oWidth.Empty())
			{
				dWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, GET_PREV_WIDTH_VALUE);
				dHeight = dWidth / dAspectRatio;
			}
			else if (!oWindow.m_oHeight.Empty())
			{
				dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, GET_PREV_HEIGHT_VALUE);
				dWidth  = dHeight * dAspectRatio;
			}

			dX = oBounds.m_dLeft;
			dY = oBounds.m_dTop;

			return true;
		}

		if (!oWindow.m_oWidth.Empty())
		{
			dWidth  = oWindow.m_oWidth.ToDouble(NSCSS::Pixel, GET_PREV_WIDTH_VALUE);
			dHeight = SVG_FILE_HEIGHT;
		}
		else if (!oWindow.m_oHeight.Empty())
		{
			dHeight = oWindow.m_oHeight.ToDouble(NSCSS::Pixel, GET_PREV_HEIGHT_VALUE);
			dWidth  = SVG_FILE_WIDTH;
		}

		return true;
	}

	if (NULL != m_pContainer && !m_pContainer->Empty())
	{
		SVG::SvgMatrix oTransform;
		const SVG::TBounds oBounds{m_pContainer->GetBounds(&oTransform)};

		dX = oBounds.m_dLeft;
		dY = oBounds.m_dTop;

		dWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		dHeight = oBounds.m_dBottom - oBounds.m_dTop;

		return true;
	}

	dWidth  = SVG_FILE_WIDTH;
	dHeight = SVG_FILE_HEIGHT;

	return true;
}
