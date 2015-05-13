#ifndef _PDF_READER_NAME_TO_CHAR_CODE_H
#define _PDF_READER_NAME_TO_CHAR_CODE_H

#include "CharTypes.h"

namespace PdfReader
{
	struct NameToCharCodeEntry;

	//-------------------------------------------------------------------------------------------------------------------------------
	// NameToCharCode
	//-------------------------------------------------------------------------------------------------------------------------------

	class NameToCharCode
	{
	public:

		NameToCharCode();
		~NameToCharCode();

		void Add(char *sName, CharCode nCode);
		CharCode Lookup(char *sName);

	private:

		int Hash(char *sName);

	private:

		NameToCharCodeEntry *m_pTable;
		int                  m_nSize;
		int                  m_nLen;
	};
}

#endif // _PDF_READER_NAME_TO_CHAR_CODE_H
