#ifndef _PDF_READER_PS_LEXER_H
#define _PDF_READER_PS_LEXER_H

namespace PdfReader
{
	class PSLexer
	{
	public:

		PSLexer(int(*GetCharFunc)(void *), void *pData);
		~PSLexer();

		// Некоторый аналог функции Lexer::GetObject. Но для чтения PS вставок в PDF файле.
		// Здесь читаются данные заключенные между скобок "<>", "()" или какая-нибудь команда.
		// Данные в квадратных скобках "[]" не читаются.
		bool GetToken(char *sBuffer, int nSize, int *pnLength);

	private:

		int LookChar();
		int GetChar();

	private:

		int(*GetCharFunction)(void *); // Указатель на функцию для считывания символа
		void *m_pData;
		int   m_nCharBuffer;
	};
}

#endif // _PDF_READER_PS_LEXER_H
