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
#ifndef _PDF_READER_FONT_FILE_TYPE1_H
#define _PDF_READER_FONT_FILE_TYPE1_H

#include "FontFileBase.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CFontFileType1
	//------------------------------------------------------------------------

	class CFontFileType1 : public CFontFileBase
	{
	public:

		static CFontFileType1 *LoadFromBuffer(char *sBuffer, int nLen);
		static CFontFileType1 *LoadFromFile(wchar_t *wsFileName);

		virtual ~CFontFileType1();

		char *GetName();

		char **GetEncoding();

		void WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	private:

		CFontFileType1(char *sBuffer, int nLen, bool bFreeData);

		char *GetNextLine(char *sLine);
		void Parse();

	private:

		char  *m_sName;
		char **m_arrEncoding;
		bool   m_bParsed;
	};
}

#endif // _PDF_READER_FONT_FILE_TYPE1_H
