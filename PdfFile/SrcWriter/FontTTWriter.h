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
#ifndef _PDF_WRITER_SRC_TTWRITER_H
#define _PDF_WRITER_SRC_TTWRITER_H

#include "Streams.h"

namespace PdfWriter
{
	struct TrueTypeTable;
	struct TrueTypeCmap;
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

		// Записываем TrueTypeFont File, заполняя недостающие таблицы и корректируя 
		// различные ошибки. Если задан парметр <sName>, в шрифте переписываем таблицу
		// 'name'. Если задан парамтре <pCodeToGID>, тогда в шрифте переписываем
		// таблицу 'cmap'.
		void WriteTTF(CStream* pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL, unsigned int unCodesCount = 0, unsigned char *pUseGlyfs = NULL, long lGlyfsCount = 0);
		void WriteOTF(CStream* pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL);

		int  GetAscent();
		int  GetDescent();
		int  GetCapHeight();
		int* GetBBox();
		int  GetWeight();

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
					   
		bool           m_bSuccess;
	};
}

#endif // _PDF_WRITER_SRC_TTWRITER_H
