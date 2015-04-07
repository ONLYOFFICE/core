#pragma once
#ifndef OOX_RID_INCLUDE_H_
#define OOX_RID_INCLUDE_H_

#include "../Base/Base.h"
#include "../XML/xmlutils.h"

namespace OOX
{
	class RId
	{
	public:
		RId() : m_id(0)
		{
		}
		RId(const size_t id) : m_id(id)
		{
		}
		RId(const CString& rid)
		{
			(*this) = rid;
		}
		RId(const RId& oSrc)
		{
			(*this) = oSrc;
		}

	public:
		//const RId& operator= (const size_t id)
		//{
		//	m_id = id;
		//	return *this;
		//}		
		const RId& operator= (const CString& rid)
		{
			//Учитывает только rid начинающиеся с rId, остальные сохраняем так как есть
			//Тогда не будет проблем с добавление новый id, мы всегда будем генерировать их с префиксом rId
			CString sFindString(_T("rId"));
			int nFindStringLength = sFindString.GetLength();
			if(0 == rid.Find(sFindString) && rid.GetLength() > nFindStringLength && 0 != isdigit(rid[nFindStringLength]))
			{
				CString strParam = rid.Mid(nFindStringLength);
				m_id = XmlUtils::GetUInteger(strParam.GetBuffer());
			}
			else
			{
				m_id = 0;
				m_sId = rid;
			}
			
			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		const RId& operator= (const BSTR& rid)
		{
			(*this) = (CString)rid;
			
			return *this;
		}
#endif
		const RId& operator= (const RId& oSrc)
		{
			m_id = oSrc.m_id;
			m_sId = oSrc.m_sId;
			return *this;
		}

	public:
		const bool operator ==(const RId& lhs) const
		{
			return m_id == lhs.m_id && m_sId == lhs.m_sId;
		}
		const bool operator !=(const RId& lhs) const
		{
			return !operator ==(lhs);
		}
		//const bool operator < (const RId& lhs) const
		//{
		//	return m_id < lhs.m_id;
		//}
		//const bool operator <=(const RId& lhs) const
		//{
		//	return m_id <= lhs.m_id;
		//}
		//const bool operator >(const RId& lhs) const
		//{
		//	return m_id > lhs.m_id;
		//}
		//const bool operator >=(const RId& lhs) const
		//{
		//	return m_id >= lhs.m_id;
		//}

		AVSINLINE CString get() const 
		{
			return ToString();
		}
		AVSINLINE size_t getNumber() const { return m_id; }
	public:
		const RId	next() const
		{
			return RId(m_id + 1);
		}
		
	public:
		const CString ToString() const
		{
			if(!m_sId.IsEmpty())
				return m_sId;
			else
				return _T("rId") + XmlUtils::UIntToString(m_id);
		}

	private:
		size_t m_id;
		CString m_sId;
	};
} // namespace OOX

#endif // OOX_RID_INCLUDE_H_
