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
#ifndef _PDF_WRITER_SRC_TTWRITER_H
#define _PDF_WRITER_SRC_TTWRITER_H

#include "Streams.h"

namespace PdfWriter
{
	struct TrueTypeTable
	{
		unsigned int unTag;
		unsigned int unChecksum;
		int          nOffset;
		int          nOrigOffset;
		int          nLen;
	};
	struct TrueTypeCmap
	{
		int nPlatform;
		int nEncoding;
		int nOffset;
		int nLen;
		int nFormat;
	};
	//----------------------------------------------------------------------------------------
	// CFontFileBase
	//----------------------------------------------------------------------------------------
	class CFontFileBase
	{
	public:

		virtual ~CFontFileBase();

	protected:

		CFontFileBase(char *sFile, int nLen, bool bFreeFileData);

		static char *ReadFile(const std::wstring & wsFileName, int *pnFileLen);
		int          GetS8(int nPos, bool *pbSuccess);
		int          GetU8(int nPos, bool *pbSuccess);
		int          GetS16BE(int nPos, bool *pbSuccess);
		int          GetU16BE(int nPos, bool *pbSuccess);
		int          GetS32BE(int nPos, bool *pbSuccess);
		unsigned int GetU32BE(int nPos, bool *pbSuccess);
		unsigned int GetUVarBE(int nPos, int nSize, bool *pbSuccess);
		bool         CheckRegion(int nPos, int nSize);

	protected:

		unsigned char *m_sFileData;
		unsigned char *m_sFile;
		int            m_nLen;
		bool           m_bFreeFileData;

	};
	//----------------------------------------------------------------------------------------
	// CFontFileTrueType
	//----------------------------------------------------------------------------------------
	class CFontFileTrueType : public CFontFileBase
	{
	public:

		static CFontFileTrueType *LoadFromBuffer(char* sBuffer, int nLen, unsigned int unIndex);
		static CFontFileTrueType *LoadFromFile(const std::wstring& wsFileName, unsigned int unIndex);
		~CFontFileTrueType();

		// Write TrueTypeFont File, filling in missing tables and correcting
		// various errors. If <sName> parameter is specified, the 'name' table
		// is rewritten in the font. If <pCodeToGID> parameter is specified,
		// the 'cmap' table is rewritten in the font.
		void WriteTTF(CStream* pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL, unsigned int unCodesCount = 0, unsigned char *pUseGlyfs = NULL, long lGlyfsCount = 0);
		void WriteOTF(CStream* pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL);
		void WriteCIDFontType0C(CStream* pOutputStream, unsigned short* pCodeToGID = NULL, unsigned int unCodesCount = 0);

		int  GetAscent();
		int  GetDescent();
		int  GetCapHeight();
		int* GetBBox();
		int  GetWeight();
		bool GetOpenTypeCFF();

		void SetName(const std::string& sName);

	private:

		CFontFileTrueType(char *sBuffer, int nLen, bool bFreeFileData, unsigned int unFontIndex);

		unsigned int ComputeTableChecksum(unsigned char *sData, int nLength);
		void Parse();
        int  SeekTable(const char *sTag);
		void ReadOS2();

	private:

		unsigned int   m_unFontIndex;
		TrueTypeTable* m_pTables;
		int            m_nTablesCount;
		TrueTypeCmap*  m_pCMaps;
		int            m_nCMapsCount;
		int            m_nGlyphs;
		int            m_nLocaFormat;
		int            m_arrBBox[4];
		bool           m_bOpenTypeCFF;
					   
		int            m_nAscent;
		int            m_nDescent;
		int            m_nCapHeight;
		int            m_nWeight;
		std::string    m_sName;
					   
		bool           m_bSuccess;
	};
}

#endif // _PDF_WRITER_SRC_TTWRITER_H
