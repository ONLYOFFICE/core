
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Clip.h"
#include "ToString.h"


namespace Odt
{
	namespace Logic
	{

		Clip::Clip(const std::string& value)
		{
			int first = 0;
			int second = value.find_first_of("(");

			m_type = value.substr(first, second - first);
			first = value.find_first_of("0123456789", second);

			second = value.find_first_of(", ", first);
			m_right = value.substr(first, second - first);
			first = value.find_first_of("0123456789", second);

			second = value.find_first_of(", ", first);
			m_top = value.substr(first, second - first);
			first = value.find_first_of("0123456789", second);

			second = value.find_first_of(", ", first);
			m_left = value.substr(first, second - first);
			first = value.find_first_of("0123456789", second);

			second = value.find_first_of(")", first);
			m_bottom = value.substr(first, second - first);
		}


		const std::string Clip::ToString() const
		{
			return	m_type.ToString() + 
							"("  + 
							m_right.ToString() + 
							", " +
							m_top.ToString() +
							", " +
							m_left.ToString() +
							", " +
							m_bottom.ToString() + 
							")";
		}

	} // namespace Logic
} // namespace Odt