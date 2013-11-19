
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Border.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>


namespace Odt
{
	namespace Logic
	{
		Border::Border()
		{
		}


		Border::Border(const std::string& value)
		{
			if (value != "none")
			{
				std::vector<std::string> vector;
				boost::split(vector, value, boost::is_any_of(" "));

				int nSize = vector.size();

				if (0 < nSize)
					Width = vector[0];
				if (1 < nSize)
					BorderType = vector[1];
				if (2 < nSize)
					Color = vector[2];
			}
		}

		const std::string Border::ToString() const
		{
			if (Width.is_init())
				return Width.ToString() + " " + BorderType.ToString() + " " + Color.ToString();
			return "none";
		}


		const bool Border::IsNone() const
		{
			return !(Width.is_init() || BorderType.is_init() || Color.is_init());
		}
	} // namespace Logic
} // namespace Odt