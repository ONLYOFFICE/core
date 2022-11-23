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
#include "Objects.h"
#include "Utils.h"
#include "Types.h"
#include "Encrypt.h"
#include "Streams.h"
#include "Document.h"
#include "EncryptDictionary.h"

// Если установлен бит OTYPE_DIRECT, значит данный объект принадлежит другому
// объекту. Если установлен бит OTYPE_INDIRECT, значит объект управляется таблицей xref.
#define  FLAG_NONE     0x0
#define  FLAG_HIDDEN   0x1
#define  FLAG_INDIRECT 0x4
#define  FLAG_DIRECT   0x8

//------ Значения относящиеся к объекту xref ------------------------------------
#define FREE_ENTRY             'f'
#define IN_USE_ENTRY           'n'

#define RELEASE_OBJECT(pObject) \
	if (pObject && !pObject->IsIndirect())\
		delete pObject;\

static const BYTE UNICODE_HEADER[] ={ 0xFE, 0xFF };

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CObjectBase
	//----------------------------------------------------------------------------------------
	bool CObjectBase::IsHidden() const
	{
		return (m_unFlags & FLAG_HIDDEN ? true : false);
	}
	bool CObjectBase::IsDirect() const
	{
		return (m_unFlags & FLAG_DIRECT ? true : false);
	}
	bool CObjectBase::IsIndirect() const
	{
		return (m_unFlags & FLAG_INDIRECT ? true : false);
	}
	void CObjectBase::SetDirect()
	{
		m_unFlags |= FLAG_DIRECT;
	}
	void CObjectBase::SetIndirect()
	{
		m_unFlags |= FLAG_INDIRECT;
	}
	void CObjectBase::SetHidden()
	{
		m_unFlags |= FLAG_HIDDEN;
	}
	void CObjectBase::WriteValue(CStream* pStream, CEncrypt* pEncrypt)
	{

		switch (GetType())
		{
		case object_type_NAME:   pStream->Write((CNameObject*)this); break;
		case object_type_NUMBER: pStream->Write((CNumberObject*)this); break;
		case object_type_REAL:   pStream->Write((CRealObject*)this); break;
		case object_type_STRING: pStream->Write((CStringObject*)this, pEncrypt); break;
		case object_type_BINARY: pStream->Write((CBinaryObject*)this, pEncrypt); break;
		case object_type_ARRAY:  pStream->Write((CArrayObject*)this, pEncrypt); break;
		case object_type_DICT:   pStream->Write((CDictObject*)this, pEncrypt); break;
		case object_type_BOOLEAN:pStream->Write((CBoolObject*)this); break;
		case object_type_NULL:   pStream->WriteStr("null"); break;
		}
	}
	void CObjectBase::Write     (CStream* pStream, CEncrypt* pEncrypt)
	{
		if (IsHidden())
			return;

		if (object_type_PROXY == GetType())
		{
			char sBuf[SHORT_BUFFER_SIZE];
			char *pBuf = sBuf;
			char *pEndPtr = sBuf + SHORT_BUFFER_SIZE - 1;

			CObjectBase* pObject = ((CProxyObject*)this)->Get();

			pBuf = ItoA(pBuf, pObject->m_unObjId & 0x00FFFFFF, pEndPtr);
			*pBuf++ = ' ';
			pBuf = ItoA(pBuf, pObject->m_unGenNo, pEndPtr);
			StrCpy(pBuf, " R", pEndPtr);
			pStream->WriteStr(sBuf);
		}
		else
		{
			WriteValue(pStream, pEncrypt);
		}
	}
	//----------------------------------------------------------------------------------------
	// CNameObject
	//----------------------------------------------------------------------------------------
	void CNameObject::Set(const char* sValue)
	{
		if (!sValue || 0 == sValue[0])
			m_sValue[0] = 0;
		else
		{
			StrCpy(m_sValue, sValue, m_sValue + LIMIT_MAX_NAME_LEN);
		}
	}
	//----------------------------------------------------------------------------------------
	// CStringObject
	//----------------------------------------------------------------------------------------
    CStringObject::CStringObject(const char* sValue, bool isUTF16, bool isDictValue)
	{
		m_pValue = NULL;
		m_unLen  = 0;
		Set(sValue, isUTF16, isDictValue);
	}
	CStringObject::~CStringObject()
	{
		if (m_pValue)
			delete[] m_pValue;
	}
	void CStringObject::Set(const char* sValue, bool isUTF16, bool isDictValue)
	{
		if (m_pValue)
		{
			delete[] m_pValue;
			m_unLen = 0;
		}

		unsigned int unLen = StrLen(sValue, LIMIT_MAX_STRING_LEN);
		m_pValue = new BYTE[unLen + 1];
		StrCpy((char*)m_pValue, (char*)sValue, (char*)(m_pValue + unLen));
		m_unLen = unLen;
		m_bUTF16     = isUTF16;
		m_bDictValue = isDictValue;
	}
	//----------------------------------------------------------------------------------------
	// CBinaryObject
	//----------------------------------------------------------------------------------------
	CBinaryObject::CBinaryObject(const BYTE* pValue, unsigned int unLen)
	{
		m_pValue = NULL;
		m_unLen  = 0;
		Set(pValue, unLen);
	}
	CBinaryObject::~CBinaryObject()
	{
		if (m_pValue)
			delete[] m_pValue;
	}
	void CBinaryObject::Set(const BYTE* pValue, unsigned int unLen)
	{
        unLen = std::min((unsigned int)LIMIT_MAX_STRING_LEN, unLen);
		if (m_pValue)
		{
			delete[] m_pValue;
			m_pValue = NULL;
			m_unLen  = 0;
		}

		if (!pValue || !unLen)
			return;

		m_unLen  = unLen;
		m_pValue = new BYTE[unLen];

		MemCpy(m_pValue, pValue, unLen);
	}
	//----------------------------------------------------------------------------------------
	// CProxyObject
	//----------------------------------------------------------------------------------------
	CProxyObject::CProxyObject(CObjectBase* pObject, bool bClear)
	{
		m_pObject = pObject;
		m_bClear = bClear;
	}
	CProxyObject::~CProxyObject()
	{
		if (m_bClear)
			RELEASE_OBJECT(m_pObject);
	}
	//----------------------------------------------------------------------------------------
	// CArrayObject
	//----------------------------------------------------------------------------------------
    void CArrayObject::Add(CObjectBase* pObject, bool bPushBack)
	{
		if (!pObject)
			return;

		// Не даем писать сложные объекты в массив не по ссылке
		if (pObject->IsDirect())
			return;

		if (GetCount() >= LIMIT_MAX_ARRAY)
		{
			RELEASE_OBJECT(pObject);
			return;
		}

		if (pObject->IsIndirect())
		{
			CObjectBase* pProxy = new CProxyObject(pObject);
			if (!pProxy)
			{
				RELEASE_OBJECT(pObject);
				return;
			}

			pObject = pProxy;
		}

		pObject->SetDirect();

		if (bPushBack)
			m_arrList.push_back(pObject);
		else
			m_arrList.insert(m_arrList.begin(), pObject);
	}
    void CArrayObject::Add(bool bValue)
	{
		CObjectBase* pBool = new CBoolObject(bValue);
		if (pBool)
			Add(pBool);
	}
    void CArrayObject::Add(int nValue)
	{
		CObjectBase* pNumber = new CNumberObject(nValue);
		if (pNumber)
			Add(pNumber);
	}
    void CArrayObject::Add(unsigned int unValue)
	{
		CObjectBase* pNumber = new CNumberObject((int)unValue);
		if (pNumber)
			Add(pNumber);
	}
    void CArrayObject::Add(float fValue)
	{
		CObjectBase* pReal = new CRealObject(fValue);
		if (pReal)
			Add(pReal);
	}
    void CArrayObject::Add(const char* sName)
	{
		CObjectBase* pName = new CNameObject(sName);
		if (pName)
			Add(pName);
	}
    void CArrayObject::Add(double dValue)
	{
		CObjectBase* pReal = new CRealObject(dValue);
		if (pReal)
			Add(pReal);
	}
    void CArrayObject::Insert(CObjectBase *pTarget, CObjectBase* pObject, bool bReplace)
	{
		if (!pObject)
			return;

		if (pObject->IsDirect())
			return;

		if (GetCount() >= LIMIT_MAX_ARRAY)
		{
			RELEASE_OBJECT(pObject);
			return;
		}

		if (pObject->IsIndirect())
		{
			CObjectBase* pProxy = new CProxyObject(pObject);
			if (!pProxy)
			{
				RELEASE_OBJECT(pObject);
				return;
			}

			pObject = pProxy;
		}

		pObject->SetDirect();

		//получаем target-object из списка
		//рассмотреть случай, когда указатель на содержимое списка
		//может быть proxy-object.

		for (int nIndex = 0, nCount = m_arrList.size(); nIndex < nCount; nIndex++)
		{
			CObjectBase* pObjectItem = m_arrList.at(nIndex);
			if (object_type_PROXY == pObjectItem->GetType())
				pObjectItem = ((CProxyObject*)pObjectItem)->Get();

			if (pObjectItem == pTarget)
			{
				if (bReplace)
				{
					m_arrList.erase(m_arrList.begin() + nIndex);
					RELEASE_OBJECT(pObjectItem);
				}
				m_arrList.insert(m_arrList.begin() + nIndex, pObject);
				return;
			}
		}

		// Дошли до сюда, значит не вставили данный объект, поэтому удаляем его
		RELEASE_OBJECT(pObject);
	}
	CObjectBase*  CArrayObject::Get(unsigned int unIndex, bool bCheckProxy) const
	{
		if (unIndex >= m_arrList.size())
			return NULL;

		CObjectBase* pObject = m_arrList.at(unIndex);
		if (bCheckProxy && object_type_PROXY == pObject->GetType())
			pObject = ((CProxyObject*)pObject)->Get();

		return pObject;
	}
	CObjectBase* CArrayObject::Remove(unsigned int unIndex)
	{
		if (unIndex >= m_arrList.size())
			return NULL;

		CObjectBase* pObject = Get(unIndex);
		if (pObject)
			m_arrList.erase(m_arrList.begin() + unIndex);
		return pObject;
	}
    void CArrayObject::Clear()
	{
		for (int nIndex = 0, nCount = m_arrList.size(); nIndex < nCount; nIndex++)
		{
			CObjectBase* pObject = m_arrList.at(nIndex);
			RELEASE_OBJECT(pObject);
		}

		m_arrList.clear();
	}
	CArrayObject* CArrayObject::CreateBox(const TBox& oBox)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return NULL;

		pArray->Add(oBox.fLeft);
		pArray->Add(oBox.fBottom);
		pArray->Add(oBox.fRight);
		pArray->Add(oBox.fTop);

		return pArray;
	}
	CArrayObject* CArrayObject::CreateBox(double dL, double dB, double dR, double dT)
	{
		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return NULL;

		pArray->Add(dL);
		pArray->Add(dB);
		pArray->Add(dR);
		pArray->Add(dT);

		return pArray;
	}
	CObjectBase* CArrayObject::Copy(CObjectBase* pOut) const
	{
		CArrayObject* pArray = pOut && pOut->GetType() == object_type_ARRAY ? (CArrayObject*)pOut : new CArrayObject();
		if (!pArray)
			return NULL;

		for (unsigned int unIndex = 0, unCount = GetCount(); unIndex < unCount; ++unIndex)
		{
			pArray->Add(Get(unIndex, false)->Copy());
		}

		return pArray;
	}

