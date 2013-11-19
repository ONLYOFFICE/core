#include "LastView.h"


namespace PPTX
{
	namespace Limit
	{
		LastView::LastView()
		{
			add("handoutView");
			add("notesMasterView");
			add("notesView");
			add("outlineView");
			add("sldMasterView");
			add("sldSorterView");
			add("sldThumbnailView");
			add("sldView");
		}

		const std::string LastView::no_find() const
		{
			return "sldThumbnailView";
		}
	} // namespace Limit
} // namespace PPTX