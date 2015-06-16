#include "Image.h"
#include "Streams.h"
#include "Document.h"

#include "../../DesktopEditor/raster/JBig2/source/Encoder/jbig2enc.h"
#include "../../DesktopEditor/raster/JBig2/source/LeptonLib/allheaders.h"

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CImageDict
	//----------------------------------------------------------------------------------------
	CImageDict::CImageDict(CXref* pXref, CDocument* pDocument)
	{
		m_pXref     = pXref;
		m_pDocument = pDocument;
	}
	void CImageDict::LoadJpeg(const wchar_t* wsFilePath, unsigned int unWidth, unsigned int unHeight)
	{
		CFileStream* pStream = new CFileStream();
		if (!pStream)
			return;

		pStream->OpenFile(wsFilePath);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceRGB");
		Add("BitsPerComponent", 8);
		SetFilter(STREAM_FILTER_DCT_DECODE);
	}
	void CImageDict::LoadJpx(const wchar_t* wsFilePath, unsigned int unWidth, unsigned int unHeight)
	{
		CFileStream* pStream = new CFileStream();
		if (!pStream)
			return;

		pStream->OpenFile(wsFilePath);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceRGB");
		Add("BitsPerComponent", 8);
		SetFilter(STREAM_FILTER_JPX_DECODE);
	}
	void CImageDict::LoadJb2(const wchar_t* wsFilePath, unsigned int unWidth, unsigned int unHeight)
	{
		CFileStream* pStream = new CFileStream();
		if (!pStream)
			return;

		pStream->OpenFile(wsFilePath);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceGray");
		Add("BitsPerComponent", 1);
		SetFilter(STREAM_FILTER_JBIG2_DECODE);
	}
	void CImageDict::LoadCCITT4(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight)
	{
		CFileStream* pStream = new CFileStream();
		if (!pStream)
			return;

		pStream->OpenFile(wsTempFile);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceGray");
		Add("BitsPerComponent", 1);

		CArrayObject* pDecodeParams = new CArrayObject();
		CDictObject* pParams = new CDictObject();
		pDecodeParams->Add(pParams);
		pParams->Add("K", 0);
		pParams->Add("Columns", unWidth);
		pParams->Add("EncodedByteAlign", true);
		Add("DecodeParams", pDecodeParams);

		SetFilter(STREAM_FILTER_CCITT_DECODE);

	}
	void CImageDict::LoadRaw(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight)
	{
		CMemoryStream* pStream = new CMemoryStream();
		if (!pStream)
			return;

		for (unsigned int unIndex = 0, unSize = 4 * unWidth * unHeight; unIndex < unSize; unIndex += 4)
		{
			pStream->Write(pBgra + unIndex, 3);
		}

		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("ColorSpace", "DeviceRGB");
		Add("Width", unWidth);
		Add("Height", unHeight);
		Add("BitsPerComponent", 8);
	}
	void CImageDict::LoadSMask(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight)
	{
		CMemoryStream* pStream = new CMemoryStream();
		if (!pStream)
			return;

		for (unsigned int unIndex = 0, unSize = 4 * unWidth * unHeight; unIndex < unSize; unIndex += 4)
		{
			pStream->Write(pBgra + unIndex + 3, 1);
		}

		CImageDict* pImageSMask = new CImageDict(m_pXref, m_pDocument);
		if (!pImageSMask)
			return;

		pImageSMask->SetStream(m_pXref, pStream);
		pImageSMask->Add("Type", "XObject");
		pImageSMask->Add("Subtype", "Image");
		pImageSMask->Add("ColorSpace", "DeviceGray");
		pImageSMask->Add("Width", unWidth);
		pImageSMask->Add("Height", unHeight);
		pImageSMask->Add("BitsPerComponent", 8);

//#ifndef FILTER_FLATE_DECODE_DISABLED
//		pImageSMask->SetFilter(STREAM_FILTER_LZW_DECODE | STREAM_FILTER_FLATE_DECODE);
//#else
		pImageSMask->SetFilter(STREAM_FILTER_FLATE_DECODE);
//#endif

		Add("SMask", pImageSMask);
	}
	void CImageDict::LoadBW(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride)
	{	
		SetStream(m_pXref, new CMemoryStream());
		CJbig2Global* pJbig2Global = m_pDocument->GetJbig2Global();
		pJbig2Global->AddImage(pImage, unWidth, unHeight, unStride, GetStream());

		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceGray");
		Add("BitsPerComponent", 1);
		SetFilter(STREAM_FILTER_JBIG2_DECODE);

		CArrayObject* pDecodeParams = new CArrayObject();
		CDictObject* pParams = new CDictObject();
		pDecodeParams->Add(pParams);
		pParams->Add("JBIG2Globals", pJbig2Global);
		Add("DecodeParms", pDecodeParams);
	}
	void CImageDict::LoadBW(Pix* pPix, unsigned int unWidth, unsigned int unHeight)
	{
		SetStream(m_pXref, new CMemoryStream());
		CJbig2Global* pJbig2Global = m_pDocument->GetJbig2Global();
		pJbig2Global->AddImage(pPix, GetStream());

		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);
		Add("ColorSpace", "DeviceGray");
		Add("BitsPerComponent", 1);
		SetFilter(STREAM_FILTER_JBIG2_DECODE);

		CArrayObject* pDecodeParams = new CArrayObject();
		CDictObject* pParams = new CDictObject();
		pDecodeParams->Add(pParams);
		pParams->Add("JBIG2Globals", pJbig2Global);
		Add("DecodeParms", pDecodeParams);
	}
	//----------------------------------------------------------------------------------------
	// CJbig2Global
	//----------------------------------------------------------------------------------------
	CJbig2Global::CJbig2Global(CXref* pXref) : CDictObject(pXref, true)
	{
		m_pXref = pXref;
		m_pContext = jbig2_init(0.85, 0.5, -1, -1, false, -1);
	}
	CJbig2Global::~CJbig2Global()
	{
		if (m_pContext)
			jbig2_destroy(m_pContext);
	}
	void CJbig2Global::FlushStreams()
	{
		CStream* pStream = GetStream();

		int nLen = 0;
		BYTE* pBuffer = jbig2_pages_complete(m_pContext, &nLen);

		if (pBuffer)
		{
			pStream->Write(pBuffer, nLen);
			free(pBuffer);
		}

		for (int nIndex = 0, nCount = m_vImages.size(); nIndex < nCount; nIndex++)
		{
			pBuffer = jbig2_produce_page(m_pContext, nIndex, -1, -1, &nLen);
			if (pBuffer)
			{
				pStream = m_vImages.at(nIndex);
				if (pStream)
				{
					pStream->Write(pBuffer, nLen);
				}
				free(pBuffer);
			}
		}

		jbig2_destroy(m_pContext);
		m_pContext = NULL;
	}
	void CJbig2Global::AddImage(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride, CStream* pImageStream)
	{
		if (!m_pContext)
			return;

		Pix* pPix = pixCreate(unWidth, unHeight, 1);
		if (!pPix)
			return;

		BYTE* pLine = (BYTE*)pImage;
		for (unsigned int unY = 0; unY < unHeight; unY++, pLine += unStride)
		{
			char nBit = 0;
			BYTE* pCur = pLine;
			for (unsigned int unX = 0; unX < unWidth; unX++)
			{
				pixSetPixel(pPix, unX, unY, pCur[0] & (1 << nBit));
				nBit++;

				if (8 == nBit)
				{
					nBit = 0;
					pCur++;
				}
			}
		}

		jbig2_add_page(m_pContext, pPix);
		pixDestroy(&pPix);
		m_vImages.push_back(pImageStream);
	}
	void CJbig2Global::AddImage(Pix* pPix, CStream* pImageStream)
	{
		if (!m_pContext)
			return;

		jbig2_add_page(m_pContext, pPix);
		m_vImages.push_back(pImageStream);
	}
	int  CJbig2Global::GetImagesCount()
	{
		return m_vImages.size();
	}
}