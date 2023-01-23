/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Image.h"
#include "Streams.h"
#include "Document.h"

// TODO: write JPG from Photoshop...
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

namespace NSImageReSaver
{
    static void CorrectImage(const wchar_t* wsFileName, BYTE*& pBuffer, int& nBufferSize, unsigned int& unWidth, unsigned int& unHeight)
    {
        pBuffer = NULL;
        nBufferSize = 0;

        CImageFileFormatChecker oChecker(wsFileName);
        if (oChecker.eFileType != _CXIMAGE_FORMAT_JPG)
            return;

        NSFile::CFileBinary oFile;
        if (!oFile.OpenFile(wsFileName))
            return;

        if (20 > oFile.GetFileSize())
            return;

        BYTE data[20];
        DWORD dwRead = 0;
        if (!oFile.ReadFile(data, 20, dwRead))
            return;

        std::string sFind((char*)data, 20);
        oFile.CloseFile();

        if (std::string::npos == sFind.find("Photoshop") && std::string::npos == sFind.find("photoshop"))
            return;

        CBgraFrame oFrame;
        if (!oFrame.OpenFile(wsFileName))
            return;

        oFrame.SetJpegQuality(85.0);
        if (!oFrame.Encode(pBuffer, nBufferSize, _CXIMAGE_FORMAT_JPG))
            return;

        if (!pBuffer || !nBufferSize)
            return;

        unWidth = (unsigned int)oFrame.get_Width();
        unHeight = (unsigned int)oFrame.get_Height();
    }
}

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
    void CImageDict::LoadJpeg(const wchar_t* wsFilePath, unsigned int unWidth, unsigned int unHeight, bool bGrayScale)
	{
        BYTE* pCorrectBuffer = NULL;
        int nBufferSize = 0;
        NSImageReSaver::CorrectImage(wsFilePath, pCorrectBuffer, nBufferSize, unWidth, unHeight);

        if (pCorrectBuffer != NULL)
        {
            this->LoadJpeg(pCorrectBuffer, nBufferSize, unWidth, unHeight, bGrayScale);
            free(pCorrectBuffer);
            return;
        }


		CImageFileStream* pStream = new CImageFileStream();
		if (!pStream)
			return;

		pStream->OpenFile(wsFilePath);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);

        if (bGrayScale)
            Add("ColorSpace", "DeviceGray");
        else
            Add("ColorSpace", "DeviceRGB");

		Add("BitsPerComponent", 8);
		SetFilter(STREAM_FILTER_DCT_DECODE);
	}
    void CImageDict::LoadJpeg(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight, bool bGrayScale)
	{
		CMemoryStream* pStream = new CMemoryStream();
		if (!pStream)
			return;

		pStream->Write(pBuffer, nBufferSize);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("Height", unHeight);
		Add("Width", unWidth);

        if (bGrayScale)
            Add("ColorSpace", "DeviceGray");
        else
            Add("ColorSpace", "DeviceRGB");

        Add("BitsPerComponent", 8);
		SetFilter(STREAM_FILTER_DCT_DECODE);
	}
	void CImageDict::LoadJpx(const wchar_t* wsFilePath, unsigned int unWidth, unsigned int unHeight)
	{
		CImageFileStream* pStream = new CImageFileStream();
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
	void CImageDict::LoadJpx(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight)
	{
		CMemoryStream* pStream = new CMemoryStream();
		if (!pStream)
			return;

		pStream->Write(pBuffer, nBufferSize);
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
		CImageFileStream* pStream = new CImageFileStream();
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
		CImageFileStream* pStream = new CImageFileStream();
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
		CMemoryStream* pStream = new CMemoryStream(3 * unWidth * unHeight);
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
		SetFilter(STREAM_FILTER_FLATE_DECODE);
	}
	void CImageDict::LoadRaw(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight)
	{
		CMemoryStream* pStream = new CMemoryStream(unSize);
		if (!pStream)
			return;
		pStream->Write(pBuffer, unSize);
		SetStream(m_pXref, pStream);
		Add("Type", "XObject");
		Add("Subtype", "Image");
		Add("ColorSpace", "DeviceRGB");
		Add("Width", unWidth);
		Add("Height", unHeight);
		Add("BitsPerComponent", 8);
		SetFilter(STREAM_FILTER_FLATE_DECODE);
	}
	void CImageDict::LoadSMask(CMemoryStream* pStream, const unsigned int unWidth, const unsigned int& unHeight)
	{
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
	void CImageDict::LoadSMask(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight, unsigned char unAlpha, bool bVerFlip)
	{
		if (m_pDocument->IsPDFA())
			return;

		CMemoryStream* pStream = new CMemoryStream(unWidth * unHeight);
		if (!pStream)
			return;

		if (255 != unAlpha)
		{
			double dKoef = unAlpha / 255.0;

			if (!bVerFlip)
			{
				for (unsigned int unIndex = 0, unSize = 4 * unWidth * unHeight; unIndex < unSize; unIndex += 4)
				{
					BYTE nChar = *(pBgra + unIndex + 3) * dKoef;
					pStream->Write(&nChar, 1);
				}
			}
			else
			{
				int nWidth  = (int)unWidth;
				int nHeight = (int)unHeight;
				for (int nY = nHeight - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int unIndex = 4 * (nX + nY * nWidth);
						BYTE nChar = *(pBgra + unIndex + 3) * dKoef;
						pStream->Write(&nChar, 1);
					}
				}
			}
		}
		else
		{
			if (!bVerFlip)
			{
				for (unsigned int unIndex = 0, unSize = 4 * unWidth * unHeight; unIndex < unSize; unIndex += 4)
				{
					pStream->Write(pBgra + unIndex + 3, 1);
				}
			}
			else
			{
				int nWidth  = (int)unWidth;
				int nHeight = (int)unHeight;
				for (int nY = nHeight - 1; nY >= 0; nY--)
				{
					for (int nX = 0; nX < nWidth; nX++)
					{
						int unIndex = 4 * (nX + nY * nWidth);
						pStream->Write(pBgra + unIndex + 3, 1);
					}
				}
			}
		}

		LoadSMask(pStream, unWidth, unHeight);
	}
	void CImageDict::LoadSMask(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight)
	{
		CMemoryStream* pStream = new CMemoryStream(unSize);
		if (!pStream)
			return;

		pStream->Write(pBuffer, unSize);
		LoadSMask(pStream, unWidth, unHeight);
	}
	void CImageDict::LoadSMask(const BYTE& unAlpha, const unsigned int& unWidth, const unsigned int& unHeight)
	{
		unsigned int unSize = unWidth * unHeight;
		CMemoryStream* pStream = new CMemoryStream(unSize);
		if (!pStream)
			return;

		for (unsigned int unPos = 0; unPos < unSize; unPos++)
			pStream->WriteChar(unAlpha);

		LoadSMask(pStream, unWidth, unHeight);
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
    void CImageDict::LoadBW(NSImages::CPixJbig2* pPix, unsigned int unWidth, unsigned int unHeight)
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
    void CImageDict::LoadMask(NSImages::CPixJbig2* pPix, unsigned int unWidth, unsigned int unHeight)
	{
		CImageDict* pMask = new CImageDict(m_pXref, m_pDocument);
		if (!pMask)
			return;

		pMask->SetStream(m_pXref, new CMemoryStream());
		CJbig2Global* pJbig2Global = m_pDocument->GetJbig2Global();
		pJbig2Global->AddImage(pPix, pMask->GetStream());

		pMask->Add("Type", "XObject");
		pMask->Add("Subtype", "Image");
		pMask->Add("Width", unWidth);
		pMask->Add("Height", unHeight);
		pMask->Add("BitsPerComponent", 1);
		pMask->Add("ImageMask", true);
		pMask->SetFilter(STREAM_FILTER_JBIG2_DECODE);

		CArrayObject* pDecodeParams = new CArrayObject();
		CDictObject* pParams = new CDictObject();
		pDecodeParams->Add(pParams);
		pParams->Add("JBIG2Globals", pJbig2Global);
		pMask->Add("DecodeParms", pDecodeParams);

		Add("Mask", pMask);
	}
	void CImageDict::AddTransparency(const BYTE& unAlpha)
	{
		if (CheckSMask())
		{
			CImageDict* pSMask = (CImageDict*)Get("SMask");
			CStream* pMaskStream = pSMask->GetStream();

			pMaskStream->Seek(0, EWhenceMode::SeekEnd);
			int nSize = pMaskStream->Tell();
			if (nSize <= 0)
				return;

			CMemoryStream* pStream = new CMemoryStream(nSize);
			if (!pStream)
				return;

			double dKoef = unAlpha / 255.0;

			pMaskStream->Seek(0, EWhenceMode::SeekSet);
			while (!pMaskStream->IsEof())
			{
				BYTE nChar = pMaskStream->ReadUChar();
				pStream->WriteChar((BYTE)(nChar * dKoef));
			}

			pSMask->SetStream(m_pXref, pStream);
		}
		else
		{
			LoadSMask(unAlpha, GetWidth(), GetHeight());
		}
	}
	unsigned int CImageDict::GetWidth() const
	{
		return ((unsigned int)((CNumberObject*)this->Get("Width"))->Get());
	}
	unsigned int CImageDict::GetHeight() const
	{
		return ((unsigned int)((CNumberObject*)this->Get("Height"))->Get());
	}
	bool CImageDict::CheckSMask()
	{
		CImageDict* pSMask = (CImageDict*)this->Get("SMask");
		if (!pSMask)
			return false;

		CStream* pMaskStream = pSMask->GetStream();
		if (!pMaskStream)
			return false;

		CNumberObject* pBits = (CNumberObject*)pSMask->Get("BitsPerComponent");
		if (!pBits || 8 != pBits->Get())
			return false;

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CJbig2Global
	//----------------------------------------------------------------------------------------
	CJbig2Global::CJbig2Global(CXref* pXref) : CDictObject(pXref)
	{
		m_pXref = pXref;
        m_pContext.Init(0.85, 0.5, -1, -1, false, -1);
	}
	CJbig2Global::~CJbig2Global()
	{
        m_pContext.Destroy();
	}
	void CJbig2Global::FlushStreams()
	{
		CStream* pStream = GetStream();

		int nLen = 0;
        BYTE* pBuffer = m_pContext.PagesComplete(&nLen);

		if (pBuffer)
		{
			pStream->Write(pBuffer, nLen);
			free(pBuffer);
		}

		for (int nIndex = 0, nCount = m_vImages.size(); nIndex < nCount; nIndex++)
		{
            pBuffer = m_pContext.ProducePage(nIndex, -1, -1, &nLen);
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

        m_pContext.Destroy();
	}
	void CJbig2Global::AddImage(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride, CStream* pImageStream)
	{
        if (!m_pContext.IsInit())
			return;

        NSImages::CPixJbig2 pPix;
        if (!pPix.Create(unWidth, unHeight, 1))
			return;

		BYTE* pLine = (BYTE*)pImage;
		for (unsigned int unY = 0; unY < unHeight; unY++, pLine += unStride)
		{
			char nBit = 0;
			BYTE* pCur = pLine;
			for (unsigned int unX = 0; unX < unWidth; unX++)
			{
                pPix.SetPixel(unX, unY, pCur[0] & (1 << nBit));
				nBit++;

				if (8 == nBit)
				{
					nBit = 0;
					pCur++;
				}
			}
		}

        m_pContext.AddPage(&pPix);
        pPix.Destroy();
		m_vImages.push_back(pImageStream);
	}
    void CJbig2Global::AddImage(NSImages::CPixJbig2* pPix, CStream* pImageStream)
	{
        if (!m_pContext.IsInit())
			return;

        m_pContext.AddPage(pPix);
		m_vImages.push_back(pImageStream);
	}
	int  CJbig2Global::GetImagesCount()
	{
		return m_vImages.size();
	}
}
