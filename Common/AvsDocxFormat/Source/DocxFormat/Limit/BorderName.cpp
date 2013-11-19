
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BorderName.h"


namespace OOX
{
	namespace Limit
	{
		BorderNameSet::BorderNameSet()
		{
			_list.insert("top");
			_list.insert("left");
			_list.insert("bottom");
			_list.insert("right");
			_list.insert("insideH");
			_list.insert("insideV");
		}

		BorderName::BorderName()
		{
			//add("top");
			//add("left");
			//add("bottom");
			//add("right");
			//add("insideH");
			//add("insideV");
		}

		BorderNameSet BorderName::_set;
		const std::string BorderName::no_find() const
		{
			return "top";
		}

		void BorderName::operator()(std::string& _value, Parameter value)
		{
			if (_set._list.find(value) != _set._list.end())
				_value = value;
			else
				_value = no_find();
		}
	} // namespace Limit
} // namespace OOX