#ifndef _PDF_READER_H
#define _PDF_READER_H

#ifndef PDFREADER_USE_DYNAMIC_LIBRARY
#define PDFREADER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define PDFREADER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include "Src/ErrorConstants.h"
#include "../DesktopEditor/common/officedrawingfile.h"

namespace PdfReader
{
    class CPdfReader_Private;
    class PDFREADER_DECL_EXPORT CPdfReader : public IOfficeDrawingFile
	{
	public:

        CPdfReader(CApplicationFonts* fonts);
        virtual ~CPdfReader();

        virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
                                        const std::wstring& owner_password = L"", const std::wstring& user_password = L"");

        virtual void Close();

        virtual std::wstring GetTempDirectory();
        virtual void SetTempDirectory(const std::wstring& directory);

        virtual int GetPagesCount();
        virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY);
        virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
        virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType);

        EError       GetError();
        double       GetVersion();
        int          GetPermissions();

        bool         ExtractAllImages(const wchar_t* wsDstPath, const wchar_t* wsPrefix = 0);
		int          GetImagesCount();

        void         SetCMapFolder(const wchar_t* wsCMapFolder);
		CFontManager*GetFontManager();
				     
	private:
        CPdfReader_Private* m_pInternal;
	};
}

#endif // _PDF_READER_H
