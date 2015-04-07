#pragma once
#ifndef OOX_RID_INCLUDE_H_
#define OOX_RID_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/XML/xmlutils.h"

namespace PPTX
{
	class RId
	{
	public:
		RId() : m_id(_T("rId0"))
		{
		}
		RId(const size_t id)
		{
			m_id = _T("rId") + XmlUtils::UIntToString(id);
		}
		RId(const CString& rid)
		{
			m_id = rid;
		}
		RId(const RId& oSrc)
		{
			m_id = oSrc.m_id;
		}

	public:
		const RId& operator= (const size_t id)
		{
			m_id = _T("rId") + XmlUtils::UIntToString(id);
			return *this;
		}		
		const RId& operator= (const CString& rid)
		{
			m_id = rid;			
			return *this;
		}
#if defined(_WIN32) || defined (_WIN64)
		const RId& operator= (const BSTR& rid)
		{
			m_id = (CString)rid;	
			return *this;
		}
#endif
		const RId& operator= (const RId& oSrc)
		{
			m_id = oSrc.m_id;
			return *this;
		}

	public:
		const bool operator ==(const RId& lhs) const
		{
			return m_id == lhs.m_id;
		}
		const bool operator !=(const RId& lhs) const
		{
			return m_id != lhs.m_id;
		}
		const bool operator < (const RId& lhs) const
		{
			return m_id < lhs.m_id;
		}
		const bool operator <=(const RId& lhs) const
		{
			return m_id <= lhs.m_id;
		}
		const bool operator >(const RId& lhs) const
		{
			return m_id > lhs.m_id;
		}
		const bool operator >=(const RId& lhs) const
		{
			return m_id >= lhs.m_id;
		}

		AVSINLINE CString get() const { return m_id; }

	public:
		const RId	next() const
		{
			return RId(m_id + _T("1"));
		}
		
	public:
		const CString ToString() const
		{
			return m_id;
		}

		template<typename T>
		void toPPTY(BYTE type, T pWriter) const
		{
			pWriter->WriteBYTE(type);
			pWriter->WriteStringW(m_id);
		}

	private:
		CString m_id;
	};
} // namespace PPTX

#endif // OOX_RID_INCLUDE_H_
