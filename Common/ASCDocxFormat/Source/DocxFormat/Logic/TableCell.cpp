
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCell.h" 

namespace OOX
{
	namespace Logic
	{

		TableCell::TableCell()
		{
		}


		TableCell::~TableCell()
		{
		}
	

		TableCell::TableCell(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableCell& TableCell::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCell::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Properties	= element.element("tcPr");
			XML::Fill(Items, element , "p", "tbl");
		}


		const XML::XNode TableCell::toXML() const
		{
		return XML::XElement();
		}


		const bool TableCell::isFromHMerged() const
		{
			return Properties->GridSpan.is_init();
		}


		const bool TableCell::isFromVMergedButNotFirst() const
		{
			if (Properties->VMerge.is_init())
				return !Properties->VMerge->Value.is_init();
			return false;
		}
		
		
		const bool TableCell::isFirstFromVMerged() const
		{
			if (Properties->VMerge.is_init())
				return Properties->VMerge->Value.is_init();
			return false;
		}


		const bool TableCell::hasParagraph() const
		{
			const std::vector<TextItem>& runs = Items.get();

			for (std::vector<TextItem>::const_iterator iter = runs.begin(); iter != runs.end(); ++iter)
			{
				if ((*iter).is<Paragraph>())
					return true;
			}

			return false;
		}

	} // namespace Logic
} // namespace OOX