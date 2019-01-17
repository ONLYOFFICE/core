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
#ifndef _PDF_READER_PARSER_H
#define _PDF_READER_PARSER_H

#include "Lexer.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Parser
	//------------------------------------------------------------------------

	class Parser
	{
	public:

		Parser(XRef *pXref, Lexer *pLexer, bool bAllowStreams);

		~Parser();

		// Считываем следующий объект из потока.
		Object *GetObject(Object *pObject, unsigned char *sDecryptKey = NULL, CryptAlgorithm eEncryptAlgorithm = cryptRC4, int nKeyLength = 0, int nObjectNum = 0, int nObjectGen = 0);

		Stream *GetStream()
		{
			return m_pLexer->GetStream();
		}

		int     GetPos()
		{
			return m_pLexer->GetPos();
		}

	private:

		Stream *CreateStream(Object *pDict, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen);
		void Shift();

	private:

		XRef  *m_pXref;         // Таблица Xref для даннthe xref table for this PDF file
		Lexer *m_pLexer;        // Входящий поток
		bool   m_bAllowStreams; // Будем ли парсить Stream Objects?
		Object m_oBuffer1;      // Два следующих объекта
		Object m_oBuffer2;      // 
		int    m_nInlineImage;  // Устанавливаем, когда сталкиваемся с Inline Image
	};
}

#endif // _PDF_READER_PARSER_H

