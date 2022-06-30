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
#ifndef _PDF_WRITER_SRC_OBJECTS_H
#define _PDF_WRITER_SRC_OBJECTS_H

#include <string>
#include <map>
#include <vector>

#include "Types.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

namespace PdfWriter
{
	class CXref;
	class CStream;
	class CEncrypt;
	class CDocument;

	typedef enum
	{
		object_type_UNKNOWN     = 0x01,
		object_type_NULL        = 0x02,
		object_type_BOOLEAN     = 0x03,
		object_type_NUMBER      = 0x04,
		object_type_REAL        = 0x05,
		object_type_NAME        = 0x06,
		object_type_STRING      = 0x07,
		object_type_BINARY      = 0x08,
		object_type_ARRAY       = 0x10,
		object_type_DICT        = 0x11,
		object_type_PROXY       = 0x12,
		object_type_PATTERN_REF = 0xFA,
		object_type_FUNC_REF	= 0xF2,
		object_type_MASK_FORM   = 0xF3,
		object_type_MASK_REF	= 0xF4,
		object_type_XFORM	    = 0xF5
	} EObjectType;
	typedef enum
	{
		dict_type_UNKNOWN      = 0x00,
		dict_type_FONT         = 0x01,
		dict_type_CATALOG      = 0x02,
		dict_type_PAGES        = 0x03,
		dict_type_PAGE         = 0x04,
		dict_type_XOBJECT      = 0x05,
		dict_type_OUTLINE      = 0x06,
		dict_type_DESTINATION  = 0x07,
		dict_type_ANNOTATION   = 0x08,
		dict_type_ENCRYPT      = 0x09,
		dict_type_EXT_GSTATE   = 0x0A,
		dict_type_EXT_GSTATE_R = 0x0B,  /* read only object */
		dict_type_METADATA     = 0x0C,
		dict_type_SIGNATURE    = 0x0D
	} EDictType;

	class CObjectBase
	{
	public:
		CObjectBase()
		{
			m_unFlags = 0;
			m_unGenNo = 0;
			m_unObjId = 0;
		}
		virtual ~CObjectBase() 
		{}

		virtual EObjectType GetType() const
		{
			return object_type_UNKNOWN;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			return pOut ? pOut : new CObjectBase();
		}
		bool IsHidden() const;
		bool IsDirect() const;
		bool IsIndirect() const;
		void SetDirect();
		void SetIndirect();
		void SetHidden();
		void SetRef(unsigned int unObjId, unsigned int unGenNo)
		{
			m_unObjId = unObjId;
			m_unGenNo = unGenNo;
		}
		unsigned int GetObjId() const
		{
			return m_unObjId;
		}
		unsigned int GetGenNo() const
		{
			return m_unGenNo;
		}
		void WriteValue(CStream* pStream, CEncrypt* pEncrypt);
		void Write     (CStream* pStream, CEncrypt* pEncrypt);

	private:

		unsigned int m_unFlags;
		unsigned int m_unObjId;
		unsigned int m_unGenNo;
	};
	class CNullObject : public CObjectBase
	{
	public:

		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			return pOut ? pOut : new CNullObject();
		}

