/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include <stddef.h>
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Stream.h"
#include "XRef.h"

namespace PdfReader
{
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
		return (m_eType == objRef && pXref) ? pXref->Fetch(m_uRef.nNum, m_uRef.nGen, pObject) : Copy(pObject);
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
			if (!m_uArray->Release())
			{
				delete m_uArray;
			}
			break;
			case objDict:
			if (!m_uDict->Release())
			{
				delete m_uDict;
			}
			break;
			case objStream:
			if (!m_uStream->Release())
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
			fprintf(pFile, "%s", m_uBool ? "true" : "false");
			break;
			case objInt:
			fprintf(pFile, "%d", m_uInt);
			break;
			case objReal:
			fprintf(pFile, "%g", m_uReal);
			break;
			case objString:
			fprintf(pFile, "(");
			fwrite(m_uStringExt->GetBuffer(), 1, m_uStringExt->GetLength(), pFile);
			fprintf(pFile, ")");
			break;
			case objName:
			fprintf(pFile, "/%s", m_uName);
			break;
			case objNull:
			fprintf(pFile, "null");
			break;
			case objArray:
			fprintf(pFile, "[");
			for (int nIndex = 0; nIndex < ArrayGetLength(); ++nIndex)
			{
				if (nIndex > 0)
					fprintf(pFile, " ");
				ArrayGetCopy(nIndex, &oTemp);
				oTemp.Print(pFile);
				oTemp.Free();
			}
			fprintf(pFile, "]");
			break;
			case objDict:
			fprintf(pFile, "<<");
			for (int nIndex = 0; nIndex < DictGetLength(); ++nIndex)
			{
				fprintf(pFile, " /%s ", DictGetKey(nIndex));
				DictGetValueCopy(nIndex, &oTemp);
				oTemp.Print(pFile);
				oTemp.Free();
			}
			fprintf(pFile, " >>");
			break;
			case objStream:
			fprintf(pFile, "<stream>");
			break;
			case objRef:
			fprintf(pFile, "%d %d R", m_uRef.nNum, m_uRef.nGen);
			break;
			case objCommand:
			fprintf(pFile, "%s", m_uCommand);
			break;
			case objError:
			fprintf(pFile, "<error>");
			break;
			case objEOF:
			fprintf(pFile, "<EOF>");
			break;
			case objNone:
			fprintf(pFile, "<none>");
			break;
		}
	}

	//------------------------------------------------------------------------
	// Array accessors.
	//------------------------------------------------------------------------
	int     Object::ArrayGetLength()
	{
		return m_uArray->GetCount();
	}

	void    Object::ArrayAdd(Object *pItem)
	{
		m_uArray->Add(pItem);
	}

	Object *Object::ArrayGet(int nIndex, Object *pObject)
	{
		return m_uArray->Get(nIndex, pObject);
	}

	Object *Object::ArrayGetCopy(int nIndex, Object *pObject)
	{
		return m_uArray->GetCopy(nIndex, pObject);
	}

	//------------------------------------------------------------------------
	// Dict accessors.
	//------------------------------------------------------------------------
	int     Object::DictGetLength()
	{
		return m_uDict->GetEntryCount();
	}

	void    Object::DictAdd(char *sKey, Object *pItem)
	{
		m_uDict->AddItem(sKey, pItem);
	}

	bool    Object::DictCheckType(char *sDictType)
	{
		return m_uDict->CheckType(sDictType);
	}

	bool    Object::IsDict(char *sDictType)
	{
		return (m_eType == objDict && DictCheckType(sDictType));
	}

	Object *Object::DictLookup(char *sKey, Object *pObject)
	{
		return m_uDict->Search(sKey, pObject);
	}

	Object *Object::DictLookupAndCopy(char *sKey, Object *pObject)
	{
		return m_uDict->SearchAndCopy(sKey, pObject);
	}

	char   *Object::DictGetKey(int nIndex)
	{
		return m_uDict->GetKey(nIndex);
	}

	Object *Object::DictGetValue(int nIndex, Object *pObject)
	{
		return m_uDict->GetValue(nIndex, pObject);
	}

	Object *Object::DictGetValueCopy(int nIndex, Object *pObject)
	{
		return m_uDict->GetValueCopy(nIndex, pObject);
	}

	//------------------------------------------------------------------------
	// Stream accessors.
	//------------------------------------------------------------------------

	bool         Object::StreamCheckType(char *sDictType)
	{
		return m_uStream->GetDict()->CheckType(sDictType);
	}

	bool         Object::IsStream(char *sDictType)
	{
		return (m_eType == objStream && StreamCheckType(sDictType));
	}

	void         Object::StreamReset()
	{
		m_uStream->Reset();
	}

	void         Object::StreamClose()
	{
		m_uStream->Close();
	}

	int          Object::StreamGetChar()
	{
		return m_uStream->GetChar();
	}

	int          Object::StreamLookChar()
	{
		return m_uStream->LookChar();
	}

	char        *Object::StreamGetLine(char *sBuffer, int nSize)
	{
		return m_uStream->GetLine(sBuffer, nSize);
	}

	unsigned int Object::StreamGetPos()
	{
		return m_uStream->GetPos();
	}

	void         Object::StreamSetPos(unsigned int unPos, int nDirection)
	{
		m_uStream->SetPos(unPos, nDirection);
	}

	Dict        *Object::StreamGetDict()
	{
		return m_uStream->GetDict();
	}
}