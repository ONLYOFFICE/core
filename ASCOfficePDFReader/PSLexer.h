#ifndef _PS_LEXER_H
#define _PS_LEXER_H

//------------------------------------------------------------------------

class PSLexer 
{
public:

	PSLexer(int (*GetCharFunc)(void *), void *pData);
	~PSLexer();

	// Некоторый аналог функции Lexer::GetObject. Но для чтения PS вставок в PDF файле.
	// Здесь читаются данные заключенные между скобок "<>", "()" или какая-нибудь команда.
	// Данные в квадратных скобках "[]" не читаются.
	BOOL GetToken(char *sBuffer, int nSize, int *pnLength);

private:

	int LookChar();
	int GetChar();

private:

	int (*GetCharFunction)(void *); // Указатель на функцию для считывания символа
	void *m_pData;
	int   m_nCharBuffer;
};

#endif /* _PS_LEXER_H */
