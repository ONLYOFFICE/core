#include "CSvgFile.h"

#include "SvgObjects/CHeader.h"

CSvgFile::CSvgFile()
    : m_pParser(NULL), m_pStorage(NULL)
{
	Init();
}

CSvgFile::~CSvgFile()
{
	RELEASEOBJECT(m_pParser);
	RELEASEOBJECT(m_pStorage);
}

bool CSvgFile::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	return false;
}

bool CSvgFile::OpenFromFile(const std::wstring &wsFile)
{
	Init();

	return m_pParser->LoadFromFile(wsFile, m_pStorage);
}

bool CSvgFile::Load(const std::wstring &wsContent)
{
	return false;
}

void CSvgFile::Close()
{

}

void CSvgFile::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight) const
{
	if (m_pStorage->Empty())
		return;

	SVG::CHeader* pHeader = dynamic_cast<SVG::CHeader*>(m_pStorage->GetFirstObject());

	if (NULL == pHeader)
		return;

	pHeader->GetBounds(dX, dY, dWidth, dHeight);
}

void CSvgFile::SetFontManager(NSFonts::IFontManager *pFontManager)
{
	if (NULL != m_pParser)
		m_pParser->SetFontManager(pFontManager);
}

bool CSvgFile::Draw(IRenderer *pRenderer, double dX, double dY, double dWidth, double dHeight) const
{
	if (NULL == pRenderer || NULL == m_pStorage)
		return false;

	double dFileX = 0, dFileY = 0, dFileWidth = 0, dFileHeight = 0;

	GetBounds(dFileX, dFileY, dFileWidth, dFileHeight);

	double oldTransform[6];
	oldTransform[0] = oldTransform[3] = 1;
	oldTransform[1] = oldTransform[2] = oldTransform[4] = oldTransform[5] = 0;
	pRenderer->GetTransform(&oldTransform[0], &oldTransform[1], &oldTransform[2], &oldTransform[3], &oldTransform[4], &oldTransform[5]);

	pRenderer->SetTransform(dWidth / dFileWidth, 0, 0, dHeight / dFileHeight, dX, dY);

	bool bResult =  m_pStorage->Draw(pRenderer);

	pRenderer->SetTransform(oldTransform[0], oldTransform[1], oldTransform[2], oldTransform[3], oldTransform[4], oldTransform[5]);

	return bResult;
}

void CSvgFile::Init()
{
	if (NULL == m_pParser)
		m_pParser = new SVG::CSvgParser;
	else
		m_pParser->Clear();

	if (NULL == m_pStorage)
		m_pStorage = new SVG::CSvgStorage;
	else
		m_pStorage->Clear();
}
