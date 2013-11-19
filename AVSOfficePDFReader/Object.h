#ifndef _OBJECT_H
#define _OBJECT_H

#include <stdio.h>
#include <string.h>
#include "MemoryUtils.h"
#include "StringExt.h"

class XRef;
class Array;
class Dict;
class Stream;

//------------------------------------------------------------------------
// Ref
//------------------------------------------------------------------------

struct Ref 
{
	int nNum;    // Номер объекта
	int nGen;    // Номер версии объекта(generation number)

public:

	bool operator ==(const Ref& oRight)const
	{
		if ( nNum == oRight.nNum && nGen == oRight.nGen )
			return true;

		return false;
	}
 };

//------------------------------------------------------------------------
// Типы объектов
//------------------------------------------------------------------------

enum ObjType 
{
	// Простые объекты
	objBool,         // boolean
	objInt,          // integer
	objReal,         // real
	objString,       // string
	objName,         // name
	objNull,         // null

	// составные объекты
	objArray,        // array
	objDict,         // dictionary
	objStream,       // stream
	objRef,          // indirect reference

	// специальные объекты
	objCommand,      // command name
	objError,        // error return from Lexer
	objEOF,          // end of file return from Lexer
	objNone          // uninitialized object
};

#define NumObjTypes 14

//------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------

class Object 
{
public:

	Object():
	m_eType(objNone)
	{
	}


	Object *InitBool(BOOL bBool)
    { 
		m_eType = objBool; 
		m_uBool = bBool; 
		return this; 
	}
	Object *InitInt(int nInt)
    { 
		m_eType = objInt; 
		m_uInt  = nInt; 
		return this; 
	}
	Object *InitReal(double dReal)
    { 
		m_eType = objReal; 
		m_uReal = dReal; 
		return this; 
	}
	Object *InitString(StringExt *seString)
    { 
		m_eType      = objString; 
		m_uStringExt = seString; 
		return this; 
	}
	Object *InitName(char *sName)
    { 
		m_eType = objName; 
		m_uName = CopyString(sName); 
		return this; 
	}
	Object *InitNull()
    { 
		m_eType = objNull; 
		return this; 
	}
	Object *InitArray(XRef *pXref);
	Object *InitDict(XRef *pXref);
	Object *InitDict(Dict *pDict);
	Object *InitStream(Stream *pStream);
	Object *InitRef(int nNum, int nGen)
	{ 
		m_eType = objRef; 
		m_uRef.nNum = nNum; 
		m_uRef.nGen = nGen; 
		return this; 
	}
	Object *InitCommand(char *sCommand)
    { 
		m_eType    = objCommand; 
		m_uCommand = CopyString(sCommand); 
		return this; 
	}
	Object *InitError()
    { 
		m_eType = objError; 
		return this; 
	}
	Object *InitEOF()
    { 
		m_eType = objEOF; 
		return this; 
	}


	// Копируем объект.
	Object *Copy(Object *pObject);

	// Если объект - сслка, тогда получаес объект по ссылку, в противном случае копируем данный объект.
	Object *Fetch(XRef *pXref, Object *pObject);

	// 
	void Free();

	// Проверка тип объекта.
	ObjType GetType() 
	{ 
		return m_eType; 
	}
	BOOL IsBool() 
	{ 
		return ( m_eType == objBool ); 
	}
	BOOL IsInt() 
	{ 
		return ( m_eType == objInt ); 
	}
	BOOL IsReal() 
	{ 
		return ( m_eType == objReal ); 
	}
	BOOL IsNum() 
	{ 
		return ( m_eType == objInt || m_eType == objReal ); 
	}
	BOOL IsString() 
	{ 
		return ( m_eType == objString ); 
	}
	BOOL IsName() 
	{ 
		return ( m_eType == objName ); 
	}
	BOOL IsNull() 
	{ 
		return ( m_eType == objNull ); 
	}
	BOOL IsArray() 
	{ 
		return ( m_eType == objArray ); 
	}
	BOOL IsDict() 
	{ 
		return ( m_eType == objDict ); 
	}
	BOOL IsStream() 
	{ 
		return ( m_eType == objStream ); 
	}
	BOOL IsRef() 
	{ 
		return ( m_eType == objRef ); 
	}
	BOOL IsCommand() 
	{ 
		return ( m_eType == objCommand ); 
	}
	BOOL IsError() 
	{ 
		return ( m_eType == objError ); 
	}
	BOOL IsEOF() 
	{ 
		return ( m_eType == objEOF ); 
	}
	BOOL IsNone() 
	{ 
		return ( m_eType == objNone ); 
	}

	// Сепциальные проверки.
	BOOL IsName(char *sName)
    { 
		return ( m_eType == objName && !strcmp( m_uName, sName ) ); 
	}
	BOOL IsDict(char *sDictType);
	BOOL IsStream(char *sDictType);
	BOOL IsCommand(char *sCommand)
    { 
		return ( m_eType == objCommand && !strcmp( m_uCommand, sCommand ) ); 
	}
	// В следующих функциях предполагается, что тип объекта корректный.
	// Т.е. соответствует типу запрашиваемого значения.
	BOOL       GetBool() 
	{ 
		return m_uBool; 
	}
	int        GetInt() 
	{
		return m_uInt; 
	}
	double     GetReal() 
	{ 
		return m_uReal; 
	}
	double     GetNum() 
	{ 
		return ( m_eType == objInt ) ? (double)m_uInt : m_uReal; 
	}
	StringExt *GetString() 
	{ 
		return m_uStringExt; 
	}
	char      *GetName() 
	{ 
		return m_uName; 
	}
	Array     *GetArray() 
	{ 
		return m_uArray; 
	}
	Dict      *GetDict() 
	{ 
		return m_uDict; 
	}
	Stream    *GetStream() 
	{ 
		return m_uStream; 
	}
	Ref        GetRef() 
	{ 
		return m_uRef; 
	}
	int        GetRefNum() 
	{ 
		return m_uRef.nNum; 
	}
	int        GetRefGen() 
	{ 
		return m_uRef.nGen; 
	}
	char      *GetCommand() 
	{ 
		return m_uCommand; 
	}


