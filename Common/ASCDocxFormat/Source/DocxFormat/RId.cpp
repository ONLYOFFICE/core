
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RId.h"
#include "ToString.h"
#include "Parse.h"
#include <utility>

namespace 
{
    // Вернуть следующий Id по текущему
    /*
    _ASSERTE(nextId("rId1") == "rId2" );
    _ASSERTE(nextId("rId234sf") == "rId235sf" );
    _ASSERTE(nextId("rId2sf4") == "rId3sf4" );
    _ASSERTE(nextId("rIdHp2") == "rIdHp3" );
    _ASSERTE(nextId("rIdMyId") == "rIdMyId1" );
    */

    std::string nextId(const std::string & id)
    {
        std::locale loc;
        std::string::const_iterator i;

        std::string prefix;
        int numId = 0;
        std::string suffix;
        for (i = id.begin(); i != id.end() && !std::isdigit(*i, loc); ++i);

        prefix = std::string(id.begin(), i);
        if (i != id.end())
        {
            std::stringstream strm(std::string(i, id.end()));
            strm >> numId;
            strm >> suffix;
        }

        return prefix + (boost::lexical_cast<std::string>(numId+1)) + suffix;
    }
}

namespace OOX
{
	RId::RId()
	{
	}


	RId::RId(const size_t id)
		: m_id("rId" + boost::lexical_cast<std::string>(id))
	{
	}


	RId::RId(const std::string& rid)
	{
		//m_id = Parse<size_t>(rid.substr(3));
        m_id = rid;
	}


	const RId& RId::operator= (const size_t id)
	{
        m_id = "rId" + boost::lexical_cast<std::string>(id);
		return *this;
	}

	
	const RId& RId::operator= (const std::string& rid)
	{
		//m_id = Parse<int>(rid.substr(3));
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
		//return RId(m_id + 1);
        return RId(nextId(m_id));
	}


	const std::string RId::ToString() const
	{
		//return "rId" + ::ToString(m_id);
        return m_id;
	}

} // namespace OOX