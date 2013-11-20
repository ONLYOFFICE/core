
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCell.h" 
#include <boost/foreach.hpp>


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
			return 
				XML::XElement(ns.w + "tc",
					XML::Write(Properties) + 
					XML::Write(Items) +
					XML::WriteIf(XML::XElement(ns.w + "p"), !hasParagraph())
				);
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
			BOOST_FOREACH(const TextItem& item, *Items)
			{
				if (item.is<Paragraph>())
					return true;
			}
			return false;
		}

	} // namespace Logic
} // namespace OOX