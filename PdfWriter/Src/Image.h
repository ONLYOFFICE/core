#ifndef _PDF_WRITER_SRC_IMAGE_H
#define _PDF_WRITER_SRC_IMAGE_H

#include "Objects.h"

struct jbig2ctx;
struct Pix;

namespace PdfWriter
{
	class CDocument;
	class CXObject : public CDictObject
	{
	public:
		EDictType GetDictType() const
		{
			return dict_type_XOBJECT;
		}
	};
	//----------------------------------------------------------------------------------------
	// CImageDict
	//----------------------------------------------------------------------------------------
	class CImageDict : public CXObject
	{
	public:

		CImageDict(CXref* pXref, CDocument* pDocument);

		void LoadJpeg(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadJpx(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadJb2(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadCCITT4(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadRaw(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight);
		void LoadSMask(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight);
		void LoadBW(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride);
		void LoadBW(Pix* pPix, unsigned int unWidth, unsigned int unHeight);

	private:

		CDocument*   m_pDocument;
		CXref*       m_pXref;
	};
	//----------------------------------------------------------------------------------------
	// CJbig2Global
	//----------------------------------------------------------------------------------------
	class CJbig2Global : public CDictObject
	{
	public:

		CJbig2Global(CXref* pXref);
		~CJbig2Global();
		void AddImage(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride, CStream* pImageStream);
		void AddImage(Pix* pPix, CStream* pImageStream);
		void FlushStreams();
		int  GetImagesCount();

	private:

		CXref*                m_pXref;
		jbig2ctx*             m_pContext;
		std::vector<CStream*> m_vImages;
	};
}

#endif // _PDF_WRITER_SRC_IMAGE_H