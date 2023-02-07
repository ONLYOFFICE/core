#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"

CSvgFile::CSvgFile()
    : m_pParser(NULL), m_pContainer(NULL)
{
	Init();
}

CSvgFile::~CSvgFile()
{
	RELEASEOBJECT(m_pParser);

	if (NULL != m_pContainer)
		delete m_pContainer;
}

bool CSvgFile::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	return false;
}

bool CSvgFile::OpenFromFile(const std::wstring &wsFile)
{
	Init();

	return m_pParser->LoadFromFile(wsFile, m_pContainer, this);
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
	if (NULL == m_pContainer || m_pContainer->Empty() || m_pContainer->GetWidth().Empty() || m_pContainer->GetHeight().Empty())
		return false;

	dX      = m_pContainer->GetX().ToDouble(NSCSS::Pixel);
	dY      = m_pContainer->GetY().ToDouble(NSCSS::Pixel);;
	dWidth  = m_pContainer->GetWidth().ToDouble(NSCSS::Pixel);;
	dHeight = m_pContainer->GetHeight().ToDouble(NSCSS::Pixel);;

	return true;
}

const SVG::CSvgCalculator *CSvgFile::GetSvgCalculator() const
{
	return &m_oSvgCalculator;
}

void CSvgFile::SetFontManager(NSFonts::IFontManager *pFontManager)
{
	if (NULL != m_pParser)
		m_pParser->SetFontManager(pFontManager);
}

void CSvgFile::AddStyles(const std::wstring &wsStyles)
{
	m_oSvgCalculator.AddStyles(wsStyles);
}

bool CSvgFile::Draw(IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	if (NULL == pRenderer || NULL == m_pContainer)
		return false;

	double dFileX = 0, dFileY = 0, dFileWidth = 0, dFileHeight = 0;

	if (!GetBounds(dFileX, dFileY, dFileWidth, dFileHeight))
		return false;

	double oldTransform[6];
	oldTransform[0] = oldTransform[3] = 1;
	oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;

	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);
	pRenderer->ResetTransform();

	bool bResult = m_pContainer->Draw(pRenderer);

	pRenderer->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

	return bResult;
}

void CSvgFile::Init()
{
	if (NULL == m_pParser)
		m_pParser = new SVG::CSvgParser();

	if (NULL == m_pContainer)
		m_pContainer = new SVG::CContainer();
	else
		m_pContainer->Clear();
}
