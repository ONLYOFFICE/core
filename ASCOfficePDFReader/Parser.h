#ifndef _PARSER_H
#define _PARSER_H

#include "Lexer.h"

//------------------------------------------------------------------------
// Parser
//------------------------------------------------------------------------

class Parser 
{
public:

	Parser(XRef *pXref, Lexer *pLexer, BOOL bAllowStreams);

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
	BOOL   m_bAllowStreams; // Будем ли парсить Stream Objects?
	Object m_oBuffer1;      // Два следующих объекта
	Object m_oBuffer2;      // 
	int    m_nInlineImage;  // Устанавливаем, когда сталкиваемся с Inline Image
};

#endif /* _PARSER_H */

