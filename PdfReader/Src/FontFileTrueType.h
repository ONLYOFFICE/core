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
#ifndef _PDF_READER_FONT_FILE_TRUETYPE_H
#define _PDF_READER_FONT_FILE_TRUETYPE_H

#include "FontFileBase.h"

namespace PdfReader
{
	class StringExt;
	class CHash;
	struct TrueTypeTable;
	struct TrueTypeCmap;

	//------------------------------------------------------------------------
	// CFontFileTrueType
	//------------------------------------------------------------------------

	class CFontFileTrueType : public CFontFileBase
	{
	public:

		// Создаем объект TTF из буфера.
		static CFontFileTrueType *LoadFromBuffer(char *sBuffer, int lenA);

		// Создаем объект TTF из файла.
		static CFontFileTrueType *LoadFromFile(wchar_t *wsFileName);

		virtual ~CFontFileTrueType();

		// true, если данный OpenType фонт содержите данные формата CFF.
		// false,если это TrueType фонт ( или OpenType фонт с данными в формате TrueType).
		bool IsOpenTypeCFF()
		{
			return m_bOpenTypeCFF;
		}

		int  GetCmapsCount();

		int  GetCmapPlatform(int nIndex);

		int  GetCmapEncoding(int nIndex);

		int  FindCmap(int nPlatform, int nEncoding);

		// Возвращает GID, соответствующий символу <nChar> в <nIndex>ной CMap.
		unsigned short MapCodeToGID(int nCMapIndex, int nChar);

		// Возвращает GID, соответствующий <sName> в таблице post. Возвращает 0,
		// если символа с таким именем не нашли, или таблицы post нет.
		int MapNameToGID(char *sName);

		// Возвращает карту CIDs в GIDs, и возваращет количество элементов
		// CIDs в *<pnCIDs>.  Только для CID фонтов( OpenType CFF )
		unsigned short *GetCIDToGIDMap(int *pnCIDs);

		// Лицензионные ограничения на включение фонта( в соответствие со
		// спецификацией True Type):

		// * 4: таблицы OS/2 не найдена или некорректна
		// * 3: разрешено устанавливать
		// * 2: разрешено редактировать
		// * 1: разрешено просматривать и печатать
		// * 0: ограничено лицензией
		int GetEmbeddingRestrictions();

		// Convert to a Type 42 font, suitable for embedding in a PostScript
		// file.  <psName> will be used as the PostScript font name (so we
		// don't need to depend on the 'name' table in the font).  The
		// <encoding> array specifies the mapping from char codes to names.
		// If <encoding> is NULL, the encoding is unknown or undefined.  The
		// <codeToGID> array specifies the mapping from char codes to GIDs.
		// (Not useful for OpenType CFF fonts.)
		void ToType42(char *sPSName, char **ppEncoding, unsigned short *pCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 1 font, suitable for embedding in a PostScript
		// file.  This is only useful with 8-bit fonts.  If <newEncoding> is
		// not NULL, it will be used in place of the encoding in the Type 1C
		// font.  If <ascii> is true the eexec section will be hex-encoded,
		// otherwise it will be left as binary data.  If <psName> is
		// non-NULL, it will be used as the PostScript font name.  (Only
		// useful for OpenType CFF fonts.)
		void ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 2 CIDFont, suitable for embedding in a
		// PostScript file.  <psName> will be used as the PostScript font
		// name (so we don't need to depend on the 'name' table in the
		// font).  The <cidMap> array maps CIDs to GIDs; it has <nCIDs>
		// entries.  (Not useful for OpenType CFF fonts.)
		void ToCIDType2(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 0 CIDFont, suitable for embedding in a
		// PostScript file.  <psName> will be used as the PostScript font
		// name.  (Only useful for OpenType CFF fonts.)
		void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 0 (but non-CID) composite font, suitable for
		// embedding in a PostScript file.  <psName> will be used as the
		// PostScript font name (so we don't need to depend on the 'name'
		// table in the font).  The <cidMap> array maps CIDs to GIDs; it has
		// <nCIDs> entries.  (Not useful for OpenType CFF fonts.)
		void ToType0(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Convert to a Type 0 (but non-CID) composite font, suitable for
		// embedding in a PostScript file.  <psName> will be used as the
		// PostScript font name.  (Only useful for OpenType CFF fonts.)
		void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

		// Write a clean TTF file, filling in missing tables and correcting
		// various other errors.  If <name> is non-NULL, the font is renamed
		// to <name>.  If <codeToGID> is non-NULL, the font is re-encoded,
		// using a Windows Unicode cmap.  If <name> is NULL and the font is
		// complete and correct, it will be written unmodified.  (Not useful
		// for OpenType CFF fonts.)
		void WriteTTF(FontFileOutputFunc pOutputFunc, void *pOutputStream, char *sName = NULL, unsigned short *pCodeToGID = NULL);

	private:

		CFontFileTrueType(char *sFileName, int nLen, bool bFreeFileData);

		void ConvertEncoding(char **ppEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);
		void ConvertCharStrings(char **ppEncoding, unsigned short *pnCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream);
		void ConvertSfnts(FontFileOutputFunc pOutputFunc, void *pOutputStream, StringExt *seName, bool bNeedVerticalMetrics);
		void DumpString(unsigned char *sString, int nLength, FontFileOutputFunc pOutputFunc, void *pOutputStream);
		unsigned int ComputeTableChecksum(unsigned char *sData, int nLength);
		void Parse();
		void ReadPostTable();
		int  SeekTable(char *sTag);

	private:

		TrueTypeTable   *m_pTables;
		int              m_nTablesCount;
		TrueTypeCmap    *m_pCMaps;
		int              m_nCMapsCount;
		int              m_nGlyphs;
		int              m_nLocaFormat;
		int              m_arrBBox[4];
		CHash           *m_pNameToGID;
		bool             m_bOpenTypeCFF;

		bool             m_bSuccess;
	};
}
#endif // _PDF_READER_FONT_FILE_TRUETYPE_H
