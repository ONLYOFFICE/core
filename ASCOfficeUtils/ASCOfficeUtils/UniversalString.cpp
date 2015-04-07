#include "stdafx.h"
#include "UniversalString.h"

void UniversalString::ClearUniversalString()
{
  this->length = 0;
  this->charSize = 0;
  this->wcharSize = 0;
		
  if ( this->str != NULL )
  {
    delete [](this->str);
	this->str = NULL;
  }

  if ( this->wstr != NULL )
  {
    delete [](this->wstr);
	this->wstr = NULL;
  }
}

/*========================================================================================================*/

UniversalString::UniversalString():
str(NULL), wstr(NULL), length(0), charSize(1), wcharSize(1)
{
  this->str = new char[this->charSize];
  this->str[0] = '\0';
	  
  this->wstr = new wchar_t[this->wcharSize];
  this->wstr[0] = '\0';
}

/*========================================================================================================*/

UniversalString::UniversalString( const UniversalString& _ustr ):
str(NULL), wstr(NULL), length(_ustr.length), charSize(_ustr.charSize), wcharSize(_ustr.wcharSize)
{
  if ( ( this->charSize > 0 ) && ( this->wcharSize > 0 ) )
  {
    this->str = new char[this->charSize];
	this->wstr = new wchar_t[this->wcharSize];

	if ( ( this->str != NULL ) && ( this->wstr != NULL ) )
	{
	  memset( this->str, 0, ( sizeof(char) * this->charSize ) );
	  memset( this->wstr, 0, ( sizeof(wchar_t) * this->wcharSize ) );
		
	  memcpy( this->str, _ustr.str, ( sizeof(char) * this->charSize ) );
	  memcpy( this->wstr, _ustr.wstr, ( sizeof(wchar_t) * this->wcharSize ) );
	}
  }
}

/*========================================================================================================*/

UniversalString::UniversalString( const char* _str, unsigned int CodePage ):
str(NULL), wstr(NULL), length(0), charSize(0), wcharSize(0)
{
  if ( _str != NULL )
  {
	this->length = (unsigned int)strlen( _str );
	this->charSize = ( this->length + 1 );
	this->str = new char[this->charSize];

	if ( this->str != NULL )
	{
	  memset( this->str, 0, ( sizeof(char) * this->charSize ) );
	  memcpy( this->str, _str, ( sizeof(char) * this->charSize ) );

	  this->wcharSize = MultiByteToWideChar( CodePage, 0, this->str, -1, NULL, 0 );
	  this->wstr = new wchar_t[this->wcharSize];

	  if ( this->wstr != NULL )
	  {
		memset( this->wstr, 0, ( sizeof(wchar_t) * this->wcharSize ) );
	    MultiByteToWideChar( CodePage, 0, this->str, -1, this->wstr, this->wcharSize );
	  }
	}
  }
}

/*========================================================================================================*/

UniversalString::UniversalString( const wchar_t* _wstr, unsigned int CodePage ):
str(NULL), wstr(NULL), length(0), charSize(0), wcharSize(0)
{
  if ( _wstr != NULL )
  {
    this->length = (unsigned int)wcslen( _wstr );
	this->wcharSize = ( this->length + 1 );
	this->wstr = new wchar_t[this->wcharSize];

	if ( this->wstr != NULL )
	{
      memset( this->wstr, 0, ( sizeof(wchar_t) * this->wcharSize ) );
	  memcpy( this->wstr, _wstr, ( sizeof(wchar_t) * this->wcharSize ) );

      this->charSize = WideCharToMultiByte( CodePage, 0, this->wstr, -1, NULL, 0, NULL, NULL );
	  this->str = new char[this->charSize];

	  if ( this->str != NULL )
	  {
	    memset( this->str, 0, ( sizeof(char) * this->charSize ) );
		WideCharToMultiByte( CodePage, 0, this->wstr, -1, this->str, this->charSize, NULL, NULL );
	  }
	}
  }
}

/*========================================================================================================*/

UniversalString::~UniversalString()
{
  this->ClearUniversalString();
}

/*========================================================================================================*/

UniversalString::operator char* ()
{
  return this->str;
}

/*========================================================================================================*/

UniversalString::operator wchar_t* ()
{
  return this->wstr;
}

/*========================================================================================================*/

bool UniversalString::operator == ( const UniversalString& _ustr )
{
  if ( ( this->length == _ustr.length ) && ( this->charSize == _ustr.charSize ) && ( this->wcharSize == _ustr.wcharSize ) &&
       ( strncmp( this->str, _ustr.str, _ustr.charSize ) == 0 ) && ( wcsncmp( this->wstr, _ustr.wstr, _ustr.wcharSize ) == 0 ) )
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*========================================================================================================*/

bool UniversalString::operator != ( const UniversalString& _ustr )
{
  return !(this->operator == ( _ustr ));
}

/*========================================================================================================*/

UniversalString& UniversalString::operator = ( const UniversalString& _ustr )
{
  if ( this != &_ustr )
  {	
    this->ClearUniversalString();

	this->length = _ustr.length;
	this->charSize = _ustr.charSize;
	this->wcharSize = _ustr.wcharSize;

	if ( ( this->charSize > 0 ) && ( this->wcharSize > 0 ) )
	{
      this->str = new char[this->charSize];
	  this->wstr = new wchar_t[this->wcharSize];

	  if ( ( this->str != NULL ) && ( this->wstr != NULL ) )
	  {
	    memset( this->str, 0, ( sizeof(char) * this->charSize ) );
		memset( this->wstr, 0, ( sizeof(wchar_t) * this->wcharSize ) );
		  
		memcpy( this->str, _ustr.str, ( sizeof(char) * this->charSize ) );
	    memcpy( this->wstr, _ustr.wstr, ( sizeof(wchar_t) * this->wcharSize ) );
	  }
	}
  }
	  
  return *this;
}

/*========================================================================================================*/

unsigned int UniversalString::GetLength() const
{
  return this->length;
}