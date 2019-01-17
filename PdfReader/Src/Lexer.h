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
#ifndef _PDF_READER_LEXER_H
#define _PDF_READER_LEXER_H

#include "Object.h"
#include "Stream.h"

namespace PdfReader
{
	class XRef;

#define TokenBufferSize 128

	//------------------------------------------------------------------------
	// Lexer
	//------------------------------------------------------------------------

	class Lexer
	{
	public:

		// Конструктор для одного потока. Удаляем этот поток в деструкторе.
		Lexer(XRef *pXref, Stream *pStream);

		// Конструкор для одного потока или массива потоков.
		Lexer(XRef *pXref, Object *pObject);

		~Lexer();

		// Считваем следующий объект из потока.
		Object *GetObject(Object *obj);

		// Переходим к началу новой строки.
		void SkipToNextLine();

		// Переходим к следующему символу.
		void SkipChar()
		{
			GetChar();
		}


		Stream *GetStream()
		{
			return m_oCurStream.IsNone() ? (Stream *)NULL : m_oCurStream.GetStream();
		}

		// Текущая позиция. Используется только для сообщений об ошибке, поэтому
		// возвращаемое значение типа int, а не unsigned int.
		int GetPos()
		{
			return m_oCurStream.IsNone() ? -1 : (int)m_oCurStream.StreamGetPos();
		}


		void SetPos(unsigned int unPos, int nDir = 0)
		{
			if (!m_oCurStream.IsNone())
				m_oCurStream.StreamSetPos(unPos, nDir);
		}

		// Проверяем является ли nChar пробелом.
		static bool IsSpace(int nChar);

	private:

		int GetChar();
		int LookChar();

	private:

		Array *m_pStreams;        // Массив потоков
		int    m_nCurStreamIndex; // Номер текущего потока
		Object m_oCurStream;      // Текущий поток
		bool   m_bFreeArray;      // Должны ли мы в данном классе осовбождать массив потоков?
		char   m_sTempBuffer[TokenBufferSize];	// Buffer
	};
}

#endif // _PDF_READER_LEXER_H
