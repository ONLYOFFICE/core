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

bool CSVGTransformer::LoadFile(const std::wstring& file)
{
    m_internal->m_oStorage.Clear();

    //m_oXmlParser.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));
    //m_oRender.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));
    //m_oStorage.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));
    //
    //if (0 == m_oXmlParser.LoadFromFile (CString(bstrFile), &m_oStorage))
    //    return false;

    return true;
}
bool CSVGTransformer::Load(const std::wstring& content)
{
    m_internal->m_oStorage.Clear();

    //if (0 == m_oXmlParser.LoadFromString (strXml, &m_oStorage))
    //    return false;

    return true;
}

bool CSVGTransformer::Draw(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight)
{

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
