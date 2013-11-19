#include "stdafx.h"

#include <stddef.h>
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Stream.h"
#include "XRef.h"

//------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------

char *c_sObjectsTypeNames[NumObjTypes] = 
{
	"boolean",
	"integer",
	"real",
	"string",
	"name",
	"null",
	"array",
	"dictionary",
	"stream",
	"ref",
	"cmd",
	"error",
	"eof",
	"none"
};

Object *Object::InitArray(XRef *pXref) 
{
	m_eType  = objArray;
	m_uArray = new Array(pXref);
	return this;
}

Object *Object::InitDict(XRef *pXref) 
{
	m_eType = objDict;
	m_uDict = new Dict(pXref);
	return this;
}

Object *Object::InitDict(Dict *pDict)
{
	m_eType = objDict;
	m_uDict = pDict;
	m_uDict->AddRef();
	return this;
}

Object *Object::InitStream(Stream *pStream) 
{
	m_eType = objStream;
	m_uStream = pStream;
	return this;
}

Object *Object::Copy(Object *pObject) 
{
	*pObject = *this;
	switch (m_eType)
	{
	case objString:
		pObject->m_uStringExt = m_uStringExt->Copy();
		break;
	case objName:
		pObject->m_uName = CopyString(m_uName);
		break;
	case objArray:
		m_uArray->AddRef();
		break;
	case objDict:
		m_uDict->AddRef();
		break;
	case objStream:
		m_uStream->AddRef();
		break;
	case objCommand:
		pObject->m_uCommand = CopyString(m_uCommand);
		break;
	default:
		break;
	}
	return pObject;
}

Object *Object::Fetch(XRef *pXref, Object *pObject) 
{
	return ( m_eType == objRef && pXref ) ? pXref->Fetch(m_uRef.nNum, m_uRef.nGen, pObject) : Copy(pObject);
}

void Object::Free() 
{
	switch (m_eType) 
	{
	case objString:
		delete m_uStringExt;
		break;
	case objName:
		MemUtilsFree(m_uName);
		break;
	case objArray:
		if ( !m_uArray->Release() ) 
		{
			delete m_uArray;
		}
		break;
	case objDict:
		if ( !m_uDict->Release() ) 
		{
			delete m_uDict;
		}
		break;
	case objStream:
		if ( !m_uStream->Release() ) 
		{
			delete m_uStream;
		}
		break;
	case objCommand:
		MemUtilsFree(m_uCommand);
		break;
	default:
		break;
	}
	m_eType = objNone;
}

char *Object::GetTypeName() 
{
	return c_sObjectsTypeNames[m_eType];
}

void Object::Print(FILE *pFile) 
{
	Object oTemp;

	switch (m_eType) 
	{
	case objBool:
		fprintf( pFile, "%s", m_uBool ? "true" : "false");
		break;
	case objInt:
		fprintf( pFile, "%d", m_uInt);
		break;
	case objReal:
		fprintf( pFile, "%g", m_uReal);
		break;
	case objString:
		fprintf( pFile, "(");
		fwrite(m_uStringExt->GetBuffer(), 1, m_uStringExt->GetLength(), pFile);
		fprintf( pFile, ")");
	break;
	case objName:
		fprintf( pFile, "/%s", m_uName);
		break;
	case objNull:
		fprintf( pFile, "null");
		break;
	case objArray:
		fprintf( pFile, "[");
		for ( int nIndex = 0; nIndex < ArrayGetLength(); ++nIndex ) 
		{
			if ( nIndex > 0 )
				fprintf( pFile, " ");
			ArrayGetCopy( nIndex, &oTemp);
			oTemp.Print( pFile );
			oTemp.Free();
		}
		fprintf( pFile, "]");
		break;
	case objDict:
		fprintf( pFile, "<<");
		for ( int nIndex = 0; nIndex < DictGetLength(); ++nIndex ) 
		{
			fprintf( pFile, " /%s ", DictGetKey(nIndex) );
			DictGetValueCopy( nIndex, &oTemp);
			oTemp.Print( pFile );
			oTemp.Free();
		}
		fprintf( pFile, " >>");
		break;
	case objStream:
		fprintf( pFile, "<stream>");
		break;
	case objRef:
		fprintf( pFile, "%d %d R", m_uRef.nNum, m_uRef.nGen);
		break;
	case objCommand:
		fprintf( pFile, "%s", m_uCommand);
		break;
	case objError:
		fprintf( pFile, "<error>");
		break;
	case objEOF:
		fprintf( pFile, "<EOF>");
		break;
	case objNone:
		fprintf( pFile, "<none>");
		break;
	}
}
