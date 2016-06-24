/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		void LoadJpeg(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight);
		void LoadJpx(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadJpx(BYTE* pBuffer, int nBufferSize, unsigned int unWidth, unsigned int unHeight);
		void LoadJb2(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadCCITT4(const wchar_t* wsTempFile, unsigned int unWidth, unsigned int unHeight);
		void LoadRaw(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight);
		void LoadRaw(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight);
		void LoadSMask(const BYTE* pBgra, unsigned int unWidth, unsigned int unHeight, unsigned char lAlpha = 255, bool bVerFlip = false);
		void LoadSMask(const BYTE* pBuffer, unsigned int unSize, unsigned int unWidth, unsigned int unHeight);
		void LoadBW(const BYTE* pImage, unsigned int unWidth, unsigned int unHeight, unsigned int unStride);
		void LoadBW(Pix* pPix, unsigned int unWidth, unsigned int unHeight);
		void LoadMask(Pix* pPix, unsigned int unWidth, unsigned int unHeight);

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