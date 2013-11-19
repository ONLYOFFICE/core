#ifndef _NAME_TO_CHAR_CODE_H
#define _NAME_TO_CHAR_CODE_H

#include "CharTypes.h"

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

#endif /* _NAME_TO_CHAR_CODE_H */
