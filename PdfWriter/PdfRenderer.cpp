#include "PdfRenderer.h"

#include "Src/Document.h"
#include "Src/Pages.h"

using namespace PdfWriter;

CPdfRenderer::CPdfRenderer()
{
	m_pDocument = new CDocument();
	if (!m_pDocument || !m_pDocument->CreateNew())
	{
		SetError();
		return;
	}

	m_dPageHeight = 297;
	m_dPageWidth  = 210;
	m_pPage       = NULL;
}
CPdfRenderer::~CPdfRenderer()
{
	if (m_pDocument)
		delete m_pDocument;
}
HRESULT CPdfRenderer::get_Type(LONG* lType)
{
	*lType = c_nPDFWriter;
}
HRESULT CPdfRenderer::NewPage()
{
	if (!IsValid())
		return;

	m_pPage = m_pDocument->AddPage();

	if (!m_pPage)
		SetError();

	m_pPage->SetWidth(m_dPageWidth);
	m_pPage->SetHeight(m_dPageHeight);
}
HRESULT CPdfRenderer::get_Height(double* dHeight)
{
	*dHeight = m_dPageHeight;
}
HRESULT CPdfRenderer::put_Height(const double& dHeight)
{
	if (!IsValid() || !m_pPage)
		return;

	m_dPageHeight = dHeight;
	m_pPage->SetHeight(dHeight);
}
HRESULT CPdfRenderer::get_Width(double* dWidth)
{
	*dWidth = m_dPageWidth;
}
HRESULT CPdfRenderer::put_Width(const double& dWidth)
{
	if (!IsValid() || !m_pPage)
		return;

	m_dPageWidth = dWidth;
	m_pPage->SetWidth(dWidth);
}
HRESULT CPdfRenderer::get_DpiX(double* dDpiX)
{
	*dDpiX = 72;
}
HRESULT CPdfRenderer::get_DpiY(double* dDpiY)
{
	*dDpiY = 72;
}