#define AddToObject(oVal)\
{\
	if (pObject->GetType() == object_type_DICT)\
		((CDictObject*)pObject)->Add(sName, oVal);\
	else if (pObject->GetType() == object_type_ARRAY)\
		((CArrayObject*)pObject)->Add(oVal);\
}

	void ReadDict(XmlUtils::CXmlLiteReader& oCoreReader, CObjectBase* pObject)
	{
		int gen = 0;
		std::string sType;
		std::string sName = oCoreReader.GetNameA();

		while (oCoreReader.MoveToNextAttribute())
		{
			std::wstring sAName = oCoreReader.GetName();
			std::string sAText = oCoreReader.GetTextA();
			if (sAName == L"type")
				sType = sAText;
			else if (sAName == L"gen")
				gen = std::stoi(sAText);
			else if (sAName == L"num")
			{
				if (sType == "Bool")
					AddToObject(sAText == "true")
				else if (sType == "Int")
					AddToObject(std::stoi(sAText))
				else if (sType == "Real")
					AddToObject(std::stod(sAText))
				else if (sType == "String")
					AddToObject(new CStringObject(sAText.c_str()))
				else if (sType == "Name")
					AddToObject(sAText.c_str())
				// Null  ниже
				// Array ниже
				// Dict  ниже
				// Stream игнорируется
				else if (sType == "Ref")
				{
					CObjectBase* pBase = new CObjectBase();
					pBase->SetRef(std::stoi(sAText), gen);
					AddToObject(new CProxyObject(pBase, true));
				}
				// Cmd игнорируется
				else if (sType == "Cmd")
					AddToObject(sAText.c_str())
				// Error игнорируется
				// EOF игнорируется
				// None ниже
				else if (sType == "Binary")
					gen = std::stoi(sAText);
			}
		}
		oCoreReader.MoveToElement();

		if (sType == "Array")
		{
			CArrayObject* pArray = new CArrayObject();
			AddToObject(pArray);

			int n2Death = oCoreReader.GetDepth();
			while (oCoreReader.ReadNextSiblingNode(n2Death))
				ReadDict(oCoreReader, pArray);
		}
		else if (sType == "Dict")
		{
			CDictObject* pDict = new CDictObject();
			AddToObject(pDict);

			int n2Death = oCoreReader.GetDepth();
			while (oCoreReader.ReadNextSiblingNode(n2Death))
				ReadDict(oCoreReader, pDict);
		}
		else if (sType == "None")
			AddToObject("None")
		else if (sType == "Null")
			AddToObject(new CNullObject())
		else if (sType == "Binary")
		{
			BYTE* arrId = new BYTE[gen];
			int n2Death = oCoreReader.GetDepth(), i = 0;
			while (oCoreReader.ReadNextSiblingNode(n2Death))
			{
				std::wstring sChar = oCoreReader.GetText2();
				arrId[i++] = std::stoi(sChar);
			}
			AddToObject(new CBinaryObject(arrId, gen));
			RELEASEARRAYOBJECTS(arrId);
		}
	}
	void CArrayObject::FromXml(const std::wstring& sXml)
	{
		XmlUtils::CXmlLiteReader oCoreReader;
		oCoreReader.FromString(sXml);
		oCoreReader.ReadNextNode();
		int nDeath = oCoreReader.GetDepth();
		while (oCoreReader.ReadNextSiblingNode(nDeath))
			ReadDict(oCoreReader, this);
	}
	//----------------------------------------------------------------------------------------
	// CDictObject
	//----------------------------------------------------------------------------------------
	CDictObject::CDictObject()
	{
		m_unFilter    = STREAM_FILTER_NONE;
		m_unPredictor = STREAM_PREDICTOR_NONE;
		m_pStream     = NULL;
	}
	CDictObject::CDictObject(CXref* pXref)
	{
		m_unFilter    = STREAM_FILTER_NONE;
		m_unPredictor = STREAM_PREDICTOR_NONE;
		m_pStream     = NULL;

		CNumberObject* pLength = new CNumberObject(0);

		// Только stream object добавляются в таблицу xref автоматически
		pXref->Add((CObjectBase*)this);
		pXref->Add((CObjectBase*)pLength);

		Add("Length", (CObjectBase*)pLength);

		m_pStream = new CMemoryStream(STREAM_BUF_SIZ);
	}
	CDictObject::~CDictObject()
	{
		for (auto const &oIter : m_mList) 
		{
			CObjectBase* pObject = oIter.second;
			RELEASE_OBJECT(pObject);
		}

		if (m_pStream)
			delete m_pStream;
	}
	CObjectBase*  CDictObject::Get(const std::string& sKey) const
	{
		std::map<std::string, CObjectBase*>::const_iterator oIter = m_mList.find(sKey);
		if (m_mList.end() != oIter)
		{
			CObjectBase* pObject = oIter->second;
			if (pObject && object_type_PROXY == pObject->GetType())
				pObject = ((CProxyObject*)pObject)->Get();

			return pObject;
		}

		return NULL;
	}
    void CDictObject::Add(const std::string& sKey, CObjectBase* pObject)
	{
		if (!pObject)
			return;

		if (pObject->IsDirect())
			return;

		if (m_mList.size() >= LIMIT_MAX_DICT_ELEMENT)
		{
			RELEASE_OBJECT(pObject);
			return;
		}

		// Удаляем старую запись, если она была
		Remove(sKey);
		
		if (pObject->IsIndirect())
		{
			CObjectBase* pProxy = new CProxyObject(pObject);
			pObject = pProxy;
		}
		pObject->SetDirect();
        m_mList.insert(std::make_pair(sKey, pObject));
	}
    void CDictObject::Remove(const std::string& sKey)
	{
		std::map<std::string, CObjectBase*>::const_iterator pIter = m_mList.find(sKey);
		if (m_mList.end() != pIter)
		{
			CObjectBase* pObject = pIter->second;
			RELEASE_OBJECT(pObject);
			m_mList.erase(sKey);
		}
	}
    void CDictObject::Add(const std::string& sKey, const char* sName)
	{
		Add(sKey, new CNameObject(sName));
	}
    void CDictObject::Add(const std::string& sKey, int nNumber)
	{
		Add(sKey, new CNumberObject(nNumber));
	}
    void CDictObject::Add(const std::string& sKey, unsigned int unNumber)
	{
		Add(sKey, (int)unNumber);
	}
    void CDictObject::Add(const std::string& sKey, float fReal)
	{
		Add(sKey, new CRealObject(fReal));
	}
    void CDictObject::Add(const std::string& sKey, double dReal)
	{
		Add(sKey, new CRealObject(dReal));
	}
    void CDictObject::Add(const std::string& sKey, bool bBool)
	{
		Add(sKey, new CBoolObject(bBool));
	}
	const char*   CDictObject::GetKey(const CObjectBase* pObject)
	{
		for (auto const &oIter : m_mList)
		{
			CObjectBase* pListObject = oIter.second;

			if (pListObject && object_type_PROXY == pListObject->GetType())
				pListObject = ((CProxyObject*)pListObject)->Get();

			if (pListObject == pObject)
				return oIter.first.c_str();
		}

		return NULL;
	}
    void CDictObject::WriteToStream(CStream* pStream, CEncrypt* pEncrypt)
	{
		for (auto const &oIter : m_mList)
		{
			CObjectBase* pObject = oIter.second;
			if (!pObject)
				continue;

			if (pObject->IsHidden())
			{
				// ничего не делаем
			}
			else
			{
				pStream->WriteEscapeName(oIter.first.c_str());
				pStream->WriteChar(' ');
				pStream->Write(pObject, pEncrypt);
				pStream->WriteStr("\012");
			}
		}
	}
    void CDictObject::WriteSignatureToStream(CStream* pStream, CEncrypt* pEncrypt)
	{
		for (auto const &oIter : m_mList)
		{
			CObjectBase* pObject = oIter.second;
			if (!pObject)
				continue;

			if (pObject->IsHidden())
			{
				// ничего не делаем
			}
			else
			{
				int nBegin, nEnd;
				pStream->WriteEscapeName(oIter.first.c_str());
				pStream->WriteChar(' ');
				nBegin = pStream->Tell();
				// Цифровая подпись не шифруется
				pStream->Write(pObject, oIter.first == "Contents" ? NULL : pEncrypt);
				nEnd = pStream->Tell();
				pStream->WriteStr("\012");
				if (oIter.first == "Contents")
					((CSignatureDict*)this)->SetByteRange(nBegin, nEnd);
				if (oIter.first == "ByteRange")
					((CSignatureDict*)this)->ByteRangeOffset(nBegin, nEnd);
			}
		}
	}
    void CDictObject::SetStream(CXref* pXref, CStream* pStream)
	{
		if (m_pStream)
			delete m_pStream;

		if (!Get("Length"))
		{
			CNumberObject* pLength = new CNumberObject(0);

			// Только stream object добавляются в таблицу xref автоматически
			pXref->Add((CObjectBase*)this);
			pXref->Add((CObjectBase*)pLength);

			Add("Length", (CObjectBase*)pLength);
		}

		m_pStream = pStream;
	}
	CObjectBase* CDictObject::Copy(CObjectBase* pOut) const
	{
		CDictObject* pDict = pOut && pOut->GetType() == object_type_DICT ? (CDictObject*)pOut : new CDictObject();
		if (!pDict)
			return NULL;

		for (auto const &oIter : m_mList)
		{
			pDict->Add(oIter.first, oIter.second->Copy());
		}

		return pDict;
	}
	void CDictObject::FromXml(const std::wstring& sXml)
	{
		XmlUtils::CXmlLiteReader oCoreReader;
		oCoreReader.FromString(sXml);
		oCoreReader.ReadNextNode();

		int num = 0, gen = 0;
		while (oCoreReader.MoveToNextAttribute())
		{
			std::wstring sAName = oCoreReader.GetName();
			std::string sAText = oCoreReader.GetTextA();
			if (sAName == L"gen")
				gen = std::stoi(sAText);
			else if (sAName == L"num")
				num = std::stoi(sAText);
		}
		oCoreReader.MoveToElement();
		if (num)
			SetRef(num, gen);

		int nDeath = oCoreReader.GetDepth();
		while (oCoreReader.ReadNextSiblingNode(nDeath))
			ReadDict(oCoreReader, this);
	}
	//----------------------------------------------------------------------------------------
	// CXref
	//----------------------------------------------------------------------------------------
	CXref::CXref(CDocument* pDocument, unsigned int unOffset)
	{
		m_pDocument     = pDocument;
		m_unStartOffset = unOffset;
		m_unAddr        = 0;
		m_pPrev         = NULL;
		m_pTrailer      = NULL;

		if (0 == m_unStartOffset)
		{
			// Добавляем первый элемент в таблицу xref
			// он должен иметь вид 0000000000 65535 f
			TXrefEntry* pEntry = new TXrefEntry;
			pEntry->nEntryType  =  FREE_ENTRY;
			pEntry->unByteOffset = 0;
			pEntry->unGenNo      = MAX_GENERATION_NUM;
			pEntry->pObject      = NULL;
			m_arrEntries.push_back(pEntry);
		}

		m_pTrailer = new CDictObject();
	}
	CXref::CXref(CDocument* pDocument, unsigned int unRemoveId, unsigned int unRemoveGen)
	{
		m_pDocument     = pDocument;
		m_unStartOffset = unRemoveId;
		m_unAddr        = 0;
		m_pPrev         = NULL;
		m_pTrailer      = NULL;

		// Добавляем удаляемый элемент в таблицу xref
		// он должен иметь вид 0000000000 gen+1 f
		TXrefEntry* pEntry = new TXrefEntry;
		pEntry->nEntryType  =  FREE_ENTRY;
		pEntry->unByteOffset = 0;
		pEntry->unGenNo      = unRemoveGen + 1 > MAX_GENERATION_NUM ? MAX_GENERATION_NUM : unRemoveGen + 1;
		pEntry->pObject      = NULL;
		m_arrEntries.push_back(pEntry);

		m_pTrailer = new CDictObject();
	}
	CXref::~CXref()
	{
		for (int nIndex = 0, nCount = m_arrEntries.size(); nIndex < nCount; nIndex++)
		{
			TXrefEntry* pEntry = m_arrEntries.at(nIndex);
			if (pEntry)
			{
				CObjectBase* pObject = pEntry->pObject;
				if (pObject)
					delete pObject;
			}

			delete pEntry;
		}

		if (m_pTrailer)
			delete m_pTrailer;

		if (m_pPrev)
			delete m_pPrev;
	}
	TXrefEntry* CXref::GetEntry(unsigned int unIndex) const
	{
		return m_arrEntries.at(unIndex);
	}
	TXrefEntry* CXref::GetEntryByObjectId(unsigned int nObjectId) const
	{
		const CXref* pXref = this;

		while (pXref)
		{
			if (pXref->m_arrEntries.size() + pXref->m_unStartOffset <= nObjectId)
				return NULL;

			if (pXref->m_unStartOffset <= nObjectId)
			{
				for (unsigned int unIndex = 0, nCount = pXref->m_arrEntries.size(); unIndex < nCount; unIndex++)
				{
					if (pXref->m_unStartOffset + unIndex == nObjectId)
					{
						return pXref->GetEntry(unIndex);
					}
				}
			}

			pXref = (const CXref*)pXref->m_pPrev;
		}

		return NULL;
	}
	CXref* CXref::GetXrefByObjectId(unsigned int unObjectId)
	{
		CXref* pXref = this;

		while (pXref)
		{
			if (unObjectId >= pXref->m_unStartOffset && unObjectId < pXref->m_unStartOffset + pXref->m_arrEntries.size())
				return pXref;
			pXref = pXref->m_pPrev;
		}

		return NULL;
	}
    void CXref::Add(CObjectBase* pObject)
	{
		if (!pObject)
			return;

		if (pObject->IsDirect() || pObject->IsIndirect())
			return;

		if (m_arrEntries.size() >= LIMIT_MAX_XREF_ELEMENT)
		{
			RELEASE_OBJECT(pObject);
			return;
		}


		// В случае ошибки r объектe нужно применить dispose
		TXrefEntry* pEntry = new TXrefEntry;
		if (NULL == pEntry)
		{
			RELEASE_OBJECT(pObject);
			return;
		}

		m_arrEntries.push_back(pEntry);

		pEntry->nEntryType   = IN_USE_ENTRY;
		pEntry->unByteOffset = 0;
		pEntry->unGenNo      = 0;
		pEntry->pObject      = pObject;
		pObject->SetRef(m_unStartOffset + m_arrEntries.size() - 1, pEntry->unGenNo);
		pObject->SetIndirect();
	}
    void CXref::WriteTrailer(CStream* pStream)
	{
		CXref* pPrev = m_pPrev;
		if (m_pPrev)
			while (pPrev->m_pPrev) pPrev = pPrev->m_pPrev;
		unsigned int unMaxObjId = m_pPrev ? pPrev->m_arrEntries.size() + pPrev->m_unStartOffset : m_arrEntries.size() + m_unStartOffset;

		m_pTrailer->Add("Size", unMaxObjId);
		if (m_pPrev)
			m_pTrailer->Add("Prev", pPrev->m_unAddr);

		pStream->WriteStr("trailer\012");
		pStream->Write(m_pTrailer, NULL);
		pStream->WriteStr("\012startxref\012");
		pStream->WriteUInt(m_unAddr);
		pStream->WriteStr("\012%%EOF\012");
	}
    void CXref::WriteToStream(CStream* pStream, CEncrypt* pEncrypt, bool bStream)
	{
		char sBuf[SHORT_BUFFER_SIZE];
		char* pBuf;
		char* pEndPtr = sBuf + SHORT_BUFFER_SIZE - 1;

		CXref* pXref = this;
		TXrefEntry* pNextFreeObj = NULL;
		while (pXref)
		{
			for (unsigned int unIndex = 0, unCount = pXref->m_arrEntries.size(); unIndex < unCount; unIndex++)
			{
				TXrefEntry* pEntry = pXref->m_arrEntries.at(unIndex);
				if (pEntry->nEntryType == FREE_ENTRY)
				{
					if (pNextFreeObj)
						pNextFreeObj->unByteOffset = pXref->m_unStartOffset;
					pNextFreeObj = pEntry;
				}
				else
				{
					unsigned int unObjId = pXref->m_unStartOffset + unIndex;
					unsigned int unGenNo = pEntry->unGenNo;

					pEntry->unByteOffset = pStream->Tell();

					if (pEncrypt)
						pEncrypt->InitKey(unObjId, unGenNo);

					pBuf = sBuf;
					pBuf = ItoA(pBuf, unObjId, pEndPtr);
					*pBuf++ = ' ';
					pBuf = ItoA(pBuf, unGenNo, pEndPtr);
					StrCpy(pBuf, " obj\012", pEndPtr);

					pStream->WriteStr(sBuf);
					pEntry->pObject->WriteValue(pStream, pEncrypt);
					pStream->WriteStr("\012endobj\012");
				}
			}
			pXref = pXref->m_pPrev;
		}
		if (pNextFreeObj)
			pNextFreeObj->unByteOffset = 0;

		if (bStream)
		{
			CXref* pPrev = m_pPrev;
			if (m_pPrev)
				while (pPrev->m_pPrev) pPrev = pPrev->m_pPrev;
			unsigned int unMaxObjId = m_pPrev ? pPrev->m_arrEntries.size() + pPrev->m_unStartOffset : m_arrEntries.size() + m_unStartOffset;

			CDictObject* pTrailer = m_pTrailer;
			pTrailer->Add("Type", "XRef");
			pTrailer->Add("Size", unMaxObjId + 1);
			if (m_pPrev)
				pTrailer->Add("Prev", pPrev->m_unAddr);
			CArrayObject* pW = new CArrayObject();
			pTrailer->Add("W",  pW);
			pW->Add(1);
			pW->Add(3);
			pW->Add(2);
			CArrayObject* pIndex = new CArrayObject();
			pTrailer->Add("Index",  pIndex);
			CNumberObject* pLength = new CNumberObject(0);
			pTrailer->Add("Length", pLength);
#ifndef FILTER_FLATE_DECODE_DISABLED
			pTrailer->SetFilter(STREAM_FILTER_FLATE_DECODE);
			pTrailer->Add("Filter", "FlateDecode");
#endif

			// Сортируем pXref, Index должен быть в порядке возрастания
			pXref = this;
			CXref* q, *p, *pr, *out = NULL;
			while (pXref)
			{
				q = pXref;
				pXref = pXref->m_pPrev;
				for ( p = out, pr = NULL; p && (q->m_unStartOffset > p->m_unStartOffset); pr = p, p = p->m_pPrev);
				if (pr)
				{
					q->m_pPrev = p;
					pr->m_pPrev = q;
				}
				else
				{
					q->m_pPrev = out;
					out = q;
				}
			}

			// Записываем поток
			pXref = out;
			int nStreamOffset = pStream->Tell();
			pXref->m_unAddr = nStreamOffset;
			CStream* pTrailerStream = new CMemoryStream();
			unsigned int unEntries = 0, unEntriesSize = 0;
			while (pXref)
			{
				unsigned int unNewEntries = pXref->m_unStartOffset;
				unsigned int unNewEntriesSize = (unsigned int)pXref->m_arrEntries.size() + (pXref->m_pPrev ? 0 : 1);
				if (unNewEntries == unEntries + unEntriesSize)
					unEntriesSize += unNewEntriesSize;
				else
				{
					pIndex->Add(unEntries);
					pIndex->Add(unEntriesSize);

					unEntries = unNewEntries;
					unEntriesSize = unNewEntriesSize;
				}

				for (unsigned int unIndex = 0, unCount = pXref->m_arrEntries.size(); unIndex < unCount; unIndex++)
				{
					TXrefEntry* pEntry = pXref->GetEntry(unIndex);

					if (pEntry->nEntryType == FREE_ENTRY)
						pTrailerStream->WriteChar('\000');
					else if (pEntry->nEntryType == IN_USE_ENTRY)
						pTrailerStream->WriteChar('\001');
					pTrailerStream->WriteChar((unsigned char)(pEntry->unByteOffset >> 16));
					pTrailerStream->WriteChar((unsigned char)(pEntry->unByteOffset >> 8));
					pTrailerStream->WriteChar((unsigned char)(pEntry->unByteOffset));
					pTrailerStream->WriteChar((unsigned char)(pEntry->unGenNo >> 8));
					pTrailerStream->WriteChar((unsigned char)(pEntry->unGenNo));
				}

				pXref = pXref->m_pPrev;
			}

			// Добавляем последний элемент
			pIndex->Add(unEntries);
			pIndex->Add(unEntriesSize);
			pTrailerStream->WriteChar('\001');
			pTrailerStream->WriteChar((unsigned char)(nStreamOffset >> 16));
			pTrailerStream->WriteChar((unsigned char)(nStreamOffset >> 8));
			pTrailerStream->WriteChar((unsigned char)(nStreamOffset));
			pTrailerStream->WriteChar('\000');
			pTrailerStream->WriteChar('\000');

			// Фильтруем поток, pEncrypt = NULL поток перекрестных ссылок не шифруется
			CStream* pFlateStream = new CMemoryStream();
			pFlateStream->WriteStream(pTrailerStream, pTrailer->GetFilter(), NULL);
			pLength->Set(pFlateStream->Size());

			pBuf = sBuf;
			pBuf = ItoA(pBuf, unMaxObjId, pEndPtr);
			*pBuf++ = ' ';
			pBuf = ItoA(pBuf, 0, pEndPtr);
			StrCpy(pBuf, " obj\012", pEndPtr);

			// Записываем cross-reference table
			pStream->WriteStr(sBuf);
			pTrailer->WriteValue(pStream, NULL);
			pStream->WriteStr("\012stream\015\012");
			pStream->WriteStream(pFlateStream, 0, NULL);
			pStream->WriteStr("\012endstream\012endobj\012startxref\012");
			pStream->WriteUInt(nStreamOffset);
			pStream->WriteStr("\012%%EOF\012");

			RELEASEOBJECT(pFlateStream);
			RELEASEOBJECT(pTrailerStream);
			return;
		}

		// Записываем cross-reference table
		pXref = this;
		pXref->m_unAddr = pStream->Tell();
		pStream->WriteStr("xref\012");
		while (pXref)
		{
			if (pXref->m_arrEntries.size())
			{
				pStream->WriteInt(pXref->m_unStartOffset);
				pStream->WriteChar(' ');
				pStream->WriteInt(pXref->m_arrEntries.size());
				pStream->WriteChar('\012');
			}

			for (unsigned int unIndex = 0, unCount = pXref->m_arrEntries.size(); unIndex < unCount; unIndex++)
			{
				TXrefEntry* pEntry = pXref->GetEntry(unIndex);

				pBuf = sBuf;
				pBuf = ItoA2(pBuf, pEntry->unByteOffset, BYTE_OFFSET_LEN + 1);
				*pBuf++ = ' ';
				pBuf = ItoA2(pBuf, pEntry->unGenNo, GEN_NO_LEN + 1);
				*pBuf++ = ' ';
				*pBuf++ = pEntry->nEntryType;
				StrCpy(pBuf, "\015\012", pEndPtr);

				pStream->WriteStr(sBuf);
			}
			pXref = pXref->m_pPrev;
		}

		// Записываем Trailer
		WriteTrailer(pStream);
	}
	bool CXref::IsPDFA() const
	{
		return m_pDocument->IsPDFA();
	}
}
