
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OleZIndex.h"


namespace OOX
{
	namespace Logic
	{

		OleZIndex::OleZIndex()
		{
		}


		OleZIndex::OleZIndex(const Common::ZIndex& zIndex)
		{
			fromBase(zIndex);
		}


		const std::string OleZIndex::toString() const
		{		
			return "z-index:" + Index.ToString();
		}


		const OleZIndex& OleZIndex::operator= (const Common::ZIndex& zIndex)
		{
			fromBase(zIndex);
			return *this;
		}

	} // namespace Logic
} // namespace OOX