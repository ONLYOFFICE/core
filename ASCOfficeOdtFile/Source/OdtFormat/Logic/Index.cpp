
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Index.h"
#include "./../Unit.h"


namespace Odt
{
	namespace Logic
	{

		Index::Index()
		{
		}


		Index::Index(const Common::Index& index)
		{
			fromBase(index);
		}


		Index::Index(const std::string& value)
		{
			fromString(value);
		}


		const Index& Index::operator= (const Common::Index& index)
		{
			fromBase(index);
			return *this;
		}


		const Index& Index::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string Index::ToString() const
		{
			switch (type())
			{
			case super:
				return "super 58%";
			case normal:
				return "0% 100%";
			case sub:
				return "sub 58%";
			default:
				return "0% 100%";
			}
		}


		void Index::fromString(const std::string& value)
		{
			std::string ind = value.substr(0, value.find(" "));
			if (ind == "super")
				setSuper();
			else if (ind == "sub")
				setSub();
			else
			{
				UnitPercent un = ind;
				if (un.value() > 0)
					setSuper();
				else if (un.value() < 0)
					setSub();
				else
					setNormal();
			}
		}

	} // namespace Logic
} // namespace Odt