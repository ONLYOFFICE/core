#pragma once

#include "libdjvu/DjVuDocument.h"
#include "libdjvu/DjVuImage.h"
#include "libdjvu/GBitmap.h"
#include "libdjvu/GScaler.h"
#include "libdjvu/IFFByteStream.h"
#include "libdjvu/BSByteStream.h"
#include "libdjvu/DataPool.h"
#include "libdjvu/DjVuText.h"
#include "libdjvu/DjVmNav.h"

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)

#define UNICODE
#define _UNICODE
#define _USE_LIBXML2_READER_
#define LIBXML_READER_ENABLED
#include "../Common/DocxFormat/Source/XML/xmlutils.h"

#include "../DesktopEditor/graphics/IRenderer.h"

class CApplicationFonts;

class CDjVuFileImplementation
{
private:

	std::wstring     m_wsTempDirectory;
	GP<DjVuDocument> m_pDoc;

public:

	CDjVuFileImplementation();
	~CDjVuFileImplementation();

	bool         LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions = L"");
	void         Close();
	std::wstring GetTempDirectory() const;
	void         SetTempDirectory(const std::wstring& wsDirectory);
	int          GetPagesCount() const;
	void         GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const;
	void         DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
	void         ConvertToRaster(CApplicationFonts* pAppFonts, int nPageIndex, const std::wstring& wsDstPath, int nImageType);

private:

	void               CreateFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& text);
	void               CreateGrFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, bool* pBreak);
	XmlUtils::CXmlNode ParseText(GP<DjVuImage> pPage);
	void               TextToRenderer(IRenderer* pRenderer, XmlUtils::CXmlNode text, double koef, bool isView = true);
	void               DrawText(IRenderer* pRenderer, double* pdCoords, const std::wstring& wsText);
	void               ParseCoords(const std::wstring& wsCoordsStr, double* pdCoords, double dKoef);
};
