
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BorderLine.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>


namespace Odt
{
	namespace Logic
	{
		BorderLine::BorderLine()
		{
		}


		BorderLine::BorderLine(const std::string& value)
		{
			std::vector<std::string> vector;
			boost::split(vector, value, boost::is_any_of(" "));

			FirstWidth  = vector[0];
			SecondWidth = vector[1];
			ThirdWidth  = vector[2];
		}

		const std::string BorderLine::ToString() const
		{
			return FirstWidth.ToString() + " " + SecondWidth.ToString() + " " + ThirdWidth.ToString();
		}
	} // namespace Logic
} // namespace Odt