	// Array 
	int     ArrayGetLength();
	void    ArrayAdd(Object *pItem);
	Object *ArrayGet(int nIndex, Object *pObject);
	Object *ArrayGetCopy(int nIndex, Object *pObject);

	// Dict
	int     DictGetLength();
	void    DictAdd(char *sKey, Object *pItem);
	BOOL    DictCheckType(char *sDictType);
	Object *DictLookup(char *sKey, Object *pObject);
	Object *DictLookupAndCopy(char *sKey, Object *pObject);
	char   *DictGetKey(int nIndex);
	Object *DictGetValue(int nIndex, Object *pObject);
	Object *DictGetValueCopy(int nIndex, Object *pObject);

	// Stream
	BOOL         StreamCheckType(char *sDictType);
	void         StreamReset();
	void         StreamClose();
	int          StreamGetChar();
	int          StreamLookChar();
	char        *StreamGetLine(char *sBuffer, int nSize);
	unsigned int StreamGetPos();
	void         StreamSetPos(unsigned int unPos, int nDirection = 0);
	Dict        *StreamGetDict();

	// Для записи
	char *GetTypeName();
	void Print(FILE *pFile = stdout);

private:

	ObjType m_eType;  // тип объекта
	union 
	{
		BOOL       m_uBool;       // boolean
		int        m_uInt;        // integer
		double     m_uReal;       // real
		StringExt *m_uStringExt;  // StringExt
		char      *m_uName;       // имя
		Array     *m_uArray;      // массив
		Dict      *m_uDict;       // словарь
		Stream    *m_uStream;     // поток
		Ref        m_uRef;        // ссылка
		char      *m_uCommand;    // команда
	};
};

//------------------------------------------------------------------------
// Array accessors.
//------------------------------------------------------------------------

#include "Array.h"

inline int     Object::ArrayGetLength()
{ 
	return m_uArray->GetCount(); 
}

inline void    Object::ArrayAdd(Object *pItem)
{ 
	m_uArray->Add( pItem ); 
}

inline Object *Object::ArrayGet(int nIndex, Object *pObject)
{ 
	return m_uArray->Get( nIndex, pObject); 
}

inline Object *Object::ArrayGetCopy(int nIndex, Object *pObject)
{ 
	return m_uArray->GetCopy( nIndex, pObject); 
}

//------------------------------------------------------------------------
// Dict accessors.
//------------------------------------------------------------------------

#include "Dict.h"

inline int     Object::DictGetLength()
{ 
	return m_uDict->GetEntryCount(); 
}

inline void    Object::DictAdd(char *sKey, Object *pItem)
{ 
	m_uDict->AddItem( sKey, pItem); 
}

inline BOOL    Object::DictCheckType(char *sDictType)
{ 
	return m_uDict->CheckType( sDictType ); 
}

inline BOOL    Object::IsDict(char *sDictType)
{ 
	return ( m_eType == objDict && DictCheckType( sDictType ) ); 
}

inline Object *Object::DictLookup(char *sKey, Object *pObject)
{ 
	return m_uDict->Search( sKey, pObject); 
}

inline Object *Object::DictLookupAndCopy(char *sKey, Object *pObject)
{ 
	return m_uDict->SearchAndCopy( sKey, pObject); 
}

inline char   *Object::DictGetKey(int nIndex)
{ 
	return m_uDict->GetKey(nIndex); 
}

inline Object *Object::DictGetValue(int nIndex, Object *pObject)
{ 
	return m_uDict->GetValue( nIndex, pObject); 
}

inline Object *Object::DictGetValueCopy(int nIndex, Object *pObject)
{ 
	return m_uDict->GetValueCopy( nIndex, pObject); 
}

//------------------------------------------------------------------------
// Stream accessors.
//------------------------------------------------------------------------

#include "Stream.h"

inline BOOL         Object::StreamCheckType(char *sDictType)
{ 
	return m_uStream->GetDict()->CheckType( sDictType ); 
}

inline BOOL         Object::IsStream(char *sDictType)
{ 
	return ( m_eType == objStream && StreamCheckType( sDictType ) ); 
}

inline void         Object::StreamReset()
{ 
	m_uStream->Reset(); 
}

inline void         Object::StreamClose()
{ 
	m_uStream->Close(); 
}

inline int          Object::StreamGetChar()
{ 
	return m_uStream->GetChar(); 
}

inline int          Object::StreamLookChar()
{ 
	return m_uStream->LookChar(); 
}

inline char        *Object::StreamGetLine(char *sBuffer, int nSize)
{ 
	return m_uStream->GetLine( sBuffer, nSize); 
}

inline unsigned int Object::StreamGetPos()
{ 
	return m_uStream->GetPos(); 
}

inline void         Object::StreamSetPos(unsigned int unPos, int nDirection)
{ 
	m_uStream->SetPos( unPos, nDirection); 
}

inline Dict        *Object::StreamGetDict()
{ 
	return m_uStream->GetDict(); 
}

#endif /* _OBJECT_H */
