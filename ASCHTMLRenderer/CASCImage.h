#ifndef C_IMAGE_RENDERER
#define C_IMAGE_RENDERER

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../DesktopEditor/graphics/IRenderer.h"

const long c_lImageTypeUnknown  = 0x0000;
const long c_lImageTypeMetafile = 0x1000;
const long c_lImageTypeBitmap   = 0x2000;

const long c_lMetaWmf = 0x01;
const long c_lMetaEmf = 0x02;
const long c_lMetaSVG = 0x04;


const long c_ag_ImageTypeBitmap			= 0;
const long c_ag_ImageTypeMetafile		= 1;

namespace MetaFile
{
	class CMetaFile;
};

class CFontManager;
class CApplicationFonts;

namespace Aggplus
{
	class CImage;
}
namespace NSHtmlRenderer
{
	class IASCImage
	{
	public:
		virtual HRESULT get_Type(LONG* lType) = 0;

		virtual HRESULT get_Width(LONG* lWidth) = 0;
		virtual HRESULT put_Width(LONG lWidth) = 0;

		virtual HRESULT get_Height(LONG* lHeight) = 0;
		virtual HRESULT put_Height(LONG lHeight) = 0;

		virtual HRESULT get_DpiX(double* dDpiX) = 0;
		virtual HRESULT put_DpiX(double dDpiX) = 0;

		virtual HRESULT get_DpiY(double* dDpiY) = 0;
		virtual HRESULT put_DpiY(double dDpiY) = 0;

		virtual CFontManager* get_FontManager() = 0;
		virtual void put_FontManager(CFontManager* pManager) = 0;

		virtual HRESULT LoadFromFile(const std::wstring& bsFilePath) = 0;
		virtual HRESULT DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight) = 0;
	};

	// CAVSImage
	class CASCImage : public IASCImage
	{
	private:

		MetaFile::CMetaFile*			m_pMetafile;

		CString  m_wsTempFilePath;

		long      m_lImageType; // 0 - BMP, 1 - WMF, 2 - EFM

		double   m_dDpiX;
		double   m_dDpiY;

		Aggplus::CImage* m_pMediaData;

        bool      m_bLoadOnlyMeta; // Загружаем только метафайл

	public:

		CASCImage(CApplicationFonts *pAppFonts);
		~CASCImage();

	private:

		void Open(const std::wstring& bsFilePath);
		void Close();
	public:

		HRESULT get_Type(LONG* lType);
		HRESULT get_Width(LONG* lWidth);
		HRESULT put_Width(LONG lWidth);
		HRESULT get_Height(LONG* lHeight);
		HRESULT put_Height(LONG lHeight);
		HRESULT get_DpiX(double* dDpiX);
		HRESULT put_DpiX(double dDpiX);
		HRESULT get_DpiY(double* dDpiY);
		HRESULT put_DpiY(double dDpiY);
		HRESULT LoadFromFile(const std::wstring& bsFilePath);
		HRESULT DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight);
		HRESULT LoadOnlyMeta(bool bVal);
		HRESULT LoadSVG(const std::wstring& sVal);
		CFontManager* get_FontManager();
		void put_FontManager(CFontManager* oManager);
		Aggplus::CImage* get_BitmapImage();
	};
}
#endif
