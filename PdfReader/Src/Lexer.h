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
