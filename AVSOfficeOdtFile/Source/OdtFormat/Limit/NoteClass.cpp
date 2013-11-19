
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NoteClass.h"


namespace Odt
{
	namespace Limit
	{
		NoteClass::NoteClass()
		{
			add("footnote");
			add("endnote");
		}

		const std::string NoteClass::no_find() const
		{
			return "footnote";
		}
	} // namespace Limit
} // namespace Odt