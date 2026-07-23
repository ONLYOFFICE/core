/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PDF_WRITER_SRC_IMAGE_H
#define _PDF_WRITER_SRC_IMAGE_H

#include "Objects.h"
#include "../../DesktopEditor/graphics/pro/Image.h"

namespace PdfWriter
{
	class CDocument;
	class CMemoryStream;
	class CXObject : public CDictObject
	{
	private:
		double m_dOriginW;
		double m_dOriginH;
		std::string m_sName;
	public:
		CXObject();
		EDictType GetDictType() const
		{
			return dict_type_XOBJECT;
		}
		void SetWidth (double dW) { m_dOriginW = dW; }
		void SetHeight(double dH) { m_dOriginH = dH; }
		void SetName(const std::string& sName) { m_sName = sName; }

		double GetWidth()  { return m_dOriginW; }
		double GetHeight() { return m_dOriginH; }
		const std::string& GetName() { return m_sName; }
	};
	//----------------------------------------------------------------------------------------
	// CImageDict
	//----------------------------------------------------------------------------------------
	class CImageDict : public CXObject
	{
	public:

		CImageDict(CXref* pXref, CDocument* pDocument);

        void LoadJpeg(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight, bool bGrayScale = false);
        void LoadJpeg(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight, bool bGrayScale = false);
		void LoadJpx(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadJpx(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight);
		void LoadJb2(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadCCITT4(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadRaw(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight);
		void LoadRaw(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight);
		void LoadSMask(CMemoryStream* pStream, const unsigned int unWidth, const unsigned int& unHeight);
		void LoadSMask(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight, unsigned char lAlpha = 255, bool bVerFlip = false);		
		void LoadSMask(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight);
		void LoadSMask(const BYTE& unAlpha, const unsigned int& unWidth, const unsigned int& unHeight);
		void LoadBW(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride);
        void LoadBW(NSImages::CPixJbig2* pPix, unsigned int unWidth, unsigned int unHeight);
        void LoadMask(NSImages::CPixJbig2* pPix, unsigned int unWidth, unsigned int unHeight);
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		void AddTransparency(const BYTE& unAlpha);

	private:

		bool CheckSMask();

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
        void AddImage(NSImages::CPixJbig2* pPix, CStream* pImageStream);
		void FlushStreams();
		int  GetImagesCount();

	private:

        CXref*                  m_pXref;
        NSImages::CJbig2Context m_pContext;
        std::vector<CStream*>   m_vImages;
	};
}

#endif // _PDF_WRITER_SRC_IMAGE_H
