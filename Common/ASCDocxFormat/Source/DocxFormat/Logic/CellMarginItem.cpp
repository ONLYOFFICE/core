
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "CellMarginItem.h"


namespace OOX
{
	namespace Logic
	{

		CellMarginItem::CellMarginItem()
		{
		}

		CellMarginItem::CellMarginItem(int width, const std::string& typeMar)
			:W(width),
			 Type(typeMar)
		{
		}



		CellMarginItem::~CellMarginItem()
		{
		}
	} // namespace Logic
} // namespace OOX