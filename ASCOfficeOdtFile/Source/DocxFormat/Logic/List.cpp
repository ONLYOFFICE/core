
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "List.h"
#include "Paragraph.h"


namespace OOX
{
	namespace Logic
	{

		List::List()
		{
			Num = 0;
		}


		List::~List()
		{
		}


		//List::List(const XML::XNode& node)
		//{
		//	add(node);
		//}


		//const List& List::operator =(const XML::XNode& node)
		//{
		//	add(node);
		//	return *this;
		//}


		//void List::fromXML(const XML::XNode& node)
		//{			
		//}


		//const XML::XNode List::toXML() const
		//{
		//	return XML::XContainer(XML::Write(Items));
		//}


//		void List::add(const XML::XNode& node)
//		{
//			const XML::XElement element(node);
//			if (Items->empty())
//			{
//				Num		= element.element("pPr").element("numPr").element("numId").attribute("val").value();
//				Level = element.element("pPr").element("numPr").element("ilvl").attribute("val").value();
//			}
//			if (element.element("pPr").element("numPr").element("ilvl").attribute("val").value() == Level)
//			{	
//				TextItem text(element);
////				text.inList();
//				ListItem listItem;
//				listItem.Items->push_back(text);
//				Items->push_back(listItem);
//			}
//			else
//				Items->back().add(element);
//		}


		//void List::add(const TextItem& text)
		//{
		//	Items->back().add(text);
		//}


		void List::add(const Paragraph& text)
		{
			if(Items->empty())
			{
				int num = text.GetListNum();
				if(num > 0)
				{
					Num = num;
					Items->push_back(text);
				}
			}
			else
			{
				int num = text.GetListNum();
				if(*Num == num)
					Items->push_back(text);
			}
		}


		const bool List::elementFromThisList(const XML::XNode& node) const
		{
			const XML::XElement element(node);
			return (element.element("pPr").element("numPr").element("numId").attribute("val").value() == Num);
		}
	} // namespace Logic
} // namespace OOX