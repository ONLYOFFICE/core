
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RId.h"
#include "ToString.h"
#include "Parse.h"
#include <utility>
#include "ASCSTLUtils.h"

namespace 
{
	std::wstring nextId(const std::wstring & id)
	{
		std::locale loc;
		std::wstring::const_iterator i;

		std::wstring prefix;
		int numId = 0;
		std::wstring suffix;
		for (i = id.begin(); i != id.end() && !std::isdigit(*i, loc); ++i);

		prefix = std::wstring(id.begin(), i);
		if (i != id.end())
		{
			std::wstringstream strm(std::wstring(i, id.end()));
			strm >> numId;
			strm >> suffix;
		}

		return prefix + StlUtils::IntToWideString(numId+1) + suffix;
	}
}

namespace OOX
{
	RId::RId()
	{
	}

	RId::RId(const size_t id) : m_id(L"rId" + StlUtils::IntToWideString(id))
	{
	}

	RId::RId(const std::wstring& rid)
	{
		m_id = rid;
	}

	const RId& RId::operator= (const size_t id)
	{
		m_id = L"rId" +  StlUtils::IntToWideString(id);
		return *this;
	}

	const RId& RId::operator= (const std::wstring& rid)
	{
		m_id = rid;
		return *this;
	}

	const bool RId::operator ==(const RId& lhs) const
	{
		return m_id == lhs.m_id;
	}

	const bool RId::operator !=(const RId& lhs) const
	{
		return m_id != lhs.m_id;
	}

	const bool RId::operator < (const RId& lhs) const
	{
		return m_id < lhs.m_id;
	}

	const bool RId::operator <=(const RId& lhs) const
	{
		return m_id <= lhs.m_id;
	}

	const bool RId::operator >(const RId& lhs) const
	{
		return m_id > lhs.m_id;
	}

	const bool RId::operator >=(const RId& lhs) const
	{
		return m_id >= lhs.m_id;
	}

	const RId RId::next() const
	{
		return RId(nextId(m_id));
	}

	const std::wstring RId::ToString() const
	{
		return m_id;
	}

} // namespace OOX