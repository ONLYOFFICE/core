#include "CSvgFile.h"

#include "SvgObjects/CContainer.h"
#include "SvgObjects/CHeader.h"

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
	if (NULL == m_pContainer || m_pContainer->Empty())
		return false;

	SVG::CHeader *pHeader = m_pContainer->GetHeader();

	if (NULL == pHeader)
		return false;

	pHeader->GetBounds(dX, dY, dWidth, dHeight);
	return true;
}

SVG::CGeneralStyle *CSvgFile::GetBaseStyle()
{
	return &m_oStyle;
}

void CSvgFile::SetFontManager(NSFonts::IFontManager *pFontManager)
{
	if (NULL != m_pParser)
		m_pParser->SetFontManager(pFontManager);
}

void CSvgFile::AddStyle(const std::wstring &wsStyle)
{
	m_oStyle.AddStyle(wsStyle);
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

	m_oStyle.AddStyle(L"svg{transform:matrix(" + std::to_wstring((dWidth)  / dFileWidth) + L", 0, 0, " +
	                                             std::to_wstring((dHeight) / dFileHeight) +L", " +
	                                             std::to_wstring(dX) + L", " + std::to_wstring(dY) + L")};");

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
