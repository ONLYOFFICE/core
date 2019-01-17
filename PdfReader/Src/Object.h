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
#ifndef _PDF_READER_OBJECT_H
#define _PDF_READER_OBJECT_H

#include <stdio.h>
#include <string.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "Array.h"

#ifdef GetObject
#undef GetObject
#endif

namespace PdfReader
{
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
			if (nNum == oRight.nNum && nGen == oRight.nGen)
				return true;

			return false;
		}
		bool operator <(const Ref& oRight)const
		{
			if (nNum < oRight.nNum)
				return true;
			else if (nNum == oRight.nNum)
				return nGen < oRight.nGen;
			else
				return false;
		}
		bool operator >(const Ref& oRight)const
		{
			if (nNum > oRight.nNum)
				return true;
			else if (nNum == oRight.nNum)
				return nGen > oRight.nGen;
			else
				return false;
		}
		bool operator <=(const Ref& oRight)const
		{
			if (*this == oRight)
				return true;

			return (*this < oRight);
		}
		bool operator >=(const Ref& oRight)const
		{
			if (*this == oRight)
				return true;

			return (*this > oRight);
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

		Object() :
			m_eType(objNone)
		{
		}


		Object *InitBool(bool bBool)
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
		bool IsBool()
		{
			return (m_eType == objBool);
		}
		bool IsInt()
		{
			return (m_eType == objInt);
		}
		bool IsReal()
		{
			return (m_eType == objReal);
		}
		bool IsNum()
		{
			return (m_eType == objInt || m_eType == objReal);
		}
		bool IsString()
		{
			return (m_eType == objString);
		}
		bool IsName()
		{
			return (m_eType == objName);
		}
		bool IsNull()
		{
			return (m_eType == objNull);
		}
		bool IsArray()
		{
			return (m_eType == objArray);
		}
		bool IsDict()
		{
			return (m_eType == objDict);
		}
		bool IsStream()
		{
			return (m_eType == objStream);
		}
		bool IsRef()
		{
			return (m_eType == objRef);
		}
		bool IsCommand()
		{
			return (m_eType == objCommand);
		}
		bool IsError()
		{
			return (m_eType == objError);
		}
		bool IsEOF()
		{
			return (m_eType == objEOF);
		}
		bool IsNone()
		{
			return (m_eType == objNone);
		}

		// Сепциальные проверки.
		bool IsName(char *sName)
		{
			return (m_eType == objName && !strcmp(m_uName, sName));
		}
		bool IsDict(char *sDictType);
		bool IsStream(char *sDictType);
		bool IsCommand(char *sCommand)
		{
			return (m_eType == objCommand && !strcmp(m_uCommand, sCommand));
		}
		// В следующих функциях предполагается, что тип объекта корректный.
		// Т.е. соответствует типу запрашиваемого значения.
		bool       GetBool()
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
			return (m_eType == objInt) ? (double)m_uInt : m_uReal;
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
		bool    DictCheckType(char *sDictType);
		Object *DictLookup(char *sKey, Object *pObject);
		Object *DictLookupAndCopy(char *sKey, Object *pObject);
		char   *DictGetKey(int nIndex);
		Object *DictGetValue(int nIndex, Object *pObject);
		Object *DictGetValueCopy(int nIndex, Object *pObject);

		// Stream
		bool         StreamCheckType(char *sDictType);
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
			bool       m_uBool;       // boolean
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
}
#endif // _PDF_READER_OBJECT_H
