
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Index.h"


namespace OOX
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
				return "superscript";
			case normal:
				return "";
			case sub:
				return "subscript";
			default:
				return "";
			}
		}


		void Index::fromString(const std::string& value)
		{
			if (value == "superscript")
				setSuper();
			else if (value == "subscript")
				setSub();
			else
				setNormal();
		}
	} // namespace Logic
} // namespace OOX