		EObjectType GetType() const
		{
			return object_type_NULL;
		}
	};
	class CBoolObject : public CObjectBase
	{
	public:

		CBoolObject(const bool& bValue)
		{
			m_bValue = bValue;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_BOOLEAN)
			{
				((CBoolObject*)pOut)->Set(m_bValue);
				return pOut;
			}
			return new CBoolObject(m_bValue);
		}
		EObjectType GetType() const
		{
			return object_type_BOOLEAN;
		}
		bool Get() const
		{
			return m_bValue;
		}
		void Set(const bool& bValue)
		{
			m_bValue = bValue;
		}

	private:
		bool m_bValue;
	};
	class CNumberObject : public CObjectBase
	{
	public:
		CNumberObject()
		{
			m_nValue = 0;
		}
		CNumberObject(const int& nValue)
		{
			m_nValue = nValue;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_NUMBER)
			{
				((CNumberObject*)pOut)->Set(m_nValue);
				return pOut;
			}
			return new CNumberObject(m_nValue);
		}
		EObjectType GetType() const
		{
			return object_type_NUMBER;
		}
		void Set(const int& nValue)
		{
			m_nValue = nValue;
		}
		int  Get() const
		{
			return m_nValue;
		}
		void operator++(int)
		{
			m_nValue++;
		}
		void operator--(int)
		{
			m_nValue--;
		}

	private:
		int m_nValue;
	};
	class CRealObject : public CObjectBase
	{
	public:
		CRealObject(float fValue)
		{
			Set(fValue);
		}
		CRealObject(double dValue)
		{
			Set((float)dValue);
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_REAL)
			{
				((CRealObject*)pOut)->Set(m_fValue);
				return pOut;
			}
			return new CRealObject(m_fValue);
		}
		EObjectType GetType() const
		{
			return object_type_REAL;
		}
		void Set(float fValue)
		{
			m_fValue = std::min(std::max(fValue, LIMIT_MIN_REAL), LIMIT_MAX_REAL);
		}
		void Set(double dValue)
		{
			m_fValue = (float)std::min(std::max((float)dValue, LIMIT_MIN_REAL), LIMIT_MAX_REAL);
		}
		float Get() const
		{
			return m_fValue;
		}

		private:
			float m_fValue;
	};
	class CNameObject : public CObjectBase
	{
	public:

		CNameObject(const char* sValue)
		{
			Set(sValue);
		}
		void Set(const char* sValue);
		const char* Get() const
		{
			return m_sValue;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_NAME)
			{
				((CNameObject*)pOut)->Set(m_sValue);
				return pOut;
			}
			return new CNameObject(m_sValue);
		}
		EObjectType GetType() const
		{
			return object_type_NAME;
		}

	private:
		char m_sValue[LIMIT_MAX_NAME_LEN + 1];
	};
	class CStringObject : public CObjectBase
	{
	public:
		CStringObject(const char* sValue, bool isUTF16 = false, bool isDictValue = false);
		virtual ~CStringObject();
		void Set(const char* sValue, bool isUTF16, bool isDictValue);
		const BYTE*  GetString() const
		{
			return (const BYTE*)m_pValue;
		}
		unsigned int GetLength() const
		{
			return m_unLen;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_STRING)
			{
				((CStringObject*)pOut)->Set((const char*)m_pValue, m_bUTF16, m_bDictValue);
				return pOut;
			}
			return new CStringObject((const char*)m_pValue, m_bUTF16, m_bDictValue);
		}
		EObjectType  GetType() const
		{
			return object_type_STRING;
		}
		bool         IsUTF16() const
		{
			return m_bUTF16;
		}
		bool         IsDictValue() const
		{
			return m_bDictValue;
		}

	private:
		BYTE*        m_pValue;
		unsigned int m_unLen;
		bool         m_bUTF16;
		bool         m_bDictValue;
	};
	class CBinaryObject : public CObjectBase
	{
	public:
		CBinaryObject(const BYTE* pValue, unsigned int unLen);
		~CBinaryObject();
		void         Set(const BYTE* pValue, unsigned int unLen);
		BYTE*        GetValue() const
		{
			return m_pValue;
		}
		unsigned int GetLength() const
		{
			return m_unLen;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_BINARY)
			{
				((CBinaryObject*)pOut)->Set(m_pValue, m_unLen);
				return pOut;
			}
			return new CBinaryObject(m_pValue, m_unLen);
		}
		EObjectType  GetType() const
		{
			return object_type_BINARY;
		}

	private:
		BYTE*        m_pValue;
		unsigned int m_unLen;
	};
	class CProxyObject : public CObjectBase
	{
	public:
		CProxyObject(CObjectBase* pObject, bool bClear = false);
		~CProxyObject();
		CObjectBase* Get() const
		{
			return m_pObject;
		}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const
		{
			if (pOut && pOut->GetType() == object_type_PROXY && ((CProxyObject*)pOut)->m_pObject)
			{
				((CProxyObject*)pOut)->m_pObject->SetRef(m_pObject->GetObjId(), m_pObject->GetGenNo());
				return pOut;
			}
			CProxyObject* pRes = new CProxyObject(new CObjectBase(), true);
			pRes->Get()->SetRef(m_pObject->GetObjId(), m_pObject->GetGenNo());
			return pRes;
		}
		EObjectType GetType() const
		{
			return object_type_PROXY;
		}
	private:
		CObjectBase* m_pObject;
		bool m_bClear;
	};
	class CArrayObject : public CObjectBase
	{
	public:
		CArrayObject()
		{
		}
		virtual ~CArrayObject()
		{
			Clear();
		}
		int          GetCount() const
		{
			return m_arrList.size();
		}
		void         Add(CObjectBase* pObject, bool bPushBack = true);
		void         Add(bool bValue);
		void         Add(int nValue);
		void         Add(unsigned int unValue);
		void         Add(float fValue);
		void         Add(const char* sValue);
		void         Add(double dValue);
		void         Insert(CObjectBase* pTarget, CObjectBase* pObject, bool bReplace = false);
		CObjectBase* Get(unsigned int unIndex, bool bCheckProxy = true) const;
		CObjectBase* Remove(unsigned int unIndex);
		void         Clear();
		EObjectType  GetType() const
		{
			return object_type_ARRAY;
		}
		static CArrayObject* CreateBox(const TBox& oBox);
		static CArrayObject* CreateBox(double dL, double dB, double dR, double dT);
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const;
		void FromXml(const std::wstring& sXml);

	protected:
		std::vector<CObjectBase*> m_arrList;
	};
	class CDictObject : public CObjectBase
	{
	public:
		CDictObject();
		CDictObject(CXref* pXref);
		virtual ~CDictObject();
		virtual EObjectType GetType() const
		{
			return object_type_DICT;
		}
		CObjectBase* Get(const std::string& sKey) const;
		void         Add(const std::string& sKey, CObjectBase* pObject);
		void         Remove(const std::string& sKey);
		void         Add(const std::string& sKey, const char* sName);
		void         Add(const std::string& sKey, int nNumber);
		void         Add(const std::string& sKey, unsigned int unNumber);
		void         Add(const std::string& sKey, float fReal);
		void         Add(const std::string& sKey, double dReal);
		void         Add(const std::string& sKey, bool bBool);
		const char*  GetKey(const CObjectBase* pObject);
		CStream*     GetStream() const
		{
			return m_pStream;
		}
		unsigned int GetFilter() const
		{
			return m_unFilter;
		}
		unsigned int GetPredictor() const
		{
			return m_unPredictor;
		}
		void         SetFilter(unsigned int unFiler)
		{
			m_unFilter = unFiler;
		}
		void         SetStream(CXref* pXref, CStream* pStream);

		virtual void      BeforeWrite(){}
		virtual void      Write(CStream* pStream){}
		virtual void      AfterWrite(){}
		virtual CObjectBase* Copy(CObjectBase* pOut = NULL) const;
		virtual EDictType GetDictType() const
		{
			return dict_type_UNKNOWN;
		}

		void WriteToStream(CStream* pStream, CEncrypt* pEncrypt);
		void WriteSignatureToStream(CStream* pStream, CEncrypt* pEncrypt);
		unsigned int GetSize() { return m_mList.size(); }
		void FromXml(const std::wstring& sXml);

	private:

		std::map<std::string, CObjectBase*> m_mList;
		unsigned int                        m_unFilter;
		unsigned int                        m_unPredictor;
		CStream*                            m_pStream;
	};
	struct TXrefEntry
	{
		char         nEntryType;
		unsigned int unByteOffset;
		unsigned int unGenNo;
		CObjectBase* pObject;
	};
	class CXref
	{
	public:

		CXref(CDocument* pDocument, unsigned int unOffset);
		CXref(CDocument* pDocument, unsigned int unRemoveId, unsigned int unRemoveGen);
		~CXref();

		TXrefEntry* GetEntry(unsigned int unIndex) const;
		TXrefEntry* GetEntryByObjectId(unsigned int unObjectId) const;
		CXref*      GetXrefByObjectId(unsigned int unObjectId);
		void        Add(CObjectBase* pObject);		
		void        WriteToStream(CStream* pStream, CEncrypt* pEncrypt, bool bStream = false);
		void        SetPrev(CXref* pPrev)
		{
			m_pPrev = pPrev;
		}
		void        SetPrevAddr(unsigned int unAddr)
		{
			m_unAddr = unAddr;
		}
		unsigned int GetPrevAddr() const
		{
			return m_unAddr;
		}
		unsigned int GetSizeXRef() const
		{
			return m_unStartOffset + m_arrEntries.size();
		}
		int         GetCount() const
		{
			return m_arrEntries.size();
		}
		CDictObject*GetTrailer() const
		{
			return m_pTrailer;
		}
		bool        IsPDFA() const;

	private:

		void        WriteTrailer(CStream* pStream);

	private:
		std::vector<TXrefEntry*> m_arrEntries;
		unsigned int             m_unStartOffset;
		unsigned int             m_unAddr;
		CXref*                   m_pPrev;
		CDictObject*             m_pTrailer;
		CDocument*               m_pDocument;
	};
}

#endif // _PDF_WRITER_SRC_OBJECTS_H

