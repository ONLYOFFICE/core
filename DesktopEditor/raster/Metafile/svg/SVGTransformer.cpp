#include "SVGTransformer.h"
#include "SVGFramework.h"

class CSVGTransformer_private
{
public:
	SVG::Parser		m_oXmlParser;
	SVG::Storage	m_oStorage;
	SVG::Painter	m_oRender;
};

CSVGTransformer::CSVGTransformer()
{
	m_internal = new CSVGTransformer_private();
}
CSVGTransformer::~CSVGTransformer()
{
	delete m_internal;
}

bool CSVGTransformer::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize)
{
	m_internal->m_oStorage.Clear();

	int nSize = NSBase64::Base64EncodeGetRequiredLength(unSize);

	if (nSize <= 0)
		return false;

	BYTE* ucValue = new BYTE[nSize];

	if (!ucValue)
		return false;

	NSBase64::Base64Encode(pBuffer, unSize, ucValue, &nSize);
	std::wstring wsValue(ucValue, ucValue + nSize);

	RELEASEOBJECT(ucValue);

	if (wsValue.empty())
		return false;

	if (0 == m_internal->m_oXmlParser.LoadFromString(wsValue, &m_internal->m_oStorage))
		return false;
}

bool CSVGTransformer::OpenFromFile(const std::wstring& file)
{
	m_internal->m_oStorage.Clear();

	std::wstring sDirectory = NSFile::GetDirectoryName(file);

	m_internal->m_oXmlParser.SetWorkingDirectory(sDirectory);
	m_internal->m_oRender.SetWorkingDirectory(sDirectory);
	m_internal->m_oStorage.SetWorkingDirectory(sDirectory);

	try
	{
		if (!m_internal->m_oXmlParser.LoadFromFile(file, &m_internal->m_oStorage))
			return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}
bool CSVGTransformer::Load(const std::wstring& content)
{
	m_internal->m_oStorage.Clear();

	if (0 == m_internal->m_oXmlParser.LoadFromString(content, &m_internal->m_oStorage))
		return false;

	return true;
}

void CSVGTransformer::Close()
{
	delete m_internal;
	m_internal = new CSVGTransformer_private();
}

void CSVGTransformer::SetFontManager(NSFonts::IFontManager* pFontManager)
{
	m_internal->m_oRender.SetFontManager(pFontManager);
	return;
}

bool CSVGTransformer::Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
{
	m_internal->m_oRender.SetCSS(m_internal->m_oXmlParser.GetCSS());
	m_internal->m_oRender.Draw(&m_internal->m_oStorage, pRenderer, m_internal->m_oXmlParser.GetUnitSystem(), dX, dY, dWidth, dHeight);
	return true;
}
int CSVGTransformer::get_Width()
{
	return m_internal->m_oXmlParser.GetWidth();
}
int CSVGTransformer::get_Height()
{
	return m_internal->m_oXmlParser.GetHeight();
}
int CSVGTransformer::get_Metrics()
{
	return m_internal->m_oXmlParser.GetMetrics();
}
