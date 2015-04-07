#pragma once

#include <string.h>
#include <stdlib.h>

class UniversalString
{
  private:
    char *str;
	wchar_t *wstr;
	unsigned int length;
	unsigned int charSize;
	unsigned int wcharSize;

	void ClearUniversalString();
	
  public:
  	UniversalString();
	UniversalString( const UniversalString& _ustr );
	UniversalString( const char* _str, unsigned int CodePage = CP_ACP);
	UniversalString( const wchar_t* _wstr, unsigned int CodePage = CP_ACP);
	~UniversalString();
	operator char* ();
	operator wchar_t* ();
	bool operator == ( const UniversalString& _ustr );
	bool operator != ( const UniversalString& _ustr );
	UniversalString& operator = ( const UniversalString& _ustr );
	unsigned int GetLength() const;
};